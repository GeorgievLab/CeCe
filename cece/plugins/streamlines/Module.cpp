/* ************************************************************************ */
/* Georgiev Lab (c) 2015-2016                                               */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/*                                                                          */
/* This file is part of CeCe.                                               */
/*                                                                          */
/* CeCe is free software: you can redistribute it and/or modify             */
/* it under the terms of the GNU General Public License as published by     */
/* the Free Software Foundation, either version 3 of the License, or        */
/* (at your option) any later version.                                      */
/*                                                                          */
/* CeCe is distributed in the hope that it will be useful,                  */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of           */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            */
/* GNU General Public License for more details.                             */
/*                                                                          */
/* You should have received a copy of the GNU General Public License        */
/* along with CeCe.  If not, see <http://www.gnu.org/licenses/>.            */
/*                                                                          */
/* ************************************************************************ */

// Declaration
#include "cece/plugins/streamlines/Module.hpp"

// C++
#include <algorithm>

// CeCe
#include "cece/core/Assert.hpp"
#include "cece/core/constants.hpp"
#include "cece/core/StaticArray.hpp"
#include "cece/core/DynamicArray.hpp"
#include "cece/core/Exception.hpp"
#include "cece/core/VectorRange.hpp"
#include "cece/core/constants.hpp"
#include "cece/core/FileStream.hpp"
#include "cece/core/CsvFile.hpp"
#include "cece/core/TimeMeasurement.hpp"
#include "cece/core/BinaryInput.hpp"
#include "cece/core/BinaryOutput.hpp"
#include "cece/core/ShapeToGrid.hpp"
#include "cece/core/Log.hpp"
#include "cece/core/UnitIo.hpp"
#include "cece/object/Object.hpp"
#include "cece/simulator/TimeMeasurement.hpp"
#include "cece/simulator/Simulation.hpp"

// Plugin
#include "cece/plugins/streamlines/NoDynamics.hpp"
#include "cece/plugins/streamlines/BounceBackDynamics.hpp"
#include "cece/plugins/streamlines/BgkDynamics.hpp"
#include "cece/plugins/streamlines/ZouHeDynamics.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace streamlines {

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

constexpr StaticArray<char, 5> FILE_GUARD{{'C', 'E', 'S', 'L', '\0'}};

/* ************************************************************************ */

}

/* ************************************************************************ */

Module::Module(simulator::Simulation& simulation)
    : module::Module(simulation)
    , m_boundaries(simulation)
{
    // Nothing to do
}

/* ************************************************************************ */

Module::~Module() = default;

/* ************************************************************************ */

void Module::init(AtomicBool& termFlag)
{
    // Print simulation info
    printInfo();

    // Set fluid dynamics
    setFluidDynamics(createFluidDynamics());
    setWallDynamics(createWallDynamics());

    // Initialize boundary positions
    for (int i = 0; i < 4; ++i)
        m_boundaries[i].setDynamics(createBorderDynamics(m_boundaries[i].getPosition()));

    // Initialize boundaries
    m_boundaries.init();

    // Obstacles
    updateObstacleMap();

    Log::info("[streamlines] Initialization...");

    bool initialized = false;

    if (!m_initFile.empty() && fileExists(m_initFile))
    {
        try
        {
            Log::info("[streamlines] Loading from external file: ", m_initFile);

            loadFromFile(m_initFile);
            initialized = true;
        }
        catch (const Exception& e)
        {
            Log::warning("[streamlines] ", e.what());
        }
    }

    if (!initialized)
    {
        m_lattice.initEquilibrium();

        // Initialization iterations
        for (simulator::IterationNumber it = 1; it <= getInitIterations(); it++)
        {
            if ((it % 100) == 0)
                Log::info("[streamlines] Initialization ", it, "/", getInitIterations());

            if (termFlag)
            {
                Log::info("[streamlines] Initialization interrupted.");
                return;
            }

            m_lattice.collideAndStream();

            // Apply boundary conditions
            applyBoundaryConditions();
        }

        // Store initialization
        if (!m_initFile.empty())
        {
            storeToFile(m_initFile);
        }
    }

    Log::info("[streamlines] Initialization done.");
}

/* ************************************************************************ */

void Module::update()
{
    Assert(m_lattice.getSize() != Zero);

    auto _ = measure_time("streamlines", simulator::TimeMeasurement(getSimulation()));

    // Store streamlines data
    if (m_dataOut)
        storeData();

    // No recalculation
    if (isDynamic())
    {
        // Obstacles
        updateObstacleMap();

#ifdef CECE_THREAD_SAFE
        // Lock access
        MutexGuard guard(m_mutex);
#endif

        // Compute inner iterations
        for (simulator::IterationNumber it = 0; it < getInnerIterations(); it++)
        {
            // Collide and propagate
            m_lattice.collideAndStream();

            // Apply boundary conditions
            applyBoundaryConditions();
        }
    }

    // Apply streamlines to world objects
    applyToObjects();
}

/* ************************************************************************ */

void Module::loadConfig(const config::Configuration& config)
{
    // Configure parent
    module::Module::loadConfig(config);

    // Set streamlines dynamicity
    setDynamic(config.get("dynamic", isDynamic()));

    // Number of init iterations
    setInitIterations(config.get("init-iterations", getInitIterations()));

    // Number of inner iterations
    setInnerIterations(config.get("inner-iterations", getInnerIterations()));

    // Load boundaries config
    m_boundaries.loadConfig(config);

    // Load converter configuration
    m_converter.loadConfig(config);
    m_converter.setCharTime(getSimulation().getTimeStep());

    // Obsolete grid
    auto gridSize = config.get<Lattice::Size>("grid", Lattice::Size{Zero});

    if (gridSize != Zero)
    {
        Log::warning("[streamlines] Config option 'grid' is obsolete!");

        // Grid size
        m_lattice.setSize(gridSize);

        // Compute characteristic length
        m_converter.setNumberNodes(
            gridSize.getWidth() / getSimulation().getWorldSize().getWidth() * m_converter.getCharLength()
        );
    }
    else
    {
        // Calculate lattice size
        const auto size = Lattice::Size(
            getSimulation().getWorldSize() / m_converter.getCharLength() * m_converter.getNumberNodes()
        );

        if (size == Zero)
            throw InvalidArgumentException("Lattice size cannot be zero");

        // Grid size
        m_lattice.setSize(size + Lattice::Size(1, 1));
    }

    // Enable dynamic object obstacles
    setDynamicObjectsObstacles(config.get("dynamic-object-obstacles", isDynamicObjectsObstacles()));

    if (config.has("data-out-filename"))
    {
        m_dataOut = makeUnique<CsvFile>(config.get("data-out-filename"));
        m_dataOutDensity = config.get<bool>("data-out-density", false);
        m_dataOutPopulations = config.get<bool>("data-out-populations", false);

        storeDataHeader();
    }

#ifdef CECE_ENABLE_RENDER
    setDebugMagnitudeScale(config.get("debug-magnitude-scale", getDebugMagnitudeScale()));
#endif

    // Get initialization file
    if (config.has("init-file"))
    {
        // Get file name
        auto file = config.get("init-file");

        // In case of %temp%
        if (file.substr(0, 6) == "%temp%")
            m_initFile = tempDirectory() / file.substr(6);
        else
            m_initFile = file;
    }
}

/* ************************************************************************ */

void Module::storeConfig(config::Configuration& config) const
{
    module::Module::storeConfig(config);

    config.set("init-iterations", getInitIterations());
    config.set("inner-iterations", getInnerIterations());

    // Store converter config
    m_converter.storeConfig(config);

#ifdef CECE_ENABLE_RENDER
    config.set("debug-magnitude-scale", getDebugMagnitudeScale());
#endif

    // TODO: init file
}

/* ************************************************************************ */

#ifdef CECE_ENABLE_RENDER
void Module::draw(render::Context& context)
{
    if (getDrawFlags() == 0)
        return;

    const auto size = m_lattice.getSize();

    if (!m_drawableDebug)
        m_drawableDebug.create(context, size);

    // Temporary for velocities
    Grid<Descriptor::VelocityType> velocities(size);

    // Calculate grid max velocity
    const auto maxInlet = m_boundaries.getMaxInletVelocity();
    const auto maxVel = m_debugMagnitudeScale * m_converter.convertVelocity(maxInlet);

    {
#ifdef CECE_THREAD_SAFE
        // Lock access
        MutexGuard guard(m_mutex);
#endif

        // Update texture
        for (auto&& c : range(size))
        {
            // Cell alias
            const auto& node = m_lattice[c];
            const auto velocity = node.computeVelocity();
            auto dynamics = node.getDynamics();

            render::Color color = render::colors::BLACK;

            // Node velocity
            velocities[c] = velocity;

            if (dynamics == getFluidDynamics())
            {
                if (checkDebugDraw(DRAW_DEBUG_MAGNITUDE))
                    color = render::Color::fromGray(velocity.getLength() / maxVel);
            }
            else if (checkDebugDraw(DRAW_DEBUG_OBSTACLES))
            {
                if (dynamics == m_boundaries[0].getDynamics() ||
                    dynamics == m_boundaries[1].getDynamics() ||
                    dynamics == m_boundaries[2].getDynamics() ||
                    dynamics == m_boundaries[3].getDynamics())
                {
                    color = render::colors::RED;
                }
                else if (dynamics == NoDynamics::getInstance())
                {
                    color = render::colors::WHITE;
                }
                else
                {
                    color = render::colors::GREEN;
                }
/*
                case Node::Dynamics::DynamicObstacle:
                    color = render::colors::RED;
*/
                // Set alpha channel
                color.setAlpha(0);
            }

            m_drawableDebug->set(c, color);
        }
    }

    if (!m_drawableDirections)
    {
        m_drawableDirections.create(context, size, velocities.getData(), maxVel);
    }
    else
    {
        m_drawableDirections->setMax(maxVel);
        m_drawableDirections->update(velocities.getData());
    }

    // Draw color grid
    context.matrixPush();
    context.matrixScale(getSimulation().getWorldSize() / units::Length(1));

    if (checkDebugDraw(DRAW_DEBUG_MAGNITUDE) || checkDebugDraw(DRAW_DEBUG_OBSTACLES))
        m_drawableDebug->draw(context);

    if (checkDebugDraw(DRAW_DEBUG_DIRECTION))
        m_drawableDirections->draw(context);

    context.matrixPop();
}
#endif

/* ************************************************************************ */

UniquePtr<Dynamics> Module::createFluidDynamics() const
{
    return makeUnique<BgkDynamics>(m_converter.calculateOmega());
}

/* ************************************************************************ */

UniquePtr<Dynamics> Module::createWallDynamics() const
{
    return makeUnique<BounceBackDynamics>();
}

/* ************************************************************************ */

UniquePtr<Dynamics> Module::createBorderDynamics(Boundary::Position pos) const
{
    const auto omega = m_converter.calculateOmega();

    switch (pos)
    {
    case Boundary::Position::Top:
        return makeUnique<ZouHeDynamics>(omega, ZouHeDynamics::Position::Top);

    case Boundary::Position::Bottom:
        return makeUnique<ZouHeDynamics>(omega, ZouHeDynamics::Position::Bottom);

    case Boundary::Position::Left:
        return makeUnique<ZouHeDynamics>(omega, ZouHeDynamics::Position::Left);

    case Boundary::Position::Right:
        return makeUnique<ZouHeDynamics>(omega, ZouHeDynamics::Position::Right);

    default:
        Assert(false && "No way!");
        break;
    }

    return nullptr;
}

/* ************************************************************************ */

void Module::updateObstacleMap()
{
    // Clear previous flag
    m_lattice.setDynamics(getFluidDynamics());

    const PositionVector start = getSimulation().getWorldSize() * -0.5f;
    const auto step = getSimulation().getWorldSize() / m_lattice.getSize();

    // Foreach all cells
    for (auto& obj : getSimulation().getObjects())
    {
        const bool isDynamic = obj->getType() == object::Object::Type::Dynamic;

        // Ignore dynamic objects
        if (!isDynamicObjectsObstacles() && isDynamic)
            continue;

        // Get object position
        const auto pos = obj->getPosition() - start;

        // Check if position is in range
        if (!pos.inRange(Zero, getSimulation().getWorldSize()))
            continue;

        // Get grid position
        const auto coord = Coordinate(pos / step);

        // Calculate object velocity in LB
        const auto velocity = m_converter.convertVelocity(obj->getVelocity());

        // In this case duplicate coordinates doesn't harm and calling
        // operation multiple times on same coordinate is faster than
        // sorting and erasing non-unique coordinates.

        // Map shapes to grid
        for (const auto& shape : obj->getShapes())
        {
            mapShapeToGrid(
                [this, &velocity] (Coordinate&& coord) {
                    Assert(m_lattice.inRange(coord));
                    m_lattice[coord].setDynamics(getWallDynamics());
                },
                [] (Coordinate&& coord) {},
                shape, step, coord, obj->getRotation(), m_lattice.getSize()
            );
        }
    }

    m_lattice.fixupObstacles(getWallDynamics());

    //if (isDynamicObjectsObstacles())
    //    m_lattice.fixupObstacles(Node::Dynamics::DynamicObstacle);
}

/* ************************************************************************ */

void Module::applyToObjects()
{
    // Foreach objects
    for (auto& obj : getSimulation().getObjects())
        applyToObject(*obj);
}

/* ************************************************************************ */

void Module::applyToObject(object::Object& object)
{
    // Ignore static objects
    if (object.getType() != object::Object::Type::Dynamic)
        return;

    // Maximum object speed that is allowed by physical engine
    const auto maxSpeed = getSimulation().getMaxObjectTranslation() / getSimulation().getTimeStep();
    const auto maxSpeedSq = maxSpeed * maxSpeed;

    const PositionVector start = getSimulation().getWorldSize() * -0.5f;
    const auto step = getSimulation().getWorldSize() / m_lattice.getSize();

    // Get mass local center
    const auto center = object.getMassCenterOffset();

    // Coefficient used in force calculation
    const auto forceCoefficient = 6 * constants::PI * m_converter.getKinematicViscosity() * object.getDensity();

    auto force = ForceVector{Zero};
    auto velocityObjEnv = VelocityVector{Zero};

    // Map shapes border to grid
    for (const auto& shape : object.getShapes())
    {
        // Only circle shapes are supported
        if (shape.getType() != ShapeType::Circle)
            continue;

        // Shape alias
        const auto& circle = shape.getCircle();

        // Transform from [-size / 2, size / 2] to [0, size] space
        const auto pos = object.getWorldPosition(circle.center) - start;

        // Check if position is in range
        if (!pos.inRange(Zero, getSimulation().getWorldSize()))
            continue;

        // Get coordinate to lattice
        const auto coord = Coordinate(pos / step);

        Vector<RealType> velocityLB = Zero;
        unsigned long count = 0;

        // Store velocity for each coordinate
        mapShapeBorderToGrid(
            [this, &velocityLB, &count] (Coordinate&& coord) {
                velocityLB += m_lattice[coord].computeVelocity();
                ++count;
            },
            [] (Coordinate&& coord) { },
            shape, step, coord, m_lattice.getSize(), {}, 1
        );

        if (count == 0)
            continue;

        // Average
        velocityLB /= count;
        const VelocityVector velocityEnv = m_converter.convertVelocity(velocityLB);
        //const VelocityVector velocityEnv{m_inletVelocities[0], Zero};

        if (velocityEnv.getLengthSquared() > maxSpeedSq)
        {
            OutStringStream oss;
            oss <<
                "[streamlines] Physical engine can't handle environment "
                "velocity (" << velocityEnv.getLength() << " um/s < " << maxSpeed << " um/s). "
                "Decrease inlet velocities or change topology."
            ;

            Log::warning(oss.str());
            //throw RuntimeException(oss.str());
        }

        velocityObjEnv += velocityEnv;

        // Shape radius
        const auto radius = circle.radius;
        // Distance from mass center
        const auto offset = circle.center - center;

        // Angular velocity
        const auto omega = object.getAngularVelocity();

        // Calculate shape global velocity
        const auto velocity = object.getVelocity() + cross(omega, offset);

        // Difference between environment velocity and shape velocity
        const auto dv = velocityEnv - velocity;

        // Same velocity
        if (dv == Zero)
            continue;

        // Add force from shape
        force += forceCoefficient * radius * dv;
    }

    Assert(object.getShapes().size() > 0);
    velocityObjEnv /= object.getShapes().size();

    // Difference between velocities
    const auto dv = velocityObjEnv - object.getVelocity();

    // Same velocity
    if (dv == Zero)
        return;

    // Maximum impulse
    const auto impulseMax = object.getMass() * dv;

    // Calculate linear impulse from shapes
    auto impulse = force * getSimulation().getTimeStep();

    // Impulse is to big
    if (impulse.getLengthSquared() > impulseMax.getLengthSquared())
    {
        const RealType ratio = impulseMax.getLength() / impulse.getLength();
        impulse *= ratio;
    }

    // Apply impulse
    object.applyLinearImpulse(impulse);
}

/* ************************************************************************ */

void Module::applyBoundaryConditions()
{
    m_boundaries.applyConditions(m_converter, m_lattice, getFluidDynamics());
}

/* ************************************************************************ */

void Module::printInfo()
{
    // Get values
    const auto size = m_lattice.getSize();

    Log::info("[streamlines] Viscosity: ", m_converter.getKinematicViscosity(), " um2/s");
    Log::info("[streamlines] Max object speed: ", getSimulation().getMaxObjectTranslation(), " um/it");
    Log::info("[streamlines] Char. length: ", m_converter.getCharLength(), " um");
    Log::info("[streamlines] Char. time: ", m_converter.getCharTime(), " s");
    Log::info("[streamlines] Char. speed: ", m_converter.getCharVelocity(), " um/s");
    Log::info("[streamlines] Number of nodes: ", m_converter.getNumberNodes());
    Log::info("[streamlines] Number of time steps: ", m_converter.getNumberSteps());
    Log::info("[streamlines] Re: ", m_converter.calculateRe());
    Log::info("[streamlines] ## Lattice ##");
    Log::info("[streamlines] Tau: ", m_converter.calculateTau());
    Log::info("[streamlines] Omega: ", m_converter.calculateOmega());
    Log::info("[streamlines] Grid: (", size.getWidth(), "; ", size.getHeight(), ")");
    Log::info("[streamlines] Viscosity: ", m_converter.calculateViscosity());
}

/* ************************************************************************ */

void Module::storeToFile(const FilePath& filename)
{
    OutFileStream ofs(filename.string(), OutFileStream::binary);
    BinaryOutput out(ofs);

    // Write file guard
    out.write(FILE_GUARD);

    // Write lattice size
    out.write(m_lattice.getSize());

    // Write relaxation time
    out.write(m_converter.calculateTau());

    // Number of init iterations
    out.write(m_initIterations);

    for (auto&& c : range(m_lattice.getSize()))
    {
        const Node& cell = m_lattice[c];

        // Write cell populations
        out.write(cell.getData());
    }
}

/* ************************************************************************ */

void Module::loadFromFile(const FilePath& filename)
{
    InFileStream ifs(filename.string(), OutFileStream::binary);

    if (!ifs.is_open())
        throw InvalidArgumentException("[streamlines] Cannot load from file: File not found '" + filename.string() + "'");

    BinaryInput in(ifs);

    // Read guard
    StaticArray<char, FILE_GUARD.size()> guard;
    in.read(guard);

    if (guard != FILE_GUARD)
        throw InvalidArgumentException("[streamlines] Cannot load from file: File is not valid");

    // Read lattice size
    Lattice::Size size;
    in.read(size);

    if (size != m_lattice.getSize())
        throw InvalidArgumentException("[streamlines] Cannot load from file: different lattice sizes");

    RealType tau;
    in.read(tau);

    if (tau != m_converter.calculateTau())
        throw InvalidArgumentException("[streamlines] Cannot load from file: different relaxation times");

    decltype(m_initIterations) iterations;
    in.read(iterations);

    if (iterations != m_initIterations)
        throw InvalidArgumentException("[streamlines] Cannot load from file: different init iterations");

    for (auto&& c : range(m_lattice.getSize()))
    {
        Node& cell = m_lattice[c];

        // Read cell populations
        in.read(cell.getData());
    }
}

/* ************************************************************************ */

void Module::storeDataHeader()
{
    Assert(m_dataOut);

    m_dataOut->writeHeader(
        "iteration", "totalTime", "x", "y", "velX", "velY",
        CsvFile::cond(m_dataOutDensity, "rho"),
        CsvFile::cond(m_dataOutPopulations, "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7", "d8")
    );

    // Set stored value precision
    m_dataOut->setPrecision(std::numeric_limits<Descriptor::DensityType>::digits10 + 1);
}

/* ************************************************************************ */

void Module::storeData()
{
    Assert(m_dataOut);

    for (auto&& c : range(m_lattice.getSize()))
    {
        const auto& data = m_lattice[c];

        // Do not save data with no dynamics
        if (data.getDynamics() == NoDynamics::getInstance())
            continue;

        const auto vel = m_converter.convertVelocity(data.computeVelocity());

        m_dataOut->writeRecord(
            getSimulation().getIteration() - 1,
            getSimulation().getTotalTime().value(),
            c.getX(),
            c.getY(),
            vel.getX().value(),
            vel.getY().value(),
            CsvFile::cond(m_dataOutDensity,
                data.computeDensity()
            ),
            CsvFile::cond(m_dataOutPopulations,
                data[0],
                data[1],
                data[2],
                data[3],
                data[4],
                data[5],
                data[6],
                data[7],
                data[8]
            )
        );
    }

    m_dataOut->flush();
}

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */

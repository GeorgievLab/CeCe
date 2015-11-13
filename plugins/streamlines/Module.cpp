/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
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
#include "Module.hpp"

// C++
#include <algorithm>

// Simulator
#include "core/Assert.hpp"
#include "core/constants.hpp"
#include "core/StaticArray.hpp"
#include "core/DynamicArray.hpp"
#include "core/Exception.hpp"
#include "core/VectorRange.hpp"
#include "core/constants.hpp"
#include "core/FileStream.hpp"
#include "core/TimeMeasurement.hpp"
#include "simulator/Simulation.hpp"
#include "simulator/Object.hpp"
#include "simulator/Obstacle.hpp"
#include "simulator/ShapeToGrid.hpp"

/* ************************************************************************ */

namespace plugin {
namespace streamlines {

/* ************************************************************************ */

/**
 * @brief Parse layout description.
 *
 * @param is     Input stream.
 * @param layout Output layout.
 *
 * @return is.
 */
InStream& operator>>(InStream& is, Module::Layout& layout)
{
    return is >> std::skipws >>
        layout[Module::LayoutPosTop] >>
        layout[Module::LayoutPosRight] >>
        layout[Module::LayoutPosBottom] >>
        layout[Module::LayoutPosLeft]
    ;
}

/* ************************************************************************ */

/**
 * @brief Read inlet velocities.
 *
 * @param is         Input stream.
 * @param velocities
 *
 * @return is.
 */
InStream& operator>>(InStream& is, Module::InletVelocities& velocities)
{
    return is >> std::skipws >>
        velocities[Module::LayoutPosTop] >>
        velocities[Module::LayoutPosRight] >>
        velocities[Module::LayoutPosBottom] >>
        velocities[Module::LayoutPosLeft]
    ;
}

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

constexpr StaticArray<StaticArray<Vector<int>, 2>, Module::LayoutPosCount> EDGES{{
    {{{ 1, -1}, { 1,  1}}},
    {{{-1, -1}, {-1,  1}}},
    {{{-1,  1}, { 1,  1}}},
    {{{ 1, -1}, {-1, -1}}}
}};

/* ************************************************************************ */

/**
 * @brief Calculate Poiseuille flow profile.
 *
 * @param max  Max speed.
 * @param pos  Position from range.
 * @param size Range size.
 *
 * @return
 */
template<typename T>
units::Velocity calcPoiseuilleFlow(units::Velocity max, T pos, T size) noexcept
{
    Assert(pos >= 0);
    Assert(pos < size);
    Assert(size > 1);

    const RealType posF = static_cast<RealType>(pos);
    const RealType sizeF = static_cast<RealType>(size);
    return 4.f * max / (sizeF * sizeF) * (sizeF * posF - posF * posF);
}

/* ************************************************************************ */

}

/* ************************************************************************ */

Module::Module()
{
    m_layout[LayoutPosTop]    = LayoutType::Barrier;
    m_layout[LayoutPosBottom] = LayoutType::Barrier;
    m_layout[LayoutPosLeft]   = LayoutType::Inlet;
    m_layout[LayoutPosRight]  = LayoutType::Outlet;
}

/* ************************************************************************ */

Module::~Module()
{
    // Nothing to do
}

/* ************************************************************************ */

void Module::init(simulator::Simulation& simulation)
{
    initBarriers(simulation);

    // Physical size of one lattice cell
    const auto dl = simulation.getWorldSize() / m_lattice.getSize();

    // Calculate values
    const auto vMax = calculateMaxVelocity(dl);
    const auto omega = 1.0 / getTau();

    Log::info("[streamlines] Omega: ", omega);
    Log::info("[streamlines] Tau: ", getTau());
    Log::info("[streamlines] Cell size: (", dl.getX(), " um; ", dl.getY(), " um)");
    Log::info("[streamlines] Max velocity: (", vMax.getX(), " um/s; ", vMax.getY(), " um/s)");

    // Obstacles
    updateObstacleMap(simulation, vMax);

    // Initialization iterations
    for (auto it = getInitIterations(); it--; )
    {
        m_lattice.collideAndStream(omega);

        // Apply boundary conditions
        applyBoundaryConditions(simulation, vMax);
    }
}

/* ************************************************************************ */

void Module::initBarriers(simulator::Simulation& simulation)
{
    // Create barriers
    for (unsigned int pos = 0; pos < LayoutPosCount; ++pos)
    {
        if (m_layoutBarriers[pos])
        {
            // Barrier recreation is not needed
            if (m_layout[pos] == LayoutType::Barrier)
                continue;

            simulation.deleteObject(m_layoutBarriers[pos].get());
            m_layoutBarriers[pos] = nullptr;
        }
        else if (m_layout[pos] == LayoutType::Barrier)
        {
            initBorderBarrier(simulation, static_cast<LayoutPosition>(pos));
        }
    }
}

/* ************************************************************************ */

void Module::update(simulator::Simulation& simulation, units::Time dt)
{
    Assert(m_lattice.getSize() != Zero);

    auto _ = measure_time("streamlines", simulator::TimeMeasurementIterationOutput(simulation));

    // Physical size of one lattice cell
    const auto dl = simulation.getWorldSize() / m_lattice.getSize();

    // Calculate maximum flow velocity
    const auto vMax = calculateMaxVelocity(dl);

    // Relaxation parameter
    const auto omega = 1.0 / getTau();

    // Calculate conversion coefficient
    setCoefficient(calculateCoefficient(dt, dl));

    // Obstacles
    updateObstacleMap(simulation, vMax);

#if THREAD_SAFE
    // Lock access
    MutexGuard guard(m_mutex);
#endif

    // Collide and propagate
    for (auto it = getInnerIterations(); it--; )
    {
        m_lattice.collideAndStream(omega);

        // Apply boundary conditions
        applyBoundaryConditions(simulation, vMax);
    }

    // Apply streamlines to world objects
    applyToObjects(simulation, dt, vMax);

    // Store streamlines data
    if (m_dataOut)
    {
        for (auto&& c : range(m_lattice.getSize()))
        {
            const auto& data = m_lattice[c];
            const auto vel = vMax * data.calcVelocityNormalized();

            *m_dataOut <<
                // iteration
                simulation.getIteration() << ";" <<
                // totalTime
                simulation.getTotalTime().value() << ";" <<
                // x
                c.getX() << ";" <<
                // y
                c.getY() << ";" <<
                // d0
                data[0] << ";" <<
                // d1
                data[1] << ";" <<
                // d2
                data[2] << ";" <<
                // d3
                data[3] << ";" <<
                // d4
                data[4] << ";" <<
                // d5
                data[5] << ";" <<
                // d6
                data[6] << ";" <<
                // d7
                data[7] << ";" <<
                // d8
                data[8] << ";" <<
                // rho
                data.calcRho() << ";" <<
                // velX
                vel.getX().value() << ";" <<
                // velY
                vel.getY().value() << "\n"
            ;
        }

        m_dataOut->flush();
    }
}

/* ************************************************************************ */

void Module::loadConfig(simulator::Simulation& simulation, const simulator::Configuration& config)
{
    // Configure parent
    simulator::Module::loadConfig(simulation, config);

    // Number of init iterations
    setInitIterations(config.get("init-iterations", getInitIterations()));

    // Number of inner iterations
    setInnerIterations(config.get("iterations", getInnerIterations()));

    if (getInnerIterations() == 0)
        throw InvalidArgumentException("Number of inner iterations cannot be zero");

    // Convert relaxation time
    setTau(config.get("tau", getTau()));

    if (getTau() == 0)
        throw InvalidArgumentException("Relaxation time cannot be zero");

    if (config.has("inlet-velocity"))
    {
        auto vel = config.get<units::Velocity>("inlet-velocity");
        m_inletVelocities[LayoutPosTop]    = vel;
        m_inletVelocities[LayoutPosBottom] = vel;
        m_inletVelocities[LayoutPosLeft]   = vel;
        m_inletVelocities[LayoutPosRight]  = vel;
    }
    else
    {
        // Inlet velocities
        setInletVelocities(config.get("inlet-velocities", getInletVelocities()));
    }

    // Viscosity
    setKinematicViscosity(config.get("kinematic-viscosity", getKinematicViscosity()));

    if (getKinematicViscosity() == Zero)
        throw InvalidArgumentException("Kinematic viscosity cannot be zero");

    // Grid size
    m_lattice.setSize(config.get<Vector<Lattice::SizeType>>("grid"));

    if (m_lattice.getSize() == Zero)
        throw InvalidArgumentException("Lattice size cannot be zero");

    // Layout
    setLayout(config.get("layout", getLayout()));

    // Enable dynamic object obstacles
    setDynamicObjectsObstacles(config.get("dynamic-object-obstacles", isDynamicObjectsObstacles()));

    if (config.has("data-out"))
    {
        m_dataOut = makeUnique<OutFileStream>(config.get("data-out"));
        *m_dataOut << "iteration;totalTime;x;y;d0;d1;d2;d3;d4;d5;d6;d7;d8;rho;velX;velY\n";
    }

    // Initialize lattice
    init(simulation);
}

/* ************************************************************************ */

#ifdef ENABLE_RENDER
void Module::draw(const simulator::Simulation& simulation, render::Context& context)
{
#if DEV_PLUGIN_streamlines_RENDER
    if (!isDebugDraw())
        return;

    const auto size = m_lattice.getSize();

    if (!m_drawableObstacles)
        m_drawableObstacles.create(context, size);

    // Temporary for velocities
    Grid<Vector<LatticeCell::ValueType>> velocities(size);

    {
#if THREAD_SAFE
        // Lock access
        MutexGuard guard(m_mutex);
#endif

        // Update texture
        for (auto&& c : range(size))
        {
            // Cell alias
            const auto& cell = m_lattice[c];
            render::Color color = render::colors::WHITE;

            switch (cell.getDynamics())
            {
            case LatticeCell::Dynamics::BGK:
                color = render::colors::BLACK;
                break;

            case LatticeCell::Dynamics::StaticObstacle:
                color = render::colors::WHITE;
                break;

            case LatticeCell::Dynamics::DynamicObstacle:
                color = render::colors::RED;
                break;

            default:
                break;
            }

            m_drawableObstacles->set(c, color);

            if (cell.hasObstacleDynamics() || cell.hasNoDynamics())
            {
                velocities[c] = Zero;
            }
            else
            {
                // Cell velocity
                velocities[c] = cell.calcVelocityNormalized();
                m_drawableObstacles->set(c, render::Color::fromGray(1e4 * velocities[c].getLength()));
            }
        }
    }

    // Calculate grid max velocity
    const auto maxInlet = *std::max_element(m_inletVelocities.begin(), m_inletVelocities.end());
    const auto maxVel = 1e-7 * LatticeCell::MAX_SPEED * maxInlet.value();

    if (!m_drawableVelocities)
    {
        m_drawableVelocities.create(context, size, velocities.getData(), maxVel);
    }
    else
    {
        m_drawableVelocities->setMax(maxVel);
        m_drawableVelocities->update(velocities.getData());
    }

    // Draw color grid
    context.matrixPush();
    context.matrixScale(simulation.getWorldSize() / units::Length(1));
    m_drawableObstacles->draw(context);
    //m_drawableVelocities->draw(context);
    context.matrixPop();

#if DEV_PLUGIN_streamlines_FORCE_RENDER || DEV_PLUGIN_streamlines_VELOCITY_RENDER
    // Update simulations objects
    for (auto& obj : simulation.getObjects())
    {
        // Ignore static objects
        if (obj->getType() == simulator::Object::Type::Static)
            continue;

#if DEV_PLUGIN_streamlines_FORCE_RENDER
        context.drawLine(
            obj->getPosition() / units::Length(1),
            100 * obj->getForce() / units::Force(1),
            render::colors::YELLOW
        );
#endif

#if DEV_PLUGIN_streamlines_VELOCITY_RENDER
        context.drawLine(
            obj->getPosition() / units::Length(1),
            0.03 * obj->getVelocity() / units::Velocity(1),
            render::colors::BLUE
        );
#endif
    }
#endif

#endif
}
#endif

/* ************************************************************************ */

VelocityVector Module::calculateMaxVelocity(PositionVector dl) const noexcept
{
    return (LatticeCell::MAX_SPEED / calculateViscosity()) * getKinematicViscosity() / dl;
}

/* ************************************************************************ */

void Module::updateObstacleMap(const simulator::Simulation& simulation, const VelocityVector& vMax)
{
    // Clear previous flag
    m_lattice.setDynamics(LatticeCell::Dynamics::BGK);

    const PositionVector start = simulation.getWorldSize() * -0.5f;
    const auto step = simulation.getWorldSize() / m_lattice.getSize();

    // Foreach all cells
    for (auto& obj : simulation.getObjects())
    {
        const bool isDynamic = obj->getType() == simulator::Object::Type::Dynamic;

        // Ignore dynamic objects
        if (!isDynamicObjectsObstacles() && isDynamic)
            continue;

        // Get object position
        const auto pos = obj->getPosition() - start;

        // Check if position is in range
        if (!pos.inRange(Zero, simulation.getWorldSize()))
            continue;

        // Get grid position
        const auto coord = Coordinate(pos / step);

        // Calculate object velocity in LB
        const auto velocity = obj->getVelocity() / vMax;

        // In this case duplicate coordinates doesn't harm and calling
        // operation multiple times on same coordinate is faster than
        // sorting and erasing non-unique coordinates.

        // Map shapes to grid
        for (const auto& shape : obj->getShapes())
        {
            mapShapeToGrid(
                [this, &velocity, isDynamic] (Coordinate&& coord) {
                    Assert(m_lattice.inRange(coord));
                    if (isDynamic)
                        m_lattice[coord].setDynamicObstacleDynamics(velocity);
                    else
                        m_lattice[coord].setStaticObstacleDynamics();
                },
                [] (Coordinate&& coord) {},
                shape, step, coord, obj->getRotation(), m_lattice.getSize()
            );
        }
    }

    m_lattice.fixupObstacles(LatticeCell::Dynamics::StaticObstacle);

    if (isDynamicObjectsObstacles())
        m_lattice.fixupObstacles(LatticeCell::Dynamics::DynamicObstacle);
}

/* ************************************************************************ */

void Module::applyToObjects(const simulator::Simulation& simulation, units::Time dt, const VelocityVector& vMax)
{
    // Foreach objects
    for (auto& obj : simulation.getObjects())
    {
        applyToObject(*obj, simulation, dt, vMax);
    }
}

/* ************************************************************************ */

void Module::applyToObject(simulator::Object& object, const simulator::Simulation& simulation,
    units::Time dt, const VelocityVector& vMax)
{
    // Ignore static objects
    if (object.getType() != simulator::Object::Type::Dynamic)
        return;

    const PositionVector start = simulation.getWorldSize() * -0.5f;
    const auto step = simulation.getWorldSize() / m_lattice.getSize();

    // Get mass local center
    const auto center = object.getMassCenterOffset();

    // Coefficient used in force calculation
    const auto forceCoefficient = 6 * constants::PI * getKinematicViscosity() * object.getDensity();

    auto force = ForceVector{Zero};
    auto velocityObjEnv = VelocityVector{Zero};

    // Map shapes border to grid
    for (const auto& shape : object.getShapes())
    {
        // Only circle shapes are supported
        if (shape.getType() != simulator::ShapeType::Circle)
            continue;

        // Shape alias
        const auto& circle = shape.getCircle();

        // Transform from [-size / 2, size / 2] to [0, size] space
        const auto pos = object.getWorldPosition(circle.center) - start;

        // Check if position is in range
        if (!pos.inRange(Zero, simulation.getWorldSize()))
            continue;

        // Get coordinate to lattice
        const auto coord = Coordinate(pos / step);

        Vector<float> velocityLB = Zero;
        unsigned long count = 0;

        // Store velocity for each coordinate
        mapShapeBorderToGrid(
            [this, &velocityLB, &vMax, &count] (Coordinate&& coord) {
                if (!m_lattice[coord].hasObstacleDynamics())
                {
                    velocityLB += m_lattice[coord].calcVelocity();
                    ++count;
                }
            },
            [] (Coordinate&& coord) { },
            shape, step, coord, m_lattice.getSize(), {}, 1
        );

        if (count == 0)
            continue;

        // Average
        velocityLB /= count;
        const VelocityVector velocityEnv = velocityLB * vMax;
        //const VelocityVector velocityEnv{m_inletVelocities[0], Zero};

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

    // Calculate linear impulse from shapes
    const auto impulse = force * dt;

    // Maximum impulse
    const auto impulseMax = object.getMass() * dv;

    // impulse and impulseMax have same direction but different magnitude
    // In that case we can check only one coordinate
    if (abs(impulse.getX()) >= abs(impulseMax.getX()))
    {
        Assert(abs(impulse.getY()) >= abs(impulseMax.getY()));

        // Apply impulse
        object.applyLinearImpulse(impulseMax);
    }
    else
    {
        Assert(abs(impulse.getY()) <= abs(impulseMax.getY()));

        // Apply impulse
        object.applyLinearImpulse(impulse);
    }
}

/* ************************************************************************ */

void Module::applyBoundaryConditions(const simulator::Simulation& simulation, const VelocityVector& vMax)
{
    // Init boundaries
    for (unsigned int pos = 0; pos < LayoutPosCount; ++pos)
        initBorderInletOutlet(simulation, static_cast<LayoutPosition>(pos), vMax);
}

/* ************************************************************************ */

VelocityVector Module::inletVelocityProfile(
    Lattice::CoordinateType coord, LayoutPosition pos,
    DynamicArray<StaticArray<Lattice::CoordinateType, 2>> inlets
) const noexcept
{
    //return {getInletVelocities()[pos], Zero};

    StaticArray<Lattice::CoordinateType, 2> inletRange{{Zero, Zero}};
    bool found = false;

    switch (pos)
    {
    case LayoutPosTop:
    case LayoutPosBottom:
        // Find inlet range
        for (const auto& inlet : inlets)
        {
            if (inlet[0].getX() <= coord.getX() && coord.getX() <= inlet[1].getX())
            {
                inletRange = inlet;
                found = true;
                break;
            }
        }
        break;

    case LayoutPosLeft:
    case LayoutPosRight:
        // Find inlet range
        for (const auto& inlet : inlets)
        {
            if (inlet[0].getY() <= coord.getY() && coord.getY() <= inlet[1].getY())
            {
                inletRange = inlet;
                found = true;
                break;
            }
        }
        break;

    default:
        Assert(false);
        break;
    }

    if (!found)
        return Zero;

    switch (pos)
    {
    case LayoutPosTop:
        return {
            Zero,
            -calcPoiseuilleFlow(
                getInletVelocities()[pos],
                coord.getX() - inletRange[0].getX(),
                (inletRange[1] - inletRange[0]).getWidth() + 1
            )
        };
        break;

    case LayoutPosBottom:
        return {
            Zero,
            calcPoiseuilleFlow(
                getInletVelocities()[pos],
                coord.getX() - inletRange[0].getX(),
                (inletRange[1] - inletRange[0]).getWidth() + 1
            )
        };
        break;

    case LayoutPosRight:
        return {
            -calcPoiseuilleFlow(
                getInletVelocities()[pos],
                coord.getY() - inletRange[0].getY(),
                (inletRange[1] - inletRange[0]).getHeight() + 1
            ),
            Zero
        };
        break;

    case LayoutPosLeft:
        return {
            calcPoiseuilleFlow(
                getInletVelocities()[pos],
                coord.getY() - inletRange[0].getY(),
                (inletRange[1] - inletRange[0]).getHeight() + 1
            ),
            Zero
        };
        break;

    default:
        Assert(false);
        break;
    }

    return Zero;
}

/* ************************************************************************ */

RealType Module::calculateCoefficient(units::Time dt, PositionVector dl) const noexcept
{
    return calculateViscosity() * (getInnerIterations() * dl.getX() * dl.getY()) / getKinematicViscosity() / dt;
}

/* ************************************************************************ */

void Module::initBorderBarrier(simulator::Simulation& simulation, LayoutPosition pos)
{
    const auto& size = m_lattice.getSize();
    const auto worldSizeHalf = simulation.getWorldSize() * 0.5;

    // Physical size of one lattice cell
    const auto dl = simulation.getWorldSize() / size;

    // Create barrier
    auto obstacle = simulation.createObject<simulator::Obstacle>();
    auto& shapes = obstacle->getMutableShapes();
    shapes.resize(1);

    // Init shape
    shapes[0] = simulator::Shape::makeEdges({
        EDGES[pos][0] * (worldSizeHalf - 0.5 * dl),
        EDGES[pos][1] * (worldSizeHalf - 0.5 * dl)
    });
    obstacle->initShapes();

    // Store obstacle view
    m_layoutBarriers[pos] = obstacle;

    Vector<Lattice::SizeType> rngMin;
    Vector<Lattice::SizeType> rngMax;

    switch (pos)
    {
    case LayoutPosTop:
        rngMin = {0, size.getHeight() - 1};
        rngMax = {size.getWidth(), size.getHeight()};
        break;

    case LayoutPosBottom:
        rngMin = {0, 0};
        rngMax = {size.getWidth(), 1};
        break;

    case LayoutPosRight:
        rngMin = {size.getWidth() - 1, 0};
        rngMax = {size.getWidth(), size.getHeight()};
        break;

    case LayoutPosLeft:
        rngMin = {0, 0};
        rngMax = {1, size.getHeight()};
        break;

    default:
        break;
    }

    // Set obstacles
    for (auto y = rngMin.getY(); y < rngMax.getY(); ++y)
        for (auto x = rngMin.getX(); x < rngMax.getX(); ++x)
            m_lattice[{x, y}].setStaticObstacleDynamics();
}

/* ************************************************************************ */

void Module::initBorderInletOutlet(const simulator::Simulation& simulation,
    LayoutPosition pos, const VelocityVector& vMax)
{
    const auto& size = m_lattice.getSize();

    Vector<Lattice::SizeType> rngMin;
    Vector<Lattice::SizeType> rngMax;
    LatticeCell::Position dir;

    // Detect multiple inlets at the layout position
    DynamicArray<StaticArray<Lattice::CoordinateType, 2>> inlets;

    switch (pos)
    {
    case LayoutPosTop:
        rngMin = {0, size.getHeight() - 1};
        rngMax = {size.getWidth(), size.getHeight()};
        dir = LatticeCell::PositionBottom;

        for (auto x = rngMin.getX(); x < rngMax.getX(); ++x)
        {
            const Lattice::CoordinateType c1 = {x, size.getHeight() - 1};
            if (m_lattice[c1].hasStaticObstacleDynamics())
                continue;

            auto c2 = c1;

            ++x;
            for (; x < rngMax.getX(); ++x)
            {
                const Lattice::CoordinateType cNext = {x, size.getHeight() - 1};
                if (m_lattice[cNext].hasStaticObstacleDynamics())
                    break;

                c2 = cNext;
            }

            if (c1.getX() - c2.getX() > 1)
                inlets.push_back({{c1, c2}});
        }

        break;

    case LayoutPosBottom:
        rngMin = {0, 0};
        rngMax = {size.getWidth(), 1};
        dir = LatticeCell::PositionTop;

        for (auto x = rngMin.getX(); x < rngMax.getX(); ++x)
        {
            const Lattice::CoordinateType c1 = {x, 0};
            if (m_lattice[c1].hasStaticObstacleDynamics())
                continue;

            auto c2 = c1;

            ++x;
            for (; x < rngMax.getX(); ++x)
            {
                const Lattice::CoordinateType cNext = {x, 0};
                if (m_lattice[cNext].hasStaticObstacleDynamics())
                    break;

                c2 = cNext;
            }

            if (c1.getX() - c2.getX() > 1)
                inlets.push_back({{c1, c2}});
        }

        break;

    case LayoutPosRight:
        rngMin = {size.getWidth() - 1, 0};
        rngMax = {size.getWidth(), size.getHeight()};
        dir = LatticeCell::PositionLeft;

        for (auto y = rngMin.getY(); y < rngMax.getY(); ++y)
        {
            const Lattice::CoordinateType c1 = {size.getWidth() - 1, y};
            if (m_lattice[c1].hasStaticObstacleDynamics())
                continue;

            auto c2 = c1;

            ++y;
            for (; y < rngMax.getY(); ++y)
            {
                const Lattice::CoordinateType cNext = {size.getWidth() - 1, y};
                if (m_lattice[cNext].hasStaticObstacleDynamics())
                    break;

                c2 = cNext;
            }

            if (c1.getY() - c2.getY() > 1)
                inlets.push_back({{c1, c2}});
        }

        break;

    case LayoutPosLeft:
        rngMin = {0, 0};
        rngMax = {1, size.getHeight()};
        dir = LatticeCell::PositionRight;

        for (auto y = rngMin.getY(); y < rngMax.getY(); ++y)
        {
            const Lattice::CoordinateType c1 = {0, y};
            if (m_lattice[c1].hasStaticObstacleDynamics())
                continue;

            auto c2 = c1;

            ++y;
            for (; y < rngMax.getY(); ++y)
            {
                const Lattice::CoordinateType cNext = {0, y};
                if (m_lattice[cNext].hasStaticObstacleDynamics())
                    break;

                c2 = cNext;
            }

            if (c1.getY() - c2.getY() > 1)
                inlets.push_back({{c1, c2}});
        }

        break;

    default:
        throw RuntimeException("Invalid layout position");
    }

    if (m_layout[pos] == LayoutType::Outlet)
    {
        for (auto y = rngMin.getY(); y < rngMax.getY(); ++y)
            for (auto x = rngMin.getX(); x < rngMax.getX(); ++x)
                if (!m_lattice[{x, y}].hasObstacleDynamics())
                    m_lattice[{x, y}].initOutlet(dir, 1.0);
    }
    else if (m_layout[pos] == LayoutType::Inlet)
    {
        for (auto y = rngMin.getY(); y < rngMax.getY(); ++y)
        {
            for (auto x = rngMin.getX(); x < rngMax.getX(); ++x)
            {
                if (!m_lattice[{x, y}].hasObstacleDynamics())
                    m_lattice[{x, y}].initInlet(dir, inletVelocityProfile({x, y}, pos, inlets) / vMax);
            }
        }
    }
}

/* ************************************************************************ */

InStream& operator>>(InStream& is, Module::LayoutType& type)
{
    String desc;
    is >> desc;

    if (desc == "none")
        type = Module::LayoutType::None;
    else if (desc == "barrier")
        type = Module::LayoutType::Barrier;
    else if (desc == "inlet")
        type = Module::LayoutType::Inlet;
    else if (desc == "outlet")
        type = Module::LayoutType::Outlet;
    else
        throw InvalidArgumentException("Unknown layout type");

    return is;
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

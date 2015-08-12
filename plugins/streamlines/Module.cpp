/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* Author: Hynek Kasl <hkasl@students.zcu.cz>                               */
/* ************************************************************************ */

// Declaration
#include "Module.hpp"

// C++
#include <algorithm>

// Simulator
#include "core/constants.hpp"
#include "core/DynamicArray.hpp"
#include "core/Exception.hpp"
#include "core/VectorRange.hpp"
#include "core/constants.hpp"
#include "core/TimeMeasurement.hpp"
#include "simulator/Simulation.hpp"
#include "simulator/Object.hpp"
#include "simulator/Obstacle.hpp"
#include "simulator/ShapeToGrid.hpp"

/* ************************************************************************ */

namespace plugin {
namespace streamlines {

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

/**
 * @brief Parse layout.
 *
 * @param str
 *
 * @return
 */
StaticArray<Module::LayoutType, 4> parseLayout(String str)
{
    StaticArray<Module::LayoutType, 4> res;

    InStringStream iss(std::move(str));

    for (std::size_t i = 0; i < res.size(); ++i)
    {
        String desc;
        iss >> desc;

        if (desc == "none")
            res[i] = Module::LayoutType::None;
        else if (desc == "barrier")
            res[i] = Module::LayoutType::Barrier;
        else if (desc == "inlet")
            res[i] = Module::LayoutType::Inlet;
        else if (desc == "outlet")
            res[i] = Module::LayoutType::Outlet;
        else
            throw InvalidArgumentException("Unknown layout type");
    }

    return res;
}

/* ************************************************************************ */

}

/* ************************************************************************ */

Module::Module()
{
    // Nothing to do
}

/* ************************************************************************ */

Module::~Module()
{
    // Nothing to do
}

/* ************************************************************************ */

void Module::setStaticObstacleMap(const ObstacleMap& map)
{
    // Foreach obstacle map
    for (auto&& c : range(map.getSize()))
    {
        if (map[c])
            m_lattice[c].setStaticObstacle(true);
    }
}

/* ************************************************************************ */

void Module::init(simulator::Simulation& simulation)
{
    const auto worldSize = simulation.getWorldSize();
    const auto worldSizeHalf = worldSize * 0.5;

    // Initialize lattice data
    const auto& size = m_lattice.getSize();

    // Create barriers
    if (m_layout[LayoutTop] == LayoutType::Barrier)
    {
        // Create barrier
        auto obstacle = simulation.createObject<simulator::Obstacle>();
        auto& shapes = obstacle->getMutableShapes();
        shapes.resize(1);

        shapes[0] = simulator::Shape::makeEdges({
            {-worldSizeHalf.getX(), worldSizeHalf.getY()},
            { worldSizeHalf.getX(), worldSizeHalf.getY()}
        });

        obstacle->initShapes();

        for (Lattice::SizeType x = 0; x < size.getWidth(); ++x)
            m_lattice[{x, size.getHeight() - 1}].setStaticObstacle(true);
    }

    if (m_layout[LayoutBottom] == LayoutType::Barrier)
    {
        // Create barrier
        auto obstacle = simulation.createObject<simulator::Obstacle>();
        auto& shapes = obstacle->getMutableShapes();
        shapes.resize(1);

        shapes[0] = simulator::Shape::makeEdges({
            {-worldSizeHalf.getX(), -worldSizeHalf.getY()},
            { worldSizeHalf.getX(), -worldSizeHalf.getY()}
        });

        obstacle->initShapes();

        for (Lattice::SizeType x = 0; x < size.getWidth(); ++x)
            m_lattice[{x, 0}].setStaticObstacle(true);
    }

    if (m_layout[LayoutRight] == LayoutType::Barrier)
    {
        // Create barrier
        auto obstacle = simulation.createObject<simulator::Obstacle>();
        auto& shapes = obstacle->getMutableShapes();
        shapes.resize(1);

        shapes[0] = simulator::Shape::makeEdges({
            {-worldSizeHalf.getX(), -worldSizeHalf.getY()},
            {-worldSizeHalf.getX(),  worldSizeHalf.getY()}
        });

        obstacle->initShapes();

        for (Lattice::SizeType y = 0; y < size.getHeight(); ++y)
            m_lattice[{size.getWidth() - 1, y}].setStaticObstacle(true);
    }

    if (m_layout[LayoutLeft] == LayoutType::Barrier)
    {
        // Create barrier
        auto obstacle = simulation.createObject<simulator::Obstacle>();
        auto& shapes = obstacle->getMutableShapes();
        shapes.resize(1);

        shapes[0] = simulator::Shape::makeEdges({
            {worldSizeHalf.getX(), -worldSizeHalf.getY()},
            {worldSizeHalf.getX(),  worldSizeHalf.getY()}
        });

        obstacle->initShapes();

        for (Lattice::SizeType y = 0; y < size.getHeight(); ++y)
            m_lattice[{0, y}].setStaticObstacle(true);
    }
}

/* ************************************************************************ */

void Module::update(units::Duration dt, simulator::Simulation& simulation)
{
    assert(m_lattice.getSize() != Zero);

    auto _ = measure_time("streamlines", simulator::TimeMeasurementIterationOutput(simulation));

    // Physical size of one lattice cell
    const auto dl = simulation.getWorldSize() / m_lattice.getSize();

    // Modified time step
    const auto dtInner = getCoefficient() * dt;

    // Smooth time step
    const auto dtSmooth = dtInner / getIterations();

    // Calculate maximum flow velocity
    const VelocityVector vMax = LatticeData::MAX_SPEED * dl / dtSmooth;

    // Viscosity in LB units
    const auto viscosity =
        (dtSmooth / (dl.getX() * dl.getX())) *
        getKinematicViscosity()
    ;

    // Relaxation parameter
    const auto tau = (3.f * viscosity + 0.5f);
    const auto omega = 1.f / tau;

    // Dynamic obstacles
    updateDynamicObstacleMap(simulation, vMax);

#if THREAD_SAFE
    // Lock access
    MutexGuard guard(m_mutex);
#endif

    // Collide and propagate
    for (auto it = getIterations(); it--; )
    {
        m_lattice.collideAndPropagate(omega);

        // Apply boundary conditions
        applyBoundaryConditions(simulation, vMax);
    }

    // Apply streamlines to world objects
    applyToObjects(simulation, vMax);
}

/* ************************************************************************ */

void Module::configure(const simulator::Configuration& config, simulator::Simulation& simulation)
{
    // Number of inner iterations
    setIterations(config.get("iterations", getIterations()));

    if (getIterations() == 0)
        throw InvalidArgumentException("Number of inner iterations cannot be zero");

    // Convert coefficient
    setCoefficient(config.get("coefficient", getCoefficient()));

    if (getCoefficient() == 0)
        throw InvalidArgumentException("Coefficient cannot be zero");

    // Inflow velocity
    setVelocityInflow(config.get("velocity-inflow", getVelocityInflow()));

    // Viscosity
    setKinematicViscosity(config.get("kinematic-viscosity", getKinematicViscosity()));

    if (getKinematicViscosity() == Zero)
        throw InvalidArgumentException("Kinematic viscosity cannot be zero");

    // Grid size
    m_lattice.setSize(config.get<Vector<Lattice::SizeType>>("grid"));

    if (m_lattice.getSize() == Zero)
        throw InvalidArgumentException("Lattice size cannot be zero");

    // Parse layout
    if (config.has("layout"))
        m_layout = parseLayout(config.get("layout"));
    else
        m_layout = {LayoutType::Barrier, LayoutType::Outlet, LayoutType::Barrier, LayoutType::Inlet};

    // Initialize lattice
    init(simulation);
}

/* ************************************************************************ */

#ifdef ENABLE_RENDER
void Module::draw(render::Context& context, const simulator::Simulation& simulation)
{
#if DEV_DRAW_VELOCITY
    const auto size = m_lattice.getSize();

    if (!m_drawable)
        m_drawable.create(context, size);

    // Temporary for velocities
    Grid<Vector<LatticeData::ValueType>> velocities(size);

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

            // Background color
            render::Color color = render::colors::RED;

            if (!cell.isObstacle())
            {
                // Calculate velocity vector
                const auto velocity = cell.calcVelocity();

                // Cell color
                color = render::Color::fromGray(50 * LatticeData::MAX_SPEED * velocity.getLength());

                // Cell velocity
                velocities[c] = velocity;
            }
            else
            {
                velocities[c] = Zero;
            }

            // Set color
            m_drawable->set(c, color);
        }
    }

    if (!m_drawableVector)
        m_drawableVector.create(context, size, velocities.getData(), 0.05 * getCoefficient());
    else
        m_drawableVector->update(velocities.getData());

    // Draw color grid
    context.matrixPush();
    context.matrixScale(simulation.getWorldSize() / units::Length(1));
    m_drawable->draw(context);
    m_drawableVector->draw(context);
    context.matrixPop();

    // Update simulations objects
    for (auto& obj : simulation.getObjects())
    {
        // Ignore static objects
        if (obj->getType() == simulator::Object::Type::Static)
            continue;

        context.drawLine(
            obj->getPosition() / units::Length(1),
            10000 * obj->getForce() / units::Force(1),
            render::colors::YELLOW
        );

        context.drawLine(
            obj->getPosition() / units::Length(1),
            0.03 * obj->getVelocity() / units::Velocity(1),
            render::colors::BLUE
        );
    }
#endif
}
#endif

/* ************************************************************************ */

void Module::updateDynamicObstacleMap(const simulator::Simulation& simulation, const VelocityVector& vMax)
{
    // Clear previous flag
    m_lattice.clearDynamicObstacles();

    const PositionVector start = simulation.getWorldSize() * -0.5f;
    const auto step = simulation.getWorldSize() / m_lattice.getSize();

    // Foreach all cells
    for (auto& obj : simulation.getObjects())
    {
        // Ignore static objects
        //if (obj->getType() != simulator::Object::Type::Static)
        //    continue;

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
                [this, &velocity] (Coordinate&& coord) {
                    assert(m_lattice.inRange(coord));
                    m_lattice[coord].setDynamicObstacle(true, velocity);
                },
                [] (Coordinate&& coord) {},
                shape, step, coord, obj->getRotation(), m_lattice.getSize()
            );
        }
    }
}

/* ************************************************************************ */

void Module::applyToObjects(const simulator::Simulation& simulation, const VelocityVector& vMax)
{
    const PositionVector start = simulation.getWorldSize() * -0.5f;
    const auto step = simulation.getWorldSize() / m_lattice.getSize();

    // Foreach objects
    for (auto& obj : simulation.getObjects())
    {
        // Ignore static objects
        if (obj->getType() == simulator::Object::Type::Static)
            continue;

        // Transform from [-size / 2, size / 2] to [0, size] space
        const auto pos = obj->getPosition() - start;

        // Check if position is in range
        if (!pos.inRange(Zero, simulation.getWorldSize()))
            continue;

        // Get coordinate to lattice
        const auto coord = Coordinate(pos / step);

        // Map shapes border to grid
        for (const auto& shape : obj->getShapes())
        {
            VelocityVector velocity = Zero;
            unsigned long count = 0;

            // Store velocity for each coordinate
            mapShapeBorderToGrid(
                [this, &velocity, &vMax, &count] (Coordinate&& coord) {
                    if (!m_lattice[coord].isObstacle())
                    {
                        velocity += m_lattice[coord].calcVelocity() * vMax;
                        ++count;
                    }
                },
                [this, &velocity, &count] (Coordinate&& coord) {
                    // TODO: based on layout
                    velocity += inletVelocityProfileX(coord);
                    ++count;
                },
                shape, step, coord, m_lattice.getSize(), {}, 2
            );

            assert(count);

            // Average
            velocity /= count;

            // Difference between velocities
            const auto dv = velocity - obj->getVelocity();

            //obj->setVelocity(velocity / getCoefficient());

            // Cell radius
            const auto radius = shape.getCircle().radius;

            // Set object velocity
            const auto force =
                3 * constants::PI *
                getKinematicViscosity() *
                obj->getDensity() *
                dv *
                radius /
                units::LENGTH_COEFFICIENT
            ;

            // Apply force
            obj->applyForce(force / getCoefficient(), obj->getPosition() + shape.getCircle().center);
        }
    }
}

/* ************************************************************************ */

void Module::applyBoundaryConditions(const simulator::Simulation& simulation, const VelocityVector& vMax)
{
    // maximum velocity of the Poiseuille inflow
    const auto size = m_lattice.getSize();
    // Velocity in LB
    const auto velocity = getVelocityInflow();

    if (m_layout[LayoutTop] == LayoutType::Outlet)
    {
        for (Lattice::SizeType x = 0; x < size.getWidth(); ++x)
            if (!m_lattice[{x, size.getHeight() - 1}].isObstacle())
                m_lattice[{x, size.getHeight() - 1}].outlet();
    }
    else if (m_layout[LayoutTop] == LayoutType::Inlet)
    {
        for (Lattice::SizeType x = 0; x < size.getWidth(); ++x)
            if (!m_lattice[{x, size.getHeight() - 1}].isObstacle())
                m_lattice[{x, size.getHeight() - 1}].inlet(-1 * inletVelocityProfileY({x, size.getHeight() - 1}) / vMax);
    }

    if (m_layout[LayoutBottom] == LayoutType::Outlet)
    {
        for (Lattice::SizeType x = 0; x < size.getWidth(); ++x)
            if (!m_lattice[{x, 0}].isObstacle())
                m_lattice[{x, 0}].outlet();
    }
    else if (m_layout[LayoutBottom] == LayoutType::Inlet)
    {
        for (Lattice::SizeType x = 0; x < size.getWidth(); ++x)
            if (!m_lattice[{x, 0}].isObstacle())
                m_lattice[{x, 0}].inlet(inletVelocityProfileY({x, 0}) / vMax);
    }

    if (m_layout[LayoutRight] == LayoutType::Outlet)
    {
        for (Lattice::SizeType y = 0; y < size.getHeight(); ++y)
            if (!m_lattice[{size.getWidth() - 1, y}].isObstacle())
                m_lattice[{size.getWidth() - 1, y}].outlet();
    }
    else if (m_layout[LayoutRight] == LayoutType::Inlet)
    {
        for (Lattice::SizeType y = 0; y < size.getHeight(); ++y)
            if (!m_lattice[{size.getWidth() - 1, y}].isObstacle())
                m_lattice[{size.getWidth() - 1, y}].inlet(-1 * inletVelocityProfileX({size.getWidth() - 1, y}) / vMax);
    }

    if (m_layout[LayoutLeft] == LayoutType::Outlet)
    {
        for (Lattice::SizeType y = 0; y < size.getHeight(); ++y)
            if (!m_lattice[{0, y}].isObstacle())
                m_lattice[{0, y}].outlet();
    }
    else if (m_layout[LayoutLeft] == LayoutType::Inlet)
    {
        for (Lattice::SizeType y = 0; y < size.getHeight(); ++y)
            if (!m_lattice[{0, y}].isObstacle())
                m_lattice[{0, y}].inlet(inletVelocityProfileX({0, y}) / vMax);
    }
#if 0
    // Left & Right active only if velocity has X-coordinate speed.
    if (velocity.getX() != Zero)
    {
        for (Lattice::SizeType y = 0; y < size.getHeight(); ++y)
        {
            Lattice::CoordinateType in;
            Lattice::CoordinateType out;
            Lattice::CoordinateType outPrev;

            if (velocity.getX() < Zero)
            {
                in = {size.getWidth() - 1, y};
                out = {0, y};
                outPrev = {1, y};
            }
            else if (velocity.getX() > Zero)
            {
                in = {0, y};
                out = {size.getWidth() - 1, y};
                outPrev = {out.getX() - 1, y};
            }
            else
            {
                continue;
            }

            // Input
            if (!m_lattice[in].isStaticObstacle())
                m_lattice[in].inlet(inletVelocityProfile(in) / vMax);

            // Outlet
            if (!m_lattice[out].isStaticObstacle())
                m_lattice[out].outlet();
        }
/*
        for (Lattice::SizeType x = 0; x < size.getWidth(); ++x)
        {
            m_lattice[{x, 0}] = m_lattice[{x, 1}].getValues();
            m_lattice[{x, size.getHeight() - 1}] = m_lattice[{x, size.getHeight() - 2}].getValues();
        }
*/
    }
#endif
}

/* ************************************************************************ */

VelocityVector Module::inletVelocityProfileX(Lattice::CoordinateType coord) const noexcept
{
    //return {getVelocityInflow().getX(), Zero};

    // maximum velocity of the Poiseuille inflow
    const auto size = m_lattice.getSize();

    const RealType y = static_cast<RealType>(coord.getY() - 1);
    const RealType l = static_cast<RealType>(size.getHeight() - 1);
    return {4.f * getVelocityInflow().getX() / (l * l) * (l * y - y * y), Zero};
}

/* ************************************************************************ */

VelocityVector Module::inletVelocityProfileY(Lattice::CoordinateType coord) const noexcept
{
    //return {Zero, getVelocityInflow().getY()};

    // maximum velocity of the Poiseuille inflow
    const auto size = m_lattice.getSize();

    const RealType y = static_cast<RealType>(coord.getX() - 1);
    const RealType l = static_cast<RealType>(size.getWidth() - 1);
    return {Zero, 4.f * getVelocityInflow().getY() / (l * l) * (l * y - y * y)};
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

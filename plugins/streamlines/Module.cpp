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
#include "core/StaticArray.hpp"
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

/**
 * @brief Read layout type from stream.
 *
 * @param is   Input stream.
 * @param type Output type.
 *
 * @return is.
 */
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
    return is >> std::skipws >> layout.top >> layout.right >> layout.bottom >> layout.left;
}

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

/**
 * @brief Initialize barrier.
 *
 * @param simulation
 * @param lattice
 * @param rng
 * @param edges
 */
void initBarrier(
    simulator::Simulation& simulation, Lattice& lattice,
    Vector<Lattice::SizeType> rngMin, Vector<Lattice::SizeType> rngMax,
    Vector<int> edges1, Vector<int> edges2
)
{
    const auto worldSizeHalf = simulation.getWorldSize() * 0.5;

    // Create barrier
    auto obstacle = simulation.createObject<simulator::Obstacle>();
    auto& shapes = obstacle->getMutableShapes();
    shapes.resize(1);

    // Init shape
    shapes[0] = simulator::Shape::makeEdges({edges1 * worldSizeHalf, edges2 * worldSizeHalf});
    obstacle->initShapes();

    // Set obstacles
    for (auto y = rngMin.getY(); y < rngMax.getY(); ++y)
        for (auto x = rngMin.getX(); x < rngMax.getX(); ++x)
            lattice[{x, y}].setStaticObstacle(true);
}

/* ************************************************************************ */

/**
 * @brief Init boundary condition.
 *
 * @param type
 * @param lattice
 * @param rngMin
 * @param rngMax
 */
template<typename VelFn>
void initBoundary(
    Module::LayoutType type, Lattice& lattice,
    Vector<Lattice::SizeType> rngMin, Vector<Lattice::SizeType> rngMax,
    VelFn velFn
)
{
    if (type == Module::LayoutType::Outlet)
    {
        for (auto y = rngMin.getY(); y < rngMax.getY(); ++y)
            for (auto x = rngMin.getX(); x < rngMax.getX(); ++x)
                if (!lattice[{x, y}].isObstacle())
                    lattice[{x, y}].outlet();
    }
    else if (type == Module::LayoutType::Inlet)
    {
        for (auto y = rngMin.getY(); y < rngMax.getY(); ++y)
            for (auto x = rngMin.getX(); x < rngMax.getX(); ++x)
                if (!lattice[{x, y}].isObstacle())
                    lattice[{x, y}].inlet(velFn({x, y}));
    }
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
    // Initialize lattice data
    const auto& size = m_lattice.getSize();

    // Physical size of one lattice cell
    const auto dl = simulation.getWorldSize() / size;

    // Create top barrier
    if (m_layout.top == LayoutType::Barrier)
    {
        initBarrier(
            simulation, m_lattice,
            {0, size.getHeight() - 1}, {size.getWidth(), size.getHeight()},
            {-1, 1}, {1, 1}
        );
    }

    // Create bottom barrier
    if (m_layout.bottom == LayoutType::Barrier)
    {
        initBarrier(
            simulation, m_lattice,
            {0, 0}, {size.getWidth(), 1},
            {-1, -1}, {1, -1}
        );
    }

    // Create right barrier
    if (m_layout.right == LayoutType::Barrier)
    {
        initBarrier(
            simulation, m_lattice,
            {size.getWidth() - 1, 0}, {size.getWidth(), size.getHeight()},
            {-1, -1}, {-1, 1}
        );
    }

    // Create left barrier
    if (m_layout.left == LayoutType::Barrier)
    {
        initBarrier(
            simulation, m_lattice,
            {0, 0}, {1, size.getHeight()},
            {1, -1}, {1, 1}
        );
    }

    // Calculate values
    const auto vMax = calculateMaxVelocity(dl);
    const auto omega = 1.0 / getTau();

    // Initialization iterations
    for (auto it = getInitIterations(); it--; )
    {
        m_lattice.collideAndPropagate(omega);

        // Apply boundary conditions
        applyBoundaryConditions(simulation, vMax);
    }

}

/* ************************************************************************ */

void Module::update(units::Duration dt, simulator::Simulation& simulation)
{
    assert(m_lattice.getSize() != Zero);

    auto _ = measure_time("streamlines", simulator::TimeMeasurementIterationOutput(simulation));

    // Physical size of one lattice cell
    const auto dl = simulation.getWorldSize() / m_lattice.getSize();

    // Calculate maximum flow velocity
    const auto vMax = calculateMaxVelocity(dl);

    // Relaxation parameter
    const auto omega = 1.0 / getTau();

    // Calculate conversion coefficient
    setCoefficient(calculateCoefficient(dt, dl));

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
    // Number of init iterations
    setInitIterations(config.get("init-iterations", getInitIterations()));

    // Number of inner iterations
    setIterations(config.get("iterations", getIterations()));

    if (getIterations() == 0)
        throw InvalidArgumentException("Number of inner iterations cannot be zero");

    // Convert relaxation time
    setTau(config.get("tau", getTau()));

    if (getTau() == 0)
        throw InvalidArgumentException("Relaxation time cannot be zero");

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

    // Layout
    setLayout(config.get("layout", getLayout()));

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

    // Init boundaries
    initBoundary(m_layout.top, m_lattice, {0, size.getHeight() - 1}, {size.getWidth(), size.getHeight()}, [&](Lattice::CoordinateType c) {
        return -1 * inletVelocityProfileY({c.getX(), size.getHeight() - 1}) / vMax;
    });
    initBoundary(m_layout.right, m_lattice, {size.getWidth() - 1, 0}, {size.getWidth(), size.getHeight()}, [&](Lattice::CoordinateType c) {
        return -1 * inletVelocityProfileX({size.getWidth() - 1, c.getY()}) / vMax;
    });
    initBoundary(m_layout.bottom, m_lattice, {0, 0}, {size.getWidth(), 1}, [&](Lattice::CoordinateType c) {
        return inletVelocityProfileY({c.getX(), 0}) / vMax;
    });
    initBoundary(m_layout.left, m_lattice, {0, 0}, {1, size.getHeight()}, [&](Lattice::CoordinateType c) {
        return inletVelocityProfileX({0, c.getY()}) / vMax;
    });
}

/* ************************************************************************ */

VelocityVector Module::inletVelocityProfileX(Lattice::CoordinateType coord) const noexcept
{
    return {getVelocityInflow().getX(), Zero};

    // maximum velocity of the Poiseuille inflow
    const auto size = m_lattice.getSize();

    const RealType y = static_cast<RealType>(coord.getY() - 1);
    const RealType l = static_cast<RealType>(size.getHeight() - 1);
    return {4.f * getVelocityInflow().getX() / (l * l) * (l * y - y * y), Zero};
}

/* ************************************************************************ */

VelocityVector Module::inletVelocityProfileY(Lattice::CoordinateType coord) const noexcept
{
    return {Zero, getVelocityInflow().getY()};

    // maximum velocity of the Poiseuille inflow
    const auto size = m_lattice.getSize();

    const RealType y = static_cast<RealType>(coord.getX() - 1);
    const RealType l = static_cast<RealType>(size.getWidth() - 1);
    return {Zero, 4.f * getVelocityInflow().getY() / (l * l) * (l * y - y * y)};
}

/* ************************************************************************ */

RealType Module::calculateCoefficient(units::Time step, PositionVector dl) const noexcept
{
    const auto viscosity = (getTau() - 0.5) / 3.0;
    return viscosity * (getIterations() * dl.getX() * dl.getY()) / getKinematicViscosity() / step;
}

/* ************************************************************************ */

VelocityVector Module::calculateMaxVelocity(PositionVector dl) const noexcept
{
    const auto viscosity = (getTau() - 0.5) / 3.0;
    return (LatticeData::MAX_SPEED / viscosity) * VelocityVector{
        getKinematicViscosity() / dl.getX(),
        getKinematicViscosity() / dl.getY()
    };
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

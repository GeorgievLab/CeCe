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
    const RealType posF = static_cast<RealType>(pos) - 1.0;
    const RealType sizeF = static_cast<RealType>(size) - 1.0;
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
    initBarriers(simulation);

    // Physical size of one lattice cell
    const auto dl = simulation.getWorldSize() / m_lattice.getSize();

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
            // Only circle shapes are supported
            if (shape.getType() != simulator::ShapeType::Circle)
                continue;

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
                    //velocity += inletVelocityProfileX(coord);
                    //++count;
                },
                shape, step, coord, m_lattice.getSize(), {}, 2
            );

            assert(count);

            // Average
            velocity /= count;

            // Difference between velocities
            const auto dv = velocity - obj->getVelocity();

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
    // Init boundaries
    for (unsigned int pos = 0; pos < LayoutPosCount; ++pos)
        initBorderInletOutlet(simulation, static_cast<LayoutPosition>(pos), vMax);
}

/* ************************************************************************ */

VelocityVector Module::inletVelocityProfile(Lattice::CoordinateType coord, LayoutPosition pos) const noexcept
{
    const auto size = m_lattice.getSize();

    //return {getVelocityInflow().getX(), Zero};

    switch (pos)
    {
    case LayoutPosTop:
        return {
            Zero,
            -calcPoiseuilleFlow(getInletVelocities()[pos], coord.getX(), size.getWidth())
        };
        break;

    case LayoutPosBottom:
        return {
            Zero,
            calcPoiseuilleFlow(getInletVelocities()[pos], coord.getX(), size.getWidth())
        };
        break;

    case LayoutPosRight:
        return {
            -calcPoiseuilleFlow(getInletVelocities()[pos], coord.getY(), size.getHeight()),
            Zero
        };
        break;

    case LayoutPosLeft:
        return {
            calcPoiseuilleFlow(getInletVelocities()[pos], coord.getY(), size.getHeight()),
            Zero
        };
        break;

    default:
        break;
    }

    return Zero;
}

/* ************************************************************************ */

RealType Module::calculateCoefficient(units::Time step, PositionVector dl) const noexcept
{
    return calculateViscosity() * (getIterations() * dl.getX() * dl.getY()) / getKinematicViscosity() / step;
}

/* ************************************************************************ */

VelocityVector Module::calculateMaxVelocity(PositionVector dl) const noexcept
{
    return (LatticeData::MAX_SPEED / calculateViscosity()) * getKinematicViscosity() / dl;
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
            m_lattice[{x, y}].setStaticObstacle(true);
}

/* ************************************************************************ */

void Module::initBorderInletOutlet(const simulator::Simulation& simulation,
    LayoutPosition pos, const VelocityVector& vMax)
{
    const auto& size = m_lattice.getSize();

    Vector<Lattice::SizeType> rngMin;
    Vector<Lattice::SizeType> rngMax;
    LatticeData::Direction dir;

    switch (pos)
    {
    case LayoutPosTop:
        rngMin = {0, size.getHeight() - 1};
        rngMax = {size.getWidth(), size.getHeight()};
        dir = LatticeData::DirDown;
        break;

    case LayoutPosBottom:
        rngMin = {0, 0};
        rngMax = {size.getWidth(), 1};
        dir = LatticeData::DirUp;
        break;

    case LayoutPosRight:
        rngMin = {size.getWidth() - 1, 0};
        rngMax = {size.getWidth(), size.getHeight()};
        dir = LatticeData::DirLeft;
        break;

    case LayoutPosLeft:
        rngMin = {0, 0};
        rngMax = {1, size.getHeight()};
        dir = LatticeData::DirRight;
        break;

    default:
        break;
    }

    if (m_layout[pos] == LayoutType::Outlet)
    {
        for (auto y = rngMin.getY(); y < rngMax.getY(); ++y)
            for (auto x = rngMin.getX(); x < rngMax.getX(); ++x)
                if (!m_lattice[{x, y}].isObstacle())
                    m_lattice[{x, y}].outlet(dir);
    }
    else if (m_layout[pos] == LayoutType::Inlet)
    {
        for (auto y = rngMin.getY(); y < rngMax.getY(); ++y)
            for (auto x = rngMin.getX(); x < rngMax.getX(); ++x)
                if (!m_lattice[{x, y}].isObstacle())
                    m_lattice[{x, y}].inlet(inletVelocityProfile({x, y}, pos) / vMax, dir);
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

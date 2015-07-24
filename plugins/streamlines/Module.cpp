/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* ************************************************************************ */

// Declaration
#include "Module.hpp"

// C++
#include <algorithm>

// Simulator
#include "core/constants.hpp"
#include "core/DynamicArray.hpp"
#include "core/Exception.hpp"
#include "simulator/Simulation.hpp"
#include "simulator/Object.hpp"
#include "simulator/ShapeToGrid.hpp"

/* ************************************************************************ */

namespace plugin {
namespace streamlines {

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

void Module::init()
{
    // Initialize lattice data
    const auto& size = m_lattice.getSize();

    // Set border obstacles
    // TODO: replace by physical obstacles
    for (Lattice::SizeType x = 1; x < size.getWidth() - 1; ++x)
    {
        m_lattice[{x, 1}].setStaticObstacle(true);
        m_lattice[{x, size.getHeight() - 2}].setStaticObstacle(true);
    }
}

/* ************************************************************************ */

void Module::update(units::Duration dt, simulator::Simulation& simulation)
{
    // Physical size of one lattice cell
    const auto dl = simulation.getWorldSize() / m_lattice.getSize();

    // Calculate maximum flow velocity
    const VelocityVector v_max = LatticeData::MAX_SPEED * dl / dt;

    // Dynamic obstacles
    updateDynamicObstacleMap(simulation, v_max);

    // Viscosity in LB units
    const auto viscosity = (dt / (dl.getX() * dl.getX())) * getKinematicViscosity();

    // Relaxation parameter
    const float tau = (3.f * viscosity + 0.5f);
    const float omega = 1.f / tau;

    // Collide and propagate
    m_lattice.collideAndPropagate(omega);

    // Apply boundary conditions
    applyBoundaryConditions(simulation, v_max);

    // Apply streamlines to world objects
    applyToObjects(simulation, v_max);
}

/* ************************************************************************ */

void Module::configure(const simulator::Configuration& config, simulator::Simulation& simulation)
{
    // Inflow velocity
    setVelocityInflow(config.get("velocity-inflow", getVelocityInflow()));

    // Viscosity
    setKinematicViscosity(config.get("kinematic-viscosity", getKinematicViscosity()));

    // Grid size
    m_lattice.setSize(config.get<Vector<Lattice::SizeType>>("grid"));

    // Initialize lattice
    init();
}

/* ************************************************************************ */

#ifdef ENABLE_RENDER
void Module::draw(render::Context& context, const simulator::Simulation& simulation)
{
#if OPT_DRAW_VELOCITY
    const auto size = m_lattice.getSize();

    if (!m_drawable)
        m_drawable.create(context, size);

    // Temporary for velocities
    // TODO: use Lattice velocity units
    Grid<Vector<float>> velocities(size);

    // Update texture
    for (decltype(size.getHeight()) y = 0; y < size.getHeight(); ++y)
    {
        for (decltype(size.getWidth()) x = 0; x < size.getWidth(); ++x)
        {
            // Cell alias
            const auto& cell = m_lattice[{x + 1, y + 1}];
            const Coordinate coord{x, y};

            // Background color
            //render::Color color = {1, 1, 1, 1};
            render::Color color = {0, 0, 0, 1};

            if (!cell.isObstacle())
            {
                // Calculate velocity vector
                const auto velocity = cell.calcVelocityNormalized();

                // Cell color
                color = {velocity.getLength(), velocity.getLength(), velocity.getLength(), 1};

                // Cell velocity
                velocities[coord] = velocity;
            }
            else
            {
                velocities[coord] = Zero;
            }

            // Set color
            m_drawable->set(coord, color);
        }
    }

    if (!m_drawableVector)
        m_drawableVector.create(context, size, velocities.getData());
    else
        m_drawableVector->update(velocities.getData());

    // Draw color grid
    context.matrixPush();
    context.matrixScale(simulation.getWorldSize() / units::Length(1));
    m_drawable->draw(context);
    m_drawableVector->draw(context);
    context.matrixPop();
#endif
}
#endif

/* ************************************************************************ */

void Module::updateDynamicObstacleMap(const simulator::Simulation& simulation, const VelocityVector& v_max)
{
    // Clear previous flag
    m_lattice.clearDynamicObstacles();

    const PositionVector start = simulation.getWorldSize() * -0.5f;
    const auto step = simulation.getWorldSize() / m_lattice.getSize();

    // Foreach all cells
    for (auto& obj : simulation.getObjects())
    {
        // Get object position
        const auto pos = obj->getPosition() - start;

        // Check if position is in range
        if (!pos.inRange(Zero, simulation.getWorldSize()))
            continue;

        // Get grid position
        const auto coord = Coordinate(pos / step);

        // Calculate object velocity in LB
        const auto velocity = obj->getVelocity() / v_max;

        // In this case duplicate coordinates doesn't harm and calling
        // operation multiple times on same coordinate is faster than
        // sorting and erasing non-unique coordinates.

        // Map shapes to grid
        for (const auto& shape : obj->getShapes())
        {
            mapShapeToGrid(
                [this, &velocity] (Coordinate&& coord) {
                    assert(m_lattice.inRange(coord + 1));
                    m_lattice[coord + 1].setDynamicObstacle(true, velocity);
                },
                [] (Coordinate&& coord) {},
                shape, step, coord, obj->getRotation(), m_lattice.getSize()
            );
        }
    }
}

/* ************************************************************************ */

void Module::applyToObjects(const simulator::Simulation& simulation, const VelocityVector& v_max)
{
    const PositionVector start = simulation.getWorldSize() * -0.5f;
    const auto step = simulation.getWorldSize() / m_lattice.getSize();

    // Velocity profile
    const auto size = m_lattice.getRealSize();
    const auto velocity = getVelocityInflow();

    auto computePoiseuille = [&size, velocity](int i) {
        const float y = (float) (i - 1);
        const float L = (float) (size.getHeight() - 1);
        return 4.f * velocity.getX() / (L * L) * (L * y - y * y);
    };

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
                [this, &velocity, &v_max, &count] (Coordinate&& coord) {
                    velocity += m_lattice[coord + 1].calcVelocityNormalized() * v_max;
                    ++count;
                },
                [this, &velocity, &count, &computePoiseuille] (Coordinate&& coord) {
                    velocity += VelocityVector{computePoiseuille(coord.getY()), Zero};
                    ++count;
                },
                shape, step, coord, m_lattice.getSize(), {}, 5
            );

            assert(count);

            // Average
            velocity /= count;

            // Difference between velocities
            const auto dv = velocity - obj->getVelocity();

            // Set object velocity
            const auto force = 3 * constants::PI * getKinematicViscosity() * obj->getDensity() * dv * shape.circle.radius;

            // Apply force
            obj->applyForce(force, obj->getPosition() + shape.circle.center);
        }
    }
}

/* ************************************************************************ */

void Module::applyBoundaryConditions(const simulator::Simulation& simulation, const VelocityVector& v_max)
{
    // maximum velocity of the Poiseuille inflow
    const auto size = m_lattice.getRealSize();
    // Velocity in LB
    const auto velocity = getVelocityInflow() / v_max;

    auto computePoiseuille = [&size, velocity](int i) {
        const float y = (float) (i - 1);
        const float L = (float) (size.getHeight() - 1);
        return 4.f * velocity.getX() / (L * L) * (L * y - y * y);
    };

    // Left & Right active only if velocity has X-coordinate speed.
    if (velocity.getX() != 0)
    {
        for (Lattice::SizeType y = 0; y < size.getHeight(); ++y)
        {
            Lattice::CoordinateType in;
            Lattice::CoordinateType out;
            Lattice::CoordinateType outPrev;

            if (velocity.getX() < 0)
            {
                in = {size.getWidth() - 2, y};
                out = {1, y};
                outPrev = {2, y};
            }
            else if (velocity.getX() > 0)
            {
                in = {1, y};
                out = {size.getWidth() - 2, y};
                outPrev = {out.getX() - 1, y};
            }
            else
            {
                continue;
            }

            // Input
            //m_lattice[{in, y}].init(velocity);
            m_lattice[in].init({computePoiseuille(y), 0});

            // Output
            m_lattice[out] = m_lattice[outPrev];
            //m_lattice[{grid_size.getWidth() - 1, y}].clear();
        }
    }
/*
    if (velocity.getY() != 0)
    {
        for (Lattice::SizeType x = 0; x < size.getWidth(); ++x)
        {
            Lattice::CoordinateType::value_type in;
            Lattice::CoordinateType::value_type out;
            Lattice::CoordinateType::value_type outPrev;

            if (velocity.getY() < 0)
            {
                in = size.getHeight() - 2;
                out = 1;
                outPrev = 2;
            }
            else if (velocity.getY() > 0)
            {
                in = 1;
                out = size.getHeight() - 2;
                outPrev = out - 1;
            }

            // Input
            m_lattice[{x, in}].init(velocity);

            // Output
            m_lattice[{x, out}] = m_lattice[{x, outPrev}];
            //m_lattice[{grid_size.getWidth() - 1, y}].clear();
        }
    }
*/
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

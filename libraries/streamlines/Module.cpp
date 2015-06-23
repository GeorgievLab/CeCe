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
#include "parser/Parser.hpp"
#include "simulator/Simulation.hpp"
#include "simulator/Object.hpp"
#include "simulator/ShapeToGrid.hpp"

/* ************************************************************************ */

namespace module {
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
    const auto viscosity = (dt / (dl.getX() * dl.getX())) * getViscosity();

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
    // Maximum velocity
    config.callIfSetString("velocity-inflow", [this](const String& value) {
        setVelocityInflow(parser::parse_value<units::Velocity>(value));
    });

    // Viscosity
    config.callIfSetString("viscosity", [this](const String& value) {
        setViscosity(parser::parse_value<units::Viscosity>(value));
    });

    // Grid size
    config.callString("grid", [this](const String& value) {
        m_lattice.setSize(parser::parse_vector_single<Lattice::SizeType>(value));
    });

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
    Grid<VelocityVector> velocities(size);

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
                velocities[coord] = VelocityVector::Zero;
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
    context.matrixScale(simulation.getWorldSize());
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

    DynamicArray<Coordinate> coords;
    coords.reserve(256);

    // Foreach all cells
    for (auto& obj : simulation.getObjects())
    {
        // Get object position
        const auto pos = obj->getPosition() - start;

        // Check if position is in range
        if (!pos.inRange(PositionVector::Zero, simulation.getWorldSize()))
            continue;

        // Get grid position
        const auto coord = Coordinate(pos / step);

        coords.clear();
        const auto& shapes = obj->getShapes();
        auto coordIt = std::back_inserter(coords);

        // TODO: change to something that doesn't require container
        // function mapShapeToGrid should call function that does
        // required operation

        // Map shapes to grid
        for (const auto& shape : shapes)
        {
            coordIt = mapShapeToGrid(coordIt, shape, step, coord, m_lattice.getSize());
        }

        // In this case duplicate coordinates doesn't harm and calling
        // operation multiple times on same coordinate is faster than
        // sorting and erasing non-unique coordinates.

        // Calculate object velocity in LB
        const auto velocity = obj->getVelocity() / v_max;

        for (const auto& c : coords)
        {
            assert(m_lattice.inRange(c));
            m_lattice[c].setDynamicObstacle(true, velocity);
        }
    }
}

/* ************************************************************************ */

void Module::applyToObjects(const simulator::Simulation& simulation, const VelocityVector& v_max)
{
    const PositionVector start = simulation.getWorldSize() * -0.5f;
    const auto step = simulation.getWorldSize() / m_lattice.getSize();

    DynamicArray<Coordinate> coords;
    coords.reserve(256);

    // Foreach objects
    for (auto& obj : simulation.getObjects())
    {
        // Ignore static objects
        if (obj->getType() == simulator::Object::Type::Static)
            continue;

        // Transform from [-size / 2, size / 2] to [0, size] space
        const auto pos = obj->getPosition() - start;

        // Check if position is in range
        if (!pos.inRange(PositionVector::Zero, simulation.getWorldSize()))
            continue;

        // Get coordinate to lattice
        const auto coord = Coordinate(pos / step);

        // Map shapes border to grid
        for (const auto& shape : obj->getShapes())
        {
            coords.clear();
            auto coordIt = std::back_inserter(coords);
            coordIt = mapShapeBorderToGrid(coordIt, shape, step, coord, m_lattice.getSize(), {}, 5);

            // NOTE: Function should return only unique coordinates.

            // Sum of the velocities
            VelocityVector velocity = std::accumulate(coords.begin(), coords.end(),
                VelocityVector{VelocityVector::Zero},
                [&v_max, this](VelocityVector& init, const Coordinate& c) {
                    return init + m_lattice[c].calcVelocityNormalized() * v_max;
                }
            );

            assert(!coords.empty());

            // Average
            velocity = velocity / coords.size();

            // Difference between velocities
            const auto dv = velocity - obj->getVelocity();

            // Set object velocity
            const auto force = 3 * constants::PI * getViscosity() * dv * shape.circle.radius;

            // Apply force
            obj->applyForce(force, obj->getPosition() + shape.circle.center);
        }
    }
}

/* ************************************************************************ */

void Module::applyBoundaryConditions(const simulator::Simulation& simulation, const VelocityVector& v_max)
{
    // maximum velocity of the Poiseuille inflow
    const auto grid_size = m_lattice.getRealSize();
    const float lb_speed = getVelocityInflow() / v_max.getX();

    auto computePoiseuille = [&grid_size, lb_speed](int i) {
        const float y = (float) (i - 1);
        const float L = (float) (grid_size.getHeight() - 1);
        return 4.f * lb_speed / (L * L) * (L * y - y * y);
    };

    // Generate input / output
    for (Lattice::SizeType y = 0; y < grid_size.getHeight(); ++y)
    {
        // Input
        m_lattice[{1, y}].init({computePoiseuille(y), 0.f});

        // Output
        m_lattice[{grid_size.getWidth() - 2, y}] = m_lattice[{grid_size.getWidth() - 3, y}];
        m_lattice[{grid_size.getWidth() - 1, y}].clear();
    }
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

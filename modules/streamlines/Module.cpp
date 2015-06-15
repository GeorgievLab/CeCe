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

void Module::init(Size size)
{
    // Resize lattice
    m_lattice.setSize(std::move(size));

    // Get grid
    auto& grid = getLattice();
    const auto grid_size = grid.getRealSize();
/*
    auto computePoiseuille = [&grid_size](int i) {
        float y = (float) (i - 1);
        float L = (float) (grid_size.getHeight() - 1);
        return 4.f * MAX_LB_SPEED / (L * L) * (L * y - y * y);
    };
*/
    for (Lattice::SizeType y = 1; y < grid_size.getHeight() - 1; ++y)
    {
        for (Lattice::SizeType x = 1; x < grid_size.getWidth() - 1; ++x)
        {
            //m_lattice[{x, y}].init({computePoiseuille(y), 0.f});
            //m_lattice[{x, y}].init({MAX_LB_SPEED, 0.f});
            m_lattice[{x, y}].init({0.f, 0.f});
        }
    }

    for (Lattice::SizeType x = 1; x < grid_size.getWidth() - 1; ++x)
    {
        m_lattice[{x, 1}].setStaticObstacle(true);
        m_lattice[{x, grid_size.getHeight() - 2}].setStaticObstacle(true);
    }
}

/* ************************************************************************ */

void Module::update(units::Duration dt, simulator::Simulation& simulation)
{
    // Check condition time step condition
    //checkTimeStepCondition(dt, simulation);

    // Dynamic obstacles
    updateDynamicObstacleMap(simulation);

    {
        const auto dl = simulation.getWorldSize() / getLattice().getSize();

        // LB viscosity
        const auto nu_lb = (dt / (dl.getX() * dl.getX())) * getViscosity();

        // Relaxation parameter
        const float tau = (3.f * nu_lb + 0.5f);
        const float omega = 1.f / tau;

        ///////////////////

        // Compute lattice collisions
        getLattice().collide(omega);
        getLattice().propagate();

        ///////////////////
        applyBoundaryConditions(simulation);
    }

    // Apply streamlines to world objects
    applyToObjects(simulation);
}

/* ************************************************************************ */

void Module::configure(const simulator::Configuration& config, simulator::Simulation& simulation)
{
    // Maximum velocity
    config.callIfSetString("velocity-max", [this](const String& value) {
        setVelocityMax(parser::parse_value<units::Velocity>(value));
    });

    // Maximum velocity
    config.callIfSetString("velocity-inflow", [this](const String& value) {
        setVelocityInflow(parser::parse_value<units::Velocity>(value));
    });

    // Viscosity
    config.callIfSetString("viscosity", [this](const String& value) {
        setViscosity(parser::parse_value<units::Viscosity>(value.c_str()));
    });

    // Grid size
    config.callString("grid", [this](const String& value) {
        init(parser::parse_vector_single<Lattice::SizeType>(value));
    });
}

/* ************************************************************************ */

#ifdef ENABLE_RENDER
void Module::draw(render::Context& context, const simulator::Simulation& simulation)
{
#if OPT_DRAW_VELOCITY
    const auto size = getLattice().getSize();

    if (!m_drawable)
        m_drawable.create(context, size);

    Grid<VelocityVector> velocities(size);

    // Update texture
    for (decltype(size.getHeight()) y = 0; y < size.getHeight(); ++y)
    {
        for (decltype(size.getWidth()) x = 0; x < size.getWidth(); ++x)
        {
            // Cell alias
            const auto& cell = m_lattice[{x + 1, y + 1}];
            const Coordinate coord{x, size.getHeight() - y - 1};

            // Background color
            render::Color color = {1, 1, 1, 1};

            if (!cell.isObstacle())
            {
                // Calculate velocity vector
                const auto velocity = cell.calcVelocityNormalized();// / MAX_LB_SPEED;// * 100;// / getFlowSpeed();

                //color = {velocity.getX(), velocity.getY(), velocity.getLength(), 1};
                color = {velocity.getLength(), velocity.getLength(), velocity.getLength(), 1};
                //color = {cell.calcRho(), velocity.getLength(), 0, 1};
                velocities[{x, y}] = velocity;
            }
            else
            {
                velocities[{x, y}] = VelocityVector::Zero;
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

void Module::checkTimeStepCondition(units::Duration dt, const simulator::Simulation& simulation)
{
    // Steps in all directions
    const auto dl = simulation.getWorldSize() / getLattice().getSize();
    const auto dt_max = dl / getVelocityMax();

    if (dt > std::max(dt_max.getX(), dt_max.getY()))
        throw InvalidArgumentException("Time step is to large");
}

/* ************************************************************************ */

void Module::updateDynamicObstacleMap(const simulator::Simulation& simulation)
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

        // Map shapes to grid
        for (const auto& shape : shapes)
        {
            coordIt = mapShapeToGrid(coordIt, shape, step, coord, getLattice().getSize());
        }

        // Sort coordinates
        // TODO: remove lambdas -> will work in Visual Studio?
        std::sort(coords.begin(), coords.end(), [](const decltype(coord)& lhs, const decltype(coord)& rhs) {
            return lhs < rhs;
        });
        coords.erase(std::unique(coords.begin(), coords.end(), [](const decltype(coord)& lhs, const decltype(coord)& rhs) {
            return lhs == rhs;
        }), coords.end());

        for (const auto& c : coords)
        {
            const Coordinate coord(c.getX(), m_lattice.getSize().getHeight() - c.getY());

            if (m_lattice.inRange(coord))
                m_lattice[coord].setDynamicObstacle(true);
        }
    }
}

/* ************************************************************************ */

void Module::applyToObjects(const simulator::Simulation& simulation)
{
    // Get lattice
    const auto& lattice = getLattice();

    const PositionVector start = simulation.getWorldSize() * -0.5f;
    const auto step = simulation.getWorldSize() / lattice.getSize();

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

        coords.clear();
        const auto& shapes = obj->getShapes();
        auto coordIt = std::back_inserter(coords);

        // Map shapes border to grid
        for (const auto& shape : shapes)
        {
            coordIt = mapShapeBorderToGrid(coordIt, shape, step, coord, getLattice().getSize(), {}, 1);
        }

        // Sort coordinates
        // TODO: remove lambdas -> will work in Visual Studio?
        std::sort(coords.begin(), coords.end(), [](const decltype(coord)& lhs, const decltype(coord)& rhs) {
            return lhs < rhs;
        });
        coords.erase(std::unique(coords.begin(), coords.end(), [](const decltype(coord)& lhs, const decltype(coord)& rhs) {
            return lhs == rhs;
        }), coords.end());

        VelocityVector velocity = VelocityVector::Zero;

        for (const auto& c : coords)
        {
            const Coordinate coord(c.getX(), m_lattice.getSize().getHeight() - c.getY());

            // Sum velocities
            velocity += lattice[coord].calcVelocityNormalized() * getVelocityMax();
        }

        assert(!coords.empty());

        // Average
        velocity = velocity / coords.size();

        // Difference between velocities
        const auto dv = velocity - obj->getVelocity();

        // Set object velocity
        //obj->setVelocity(velocity);
        const auto force = 3 * constants::PI * getViscosity() * dv * shapes[0].circle.radius;

        // Apply force
        obj->applyForce(force);
    }
}

/* ************************************************************************ */

void Module::applyBoundaryConditions(const simulator::Simulation& simulation)
{
    /**/
    {
        // maximum velocity of the Poiseuille inflow
        const auto grid_size = getLattice().getRealSize();
        const float lb_speed = getVelocityInflow() / getVelocityMax();

        auto computePoiseuille = [&grid_size, lb_speed](int i) {
            float y = (float) (i - 1);
            float L = (float) (grid_size.getHeight() - 1);
            return 4.f * lb_speed / (L * L) * (L * y - y * y);
        };

        // Generate input / output
        for (Lattice::SizeType y = 0; y < grid_size.getHeight(); ++y)
        {
            m_lattice[{1, y}].init({computePoiseuille(y), 0.f});
            //m_lattice[{1, y}].init({lb_speed, 0.f});
            //m_lattice[{1, y}].init({computePoiseuille(y), 0.f});
            m_lattice[{grid_size.getWidth() - 2, y}].init(
                m_lattice[{grid_size.getWidth() - 3, y}].calcVelocityNormalized(),
                m_lattice[{grid_size.getWidth() - 3, y}].calcRho()
            );
            m_lattice[{grid_size.getWidth() - 1, y}].clear();
        }
    }
    /**/
    /*
    // Make periodic
    const auto realSize = getLattice().getRealSize();
    auto width = realSize.getWidth() - 2;
    auto height = realSize.getHeight() - 2;

    for (decltype(width) x = 1; x <= width; ++x)
    {
        m_lattice[{x, height}][4] = m_lattice[{x, 0}][4];
        m_lattice[{x, height}][7] = m_lattice[{x, 0}][7];
        m_lattice[{x, height}][8] = m_lattice[{x, 0}][8];

        m_lattice[{x, 1}][2] = m_lattice[{x, height + 1}][2];
        m_lattice[{x, 1}][5] = m_lattice[{x, height + 1}][5];
        m_lattice[{x, 1}][6] = m_lattice[{x, height + 1}][6];
    }

    for (decltype(height) y = 1; y <= height; ++y)
    {
        m_lattice[{1, y}][1] = m_lattice[{width + 1, y}][1];
        m_lattice[{1, y}][5] = m_lattice[{width + 1, y}][5];
        m_lattice[{1, y}][8] = m_lattice[{width + 1, y}][8];

        m_lattice[{width, y}][3] = m_lattice[{0, y}][3];
        m_lattice[{width, y}][6] = m_lattice[{0, y}][6];
        m_lattice[{width, y}][7] = m_lattice[{0, y}][7];
    }

    m_lattice[{1, 1}][5]   = m_lattice[{width + 1, height + 1}][5];
    m_lattice[{width, 1}][6]  = m_lattice[{0, height + 1}][6];
    m_lattice[{width, height}][7] = m_lattice[{0, 0}][7];
    m_lattice[{1, height}][8]  = m_lattice[{width + 1, 0}][8];
    */
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

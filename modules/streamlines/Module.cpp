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
#include <random>

// Simulator
#include "parser/Parser.hpp"
#include "simulator/Simulation.hpp"
#include "simulator/Object.hpp"
#include "simulator/ShapeToGrid.hpp"

// Module
#include "ObstacleMap.hpp"

/* ************************************************************************ */

namespace module {
namespace streamlines {

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

    // maximum velocity of the Poiseuille inflow
    const float uMax = getFlowSpeed();

    auto computePoiseuille = [&grid_size, uMax](int i) {
        float y = (float) (i - 1);
        float L = (float) (grid_size.getHeight() - 1);
        return 4.f * uMax / (L * L) * (L * y - y * y);
    };

    for (Lattice::SizeType y = 1; y < grid_size.getHeight() - 1; ++y)
    {
        for (Lattice::SizeType x = 1; x < grid_size.getWidth() - 1; ++x)
        {
            //m_lattice[{x, y}].init({computePoiseuille(y), 0.f});
            m_lattice[{x, y}].init({0.f, 0.f});
        }
    }
/*
    for (Lattice::SizeType x = 1; x < grid_size.getWidth() - 1; ++x)
    {
        m_lattice[{x, 1}].setObstacle(true);
        m_lattice[{x, grid_size.getHeight() - 2}].setObstacle(true);
    }
*/
}

/* ************************************************************************ */

void Module::update(units::Duration dt, simulator::Simulation& simulation)
{
    // Dynamic obstacles
    {
        m_lattice.clearDynamicObstacles();

        const Vector<float> start = simulation.getWorldSize() * -0.5;
        const auto step = simulation.getWorldSize() / m_lattice.getSize();

        // Foreach all cells
        for (auto& obj : simulation.getObjects())
        {
            // Get cell position
            const auto pos = obj->getPosition() - start;

            // TODO: improve
            if ((pos.getX() < 0 || pos.getY() < 0) ||
                (pos.getX() >= simulation.getWorldSize().getWidth() || pos.getY() >= simulation.getWorldSize().getHeight()))
                continue;

            // Get grid position
            Coordinate coord = pos / step;

            std::vector<decltype(coord)> coords;
            const auto& shapes = obj->getShapes();
            auto coordIt = std::inserter(coords, coords.end());

            for (const auto& shape : shapes)
            {
                coordIt = mapShapeToGrid(coordIt, shape, step, coord, getLattice().getSize());
            }

            std::sort(coords.begin(), coords.end(), [](const decltype(coord)& lhs, const decltype(coord)& rhs) {
                return lhs < rhs;
            });
            coords.erase(std::unique(coords.begin(), coords.end(), [](const decltype(coord)& lhs, const decltype(coord)& rhs) {
                return lhs == rhs;
            }), coords.end());

            for (const auto& c : coords)
            {
                Coordinate coord(c.getX(), m_lattice.getSize().getHeight() - c.getY());

                if (m_lattice.inRange(coord))
                    m_lattice[coord].setDynamicObstacle(true);
            }
        }
    }

    {
        // Get grid
        const auto size = getLattice().getSize();

        float obst_x = size.getWidth() / 2;
        float obst_y = size.getHeight() / 2;
        float obst_r = size.getHeight() / 10 + 1;

        // maximum velocity of the Poiseuille inflow
        const float uMax = getFlowSpeed();
        // Reynolds number
        const float Re = 100;
        // kinematic fluid viscosity
        const float nu = uMax * 2.f * obst_r / Re;
        // relaxation parameter
        const float omega = 1.f / (3.f * nu + 1.f / 2.f);

        ///////////////////

        // Compute lattice collisions
        getLattice().collide(omega);
        getLattice().propagate();

        ///////////////////
        /**/
        {
            // maximum velocity of the Poiseuille inflow
            const float uMax = getFlowSpeed();
            const auto grid_size = getLattice().getRealSize();

            auto computePoiseuille = [&grid_size, uMax](int i) {
                float y = (float) (i - 1);
                float L = (float) (grid_size.getHeight() - 1);
                return 4.f * uMax / (L * L) * (L * y - y * y);
            };

            // Generate input / output
            for (Lattice::SizeType y = 0; y < grid_size.getHeight(); ++y)
            {
                m_lattice[{1, y}].init({computePoiseuille(y), 0.f});
                //m_lattice[{0, y}].init({0.f, 0.f});
                //m_lattice[{1, y}].init({computePoiseuille(y), 0.f});
                //m_lattice[{grid_size.getWidth() - 1, y}].init(m_lattice[{grid_size.getWidth() - 2, y}].calcVelocityNormalized());
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

    // Apply streamlines to world objects
    /*
    {
        // Get grid
        const auto& grid = getGrid();

        const Vector<float> start = simulation.getWorldSize() * -0.5;
        const auto step = simulation.getWorldSize() / grid.getSize();

        for (auto& obj : simulation.getObjects())
        {
            // Ignore static objects
            if (obj->getType() == simulator::Object::Type::Static)
                continue;

            // Get position
            const auto pos = obj->getPosition() - start;

            // TODO: improve
            if ((pos.getX() < 0 || pos.getY() < 0) ||
                (pos.getX() >= simulation.getWorldSize().getWidth() || pos.getY() >= simulation.getWorldSize().getHeight()))
                continue;

            // Get grid position
            Vector<SizeType> coord = pos / step;

            // Get velocity
            const auto force = grid[coord] * m_flowSpeed;

            // Add acceleration to the object
            obj->applyForce(force);
        }
    }
    */
}

/* ************************************************************************ */

void Module::configure(const simulator::Configuration& config, simulator::Simulation& simulation)
{
    // Flow speed
    config.callIfSetString("flow-speed", [this](const String& value) {
        setFlowSpeed(parser::parse_value<float>(value));
    });

    // Grid size
    config.callIfSetString("grid", [this](const String& value) {
        init(parser::parse_vector<Lattice::SizeType>(value));
    });

    {
        auto obstacleMap = config.getString("obstacle-map");

        if (!obstacleMap.empty())
        {
            // Create obstacle map with given size
            auto map = ObstacleMap::createFromFile(config.buildFilePath(obstacleMap), m_lattice.getSize());
            auto size = map.getSize();

            for (unsigned int y = 0; y < size.getHeight(); ++y)
            {
                for (unsigned int x = 0; x < size.getWidth(); ++x)
                {
                    m_lattice[{x + 1, y + 1}].setStaticObstacle(map[{x, y}]);
                }
            }
        }
    }
}

/* ************************************************************************ */

#ifdef ENABLE_RENDER
void Module::draw(render::Context& context, const simulator::Simulation& simulation)
{
#if OPT_DRAW_VELOCITY
    const auto size = getLattice().getSize();

    if (!m_drawable)
        m_drawable.create(context, size);

    // Update texture
    for (decltype(size.getHeight()) y = 0; y < size.getHeight(); ++y)
    {
        for (decltype(size.getWidth()) x = 0; x < size.getWidth(); ++x)
        {
            // Cell alias
            const auto& cell = m_lattice[{x + 1, y + 1}];
            const Coordinate coord{x, size.getHeight() - y - 1};

            // Background color
            render::Color color = {0, 0, 0, 1};

            if (!cell.isObstacle())
            {
                // Calculate velocity vector
                const auto velocity = cell.calcVelocityNormalized() / getFlowSpeed();

                //color = {velocity.getX(), velocity.getY(), velocity.getLength(), 1};
                color = {velocity.getLength(), velocity.getLength(), velocity.getLength(), 1};
                //color = {0, velocity.getLength(), 0, 1};
            }

            // Set color
            m_drawable.set(coord, color);
        }
    }

    // Draw color grid
    context.matrixPush();
    context.matrixScale(simulation.getWorldSize());
    m_drawable.draw(context);
    context.matrixPop();
#endif
}
#endif

/* ************************************************************************ */

}
}

/* ************************************************************************ */

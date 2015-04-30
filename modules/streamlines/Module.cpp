
// Declaration
#include "Module.hpp"

// C++
#include <random>

// Simulator
#include "simulator/Simulation.hpp"
#include "simulator/Object.hpp"
#include "parser/Parser.hpp"

/* ************************************************************************ */

namespace module {
namespace streamlines {

/* ************************************************************************ */

Module::~Module()
{
    // Nothing to do
}

/* ************************************************************************ */

void Module::update(units::Duration dt, simulator::Simulation& simulation)
{
    if (m_update)
    {
        const auto R = 10.5f;

        // Precompute values
        const auto R2 = R * R;
        const Vector<float> start = simulation.getWorldSize() * -0.5f;
        const auto gridSize = m_grid.getSize();
        const Vector<float> step = simulation.getWorldSize() / gridSize;

        for (decltype(gridSize.getHeight()) j = 0; j < gridSize.getHeight(); ++j)
        {
            for (decltype(gridSize.getWidth()) i = 0; i < gridSize.getWidth(); ++i)
            {
                auto& velocity = m_grid[Vector<SizeType>(i, j)];

                // Transform i, j coordinates to position
                // Cell center position
                const Vector<float> coord = Vector<float>(i, j) + 0.5f;
                // Real position in the world
                const Vector<float> pos = start + step * coord;// - getMainCell()->getPosition();

                // Calculate squared distance from main cell
                const auto distSq = pos.getLengthSquared();

                // Cell is in main cell, ignore
                if (distSq <= R2)
                {
                    velocity = Vector<float>{0.f, 0.f};
                    continue;
                }
    /*
                // Precompute values
                const float distQuad = distSq * distSq;
                const float xx = pos.x * pos.x;
                const float xy = pos.x * pos.y;

                // COPYRIGHT: Hynek magic
                cell.velocity.x = U * (1 + R2 / distSq - 2 * (xx * R2) / distQuad);
                cell.velocity.y = U * -2 * (R2 * xy) / distQuad;
    */
                const float theta = atan2(pos.getY(), pos.getX());

                const Vector<float> u = Vector<float>{
                    cosf(theta) * (1 - R2 / distSq),
                    -sinf(theta) * (1 + R2 / distSq)
                };

                velocity = u.rotated(theta);
                //cell.velocity = u;
            }
        }

        m_update = false;
        m_renderUpdate = true;
    }

    // Apply streamlines to world objects
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
}

/* ************************************************************************ */

void Module::configure(const simulator::ConfigurationBase& config)
{
    // Grid size
    {
        auto grid = config.getString("grid");

        if (!grid.empty())
            m_grid.resize(parser::parse_vector<SizeType>(grid));
    }

    // Flow speed
    {
        auto flowSpeed = config.getString("flow-speed");

        if (!flowSpeed.empty())
            setFlowSpeed(parser::parse_value<float>(flowSpeed));
    }

#ifdef ENABLE_RENDER
    // Draw flags
    {
        auto drawVelocity = config.getString("draw-velocity");

        if (!drawVelocity.empty())
            m_renderObject.setRenderVelocity(parser::parse_bool(drawVelocity));
    }
#endif
}

/* ************************************************************************ */

#ifdef ENABLE_RENDER
void Module::drawInit(render::Context& context)
{
    m_renderObject.init(context, m_grid.getSize(), m_grid.getData());
}
#endif

/* ************************************************************************ */

#ifdef ENABLE_RENDER
void Module::draw(render::Context& context, const simulator::Simulation& simulation)
{
    if (m_renderUpdate)
    {
        m_renderObject.update(m_grid.getData());
        m_renderUpdate = false;
    }

    m_renderObject.draw(simulation.getWorldSize());
}
#endif

/* ************************************************************************ */

}
}

/* ************************************************************************ */

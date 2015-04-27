
// Declaration
#include "Module.hpp"

// C++
#include <random>

// Simulator
#include "simulator/Simulation.hpp"
#include "simulator/Object.hpp"

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
        const Vector<float> step = simulation.getWorldSize() / m_grid.getSize();

        for (decltype(m_grid.getHeight()) j = 0; j < m_grid.getHeight(); ++j)
        {
            for (decltype(m_grid.getWidth()) i = 0; i < m_grid.getWidth(); ++i)
            {
                auto& velocity = m_grid(i, j);

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


// Declaration
#include "Module.hpp"

// C++
#include <random>

// Simulator
#include "simulator/Simulation.hpp"
#include "simulator/DynamicObject.hpp"

/* ************************************************************************ */

namespace module {
namespace streamlines {

/* ************************************************************************ */

void Module::MainCell::setRadius(units::Length radius) noexcept
{
    shape.m_radius = radius;

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    getBody()->CreateFixture(&fixtureDef);
}

/* ************************************************************************ */

Module::~Module()
{
    // Nothing to do
}

/* ************************************************************************ */

void Module::update(units::Duration dt, simulator::Simulation& simulation)
{
    if (!m_mainCell)
    {
        m_mainCell = simulation.createObject<MainCell>();
        m_mainCell->setRadius(units::um3(10.5));
    }

    if (m_update)
    {
        const auto R = getMainCell()->shape.m_radius;

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
                const Vector<float> pos = start + step * coord - getMainCell()->getPosition();

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
            if (!obj->hasFlag(simulator::OBJECT_DYNAMIC))
                continue;

            // Cast to dynamic object
            auto ptr = obj->cast<simulator::DynamicObject>();

            // Get position
            auto pos = ptr->getPosition() - start;

            // TODO: improve
            if ((pos.getX() < 0 || pos.getY() < 0) ||
                (pos.getX() >= simulation.getWorldSize().getWidth() || pos.getY() >= simulation.getWorldSize().getHeight()))
                continue;

            // Get grid position
            Vector<SizeType> coord = pos / step;

            // Get velocity
            const auto velocity = grid[coord] * m_flowSpeed;

            // Get velocity change
            const auto acceleration = (velocity - ptr->getVelocity()) / dt;

            // Add acceleration to the object
            ptr->addAcceleration(acceleration);
        }
    }
}

/* ************************************************************************ */

#ifdef ENABLE_RENDER
void Module::drawInit(render::Context& context)
{
    m_renderCell.init();
    m_renderObject.init(m_grid.getSize(), m_grid.getData());
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

    // Draw main cell
    if (getMainCell() != nullptr)
    {
        const auto& pos = getMainCell()->getPosition();
        m_renderCell.draw(pos, getMainCell()->shape.m_radius, {0.5f, 0.5f, 0.5f, 0.8f});
    }
}
#endif

/* ************************************************************************ */

}
}

/* ************************************************************************ */

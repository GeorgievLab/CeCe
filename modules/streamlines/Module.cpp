
// Declaration
#include "Module.hpp"

// C++
#include <random>

// Simulator
#include "simulator/World.hpp"
#include "simulator/DynamicObject.hpp"

// TODO remove
#include "simulator/Cell.hpp"

/* ************************************************************************ */

namespace module {
namespace streamlines {

/* ************************************************************************ */

Module::Module()
    : m_grid(500)
    , m_mainCellRadius(units::um(1))
    , m_flowSpeed(1.f)
{
    // Nothing to do
}

/* ************************************************************************ */

Module::~Module()
{
    // Nothing to do
}

/* ************************************************************************ */

void Module::update(units::Duration dt, simulator::World& world)
{
    // Generate cells
    if (false)
    {
        const float half = world.getHeight() / 2.f;

        std::random_device rd;
        std::default_random_engine eng(rd());

        std::bernoulli_distribution d(0.5);

        // If cell should be generated
        if (d(eng))
        {
            std::uniform_real_distribution<float> dist(-half, half);
            float y = dist(eng);

            // Create cell
            simulator::Cell* cell = world.createObject<simulator::Cell>();
            cell->setVolume(units::um3(200));
            cell->setVelocity({10, 0});
            cell->setPosition({-world.getWidth() / 2.f, y});
        }
    }

    // TODO: recalc only when parameters are changed.

    if (m_update)
    {
        const auto R = getMainCellRadius();

        // Precompute values
        const auto R2 = R * R;
        const Vector<float> start = world.getSize() * -0.5f;
        const Vector<float> step = world.getSize() / m_grid.getSize();

        for (decltype(m_grid.getHeight()) j = 0; j < m_grid.getHeight(); ++j)
        {
            for (decltype(m_grid.getWidth()) i = 0; i < m_grid.getWidth(); ++i)
            {
                auto& velocity = m_grid(i, j);

                // Transform i, j coordinates to position
                // Cell center position
                const Vector<float> coord = Vector<float>(i, j) + 0.5f;
                // Real position in the world
                const Vector<float> pos = start + step * coord - getMainCellPosition();

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
        const Vector<float> start = world.getSize() * -0.5;

        for (auto& obj : world.getObjects())
        {
            auto ptr = dynamic_cast<simulator::DynamicObject*>(obj.get());
            if (!ptr)
                continue;

            auto pos = ptr->getPosition() - start;

            // Cell is out of world
            if ((pos.getX() < 0 || pos.getY() < 0) ||
                (pos.getX() >= world.getWidth() || pos.getY() >= world.getHeight()))
                continue;

            // Get grid
            const auto& grid = getGrid();

            // Get step
            const auto step = world.getSize() / grid.getSize();

            // Get grid position
            Vector<SizeType> coord = pos / step;

            // Get velocity
            const auto& velocity = grid[coord];

            // Use velocity
            ptr->setVelocity(velocity * m_flowSpeed);
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
void Module::draw(render::Context& context, const simulator::World& world)
{
    if (m_renderUpdate)
    {
        m_renderObject.update(m_grid.getData());
        m_renderUpdate = false;
    }

    m_renderObject.draw(world.getSize());

    // Draw main cell
    if (getMainCellRadius())
    {
        const auto& pos = getMainCellPosition();
        m_renderCell.draw(pos, getMainCellRadius(), {0.5f, 0.5f, 0.5f, 0.8f});
    }
}
#endif

/* ************************************************************************ */

}
}

/* ************************************************************************ */

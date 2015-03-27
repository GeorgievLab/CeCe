
/* ************************************************************************ */

// Declaration
#include "simulator/World.hpp"

// C++
#include <algorithm>

// Simulator
#include "simulator/StaticObject.hpp"
#include "simulator/DynamicObject.hpp"

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

World::World() noexcept
    : m_grid(10)
{
    // Nothing to do
}

/* ************************************************************************ */

World::~World()
{
    // Nothing to do
}

/* ************************************************************************ */

void World::clean()
{
    m_stepNumber = 0;
    m_objects.clear();
}

/* ************************************************************************ */

void World::reset()
{
    // Nothing to do yet
}

/* ************************************************************************ */

void World::update(units::Duration dt) noexcept
{
    // Increase step number
    m_stepNumber++;

    // Update all cells
    // Cell update can update cell list
    for (auto& obj : getObjects())
    {
        assert(obj);
        obj->update(dt);
    }

    // Apply streamlines
    {
        const Vector<float> start{
            -getWidth() / 2.f,
            -getHeight() / 2.f
        };

        for (auto& obj : getObjects())
        {
            auto ptr = dynamic_cast<DynamicObject*>(obj.get());
            if (!ptr)
                continue;

            auto pos = ptr->getPosition();
            pos.x -= start.x;
            pos.y -= start.y;

            auto step_x = getWidth() / m_grid.getWidth();
            auto step_y = getHeight() / m_grid.getHeight();

            // Get grid position
            unsigned int i = pos.x / step_x;
            unsigned int j = pos.y / step_y;

            const GridCell& cell = m_grid(i, j);

            // Use velocity
            ptr->setVelocity(cell.velocity);
        }
    }

    {
        const auto wh = getWidth() / 2.f;
        const auto hh = getWidth() / 2.f;

        // Kill objects that are outside world
        std::remove_if(m_objects.begin(), m_objects.end(), [wh, hh](const ObjectContainer::value_type& obj) {
            auto ptr = dynamic_cast<StaticObject*>(obj.get());

            if (!ptr)
                return false;

            // Get object position
            const Position& pos = ptr->getPosition();

            return not
                ((pos.x >= -wh) && (pos.x <= wh)) &&
                ((pos.y >= -hh) && (pos.y <= hh))
            ;
        });
    }
}

/* ************************************************************************ */

#ifdef ENABLE_RENDER

void World::render(render::Context& context)
{
    if (getRenderFlags() & RENDER_GRID)
    {
        context.drawGrid(
            {getWidth(), getHeight()},
            {m_grid.getWidth(), m_grid.getHeight()},
            {0.9f, 0.5f, 0.5f, 0.2f}
        );
    }

    context.drawCircle({0, 0}, 10, {0, 0, 0, 1});

    if (getRenderFlags() & RENDER_VELOCITY)
    {
        const Vector<float> start{
            -getWidth() / 2.f,
            -getHeight() / 2.f
        };

        const Vector<float> step{
            getWidth() / m_grid.getWidth(),
            getHeight() / m_grid.getHeight()
        };

        const auto U = 200;

        // Draw grid vectors
        for (Grid<GridCell>::SizeType i = 0; i < m_grid.getWidth(); ++i)
        {
            for (Grid<GridCell>::SizeType j = 0; j < m_grid.getHeight(); ++j)
            {
                const GridCell& cell = m_grid(i, j);
                float x = start.x + i * step.x + step.x / 2.f;
                float y = start.y + j * step.y + step.y / 2.f;

                context.drawLine(
                    {x, y},
                    {cell.velocity.x * step.x / 2.f / U, cell.velocity.y * step.y / 2.f / U},
                    {cell.velocity.x, cell.velocity.y, 0, 1}
                );
            }
        }
    }

    // Draw objects
    for (const auto& obj : getObjects())
        obj->render(context);
}

#endif

/* ************************************************************************ */

}

/* ************************************************************************ */

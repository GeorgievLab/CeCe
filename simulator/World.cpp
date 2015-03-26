
/* ************************************************************************ */

// Declaration
#include "simulator/World.hpp"

// C++
#include <algorithm>
#include <random>

// Simulator
#include "simulator/StaticObject.hpp"
#include "simulator/DynamicObject.hpp"

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

World::World() noexcept
    : m_grid(50)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1);

    for (Grid<GridCell>::SizeType i = 0; i < m_grid.getWidth(); ++i)
    {
        for (Grid<GridCell>::SizeType j = 0; j < m_grid.getHeight(); ++j)
        {
            GridCell& cell = m_grid(i, j);

            cell.velocity.x = dis(gen);
            cell.velocity.y = dis(gen);
        }
    }
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

            // Get grid position
            unsigned int i = pos.x / m_grid.getWidth();
            unsigned int j = pos.y / m_grid.getHeight();

            const GridCell& cell = m_grid(i, j);

            // TODO: apply cell force
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
    context.drawGrid(
        {getWidth(), getHeight()},
        {m_grid.getWidth(), m_grid.getHeight()},
        {0.9f, 0.5f, 0.5f, 0.2f}
    );

    {
        const Vector<float> start{
            -getWidth() / 2.f,
            -getHeight() / 2.f
        };

        const Vector<float> step{
            getWidth() / m_grid.getWidth(),
            getHeight() / m_grid.getHeight()
        };

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
                    {cell.velocity.x * step.x / 2.f, cell.velocity.y * step.y / 2.f},
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

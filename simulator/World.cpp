
/* ************************************************************************ */

// Declaration
#include "World.h"

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

World::World() noexcept
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
    m_cells.clear();
    m_barriers.clear();
}

/* ************************************************************************ */

void World::reset()
{
    // Nothing to do yet
}

/* ************************************************************************ */

void World::update(float step) noexcept
{
    // Update all cells
    for (auto& cell : m_cells)
    {
        // Update cell data
        cell->update();
    }

    // Update physics
    physics::World::update(step);

    const auto wh = getWidth() / 2.f;
    const auto hh = getWidth() / 2.f;

    // Kill overflow cells
    for (auto it = m_cells.begin(); it != m_cells.end();)
    {
        const auto& cell = *it;
        const auto& pos = cell->getPosition();

        // World space
        if (((pos.x.value() >= -wh.value()) && (pos.x.value() <= wh.value())) &&
            ((pos.z.value() >= -hh.value()) && (pos.z.value() <= hh.value())))
        {
            ++it;
        }
        else
        {
            // Kill
            it = m_cells.erase(it);
        }
    }
}

/* ************************************************************************ */

}

/* ************************************************************************ */

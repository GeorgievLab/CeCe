
/* ************************************************************************ */

// Declaration
#include "World.h"

// Simulator
#include "simulator/Cell.h"

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

World::World(std::unique_ptr<WorldImplData> data) noexcept
    : m_implData(std::move(data))
{
    // Nothing to do
}

/* ************************************************************************ */

void World::update(float step) noexcept
{
    // Update all cells
    for (auto& cell : m_cells)
    {
        if (cell->getBehaviour())
            cell->getBehaviour()(*cell);
    }

    physics::World::update(step);
}

/* ************************************************************************ */

}

/* ************************************************************************ */

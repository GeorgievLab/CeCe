
/* ************************************************************************ */

// Declaration
#include "World.h"

// C++
#include <algorithm>

// Library
#include "Cell.h"

/* ************************************************************************ */

World::World() noexcept
{
    // Nothing to do
}

/* ************************************************************************ */

World::~World()
{
    // Cells must be destroyed before the physics world
    m_cells.clear();
    m_barriers.clear();
}

/* ************************************************************************ */

Cell* World::FindCell(Cell::Id cellId) noexcept
{
    auto it = FindCellPosition(cellId);

    return it != m_cells.end() ? it->get() : nullptr;
}

/* ************************************************************************ */

const Cell* World::FindCell(Cell::Id cellId) const noexcept
{
    auto it = FindCellPosition(cellId);

    return it != m_cells.end() ? it->get() : nullptr;
}

/* ************************************************************************ */

World::CellContainer::iterator World::FindCellPosition(Cell::Id cellId) noexcept
{
    return std::find_if(m_cells.begin(), m_cells.end(), [&cellId](const std::unique_ptr<Cell>& cell) {
        return cell->GetId() == cellId;
    });
}

/* ************************************************************************ */

World::CellContainer::const_iterator World::FindCellPosition(Cell::Id cellId) const noexcept
{
    return std::find_if(m_cells.begin(), m_cells.end(), [&cellId](const std::unique_ptr<Cell>& cell) {
        return cell->GetId() == cellId;
    });
}

/* ************************************************************************ */

void World::Sort() noexcept
{
    // Sort only when needed
    if (!m_sorted)
        std::sort(m_cells.begin(), m_cells.end());
}

/* ************************************************************************ */

void World::Update() noexcept
{
    WorldPhysics::Update();
}

/* ************************************************************************ */

//void World::Render(wxDC& dc) const noexcept
void World::Render() const noexcept
{
    for (const auto& cell : m_cells)
    {
        cell->Render();
    }

    for (const auto& barrier : m_barriers)
    {
        barrier.Render();
    }
}

/* ************************************************************************ */

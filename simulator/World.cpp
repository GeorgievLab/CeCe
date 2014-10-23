
/* ************************************************************************ */

// Declaration
#include "World.h"

// Simulator
#include "simulator/Cell.h"

// Render
#include "render/World.h"

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

World::World(std::unique_ptr<WorldImplData> data) noexcept
    : m_implData(std::move(data))
{
    // Nothing to do
}

/* ************************************************************************ */

World::~World()
{
    // Cells must be destroyed before the physics world
    m_cells.clear();
}

/* ************************************************************************ */

void World::update() noexcept
{
    std::lock_guard<std::mutex> _(m_accessMutex);

    physics::World::Update();
}

/* ************************************************************************ */

void World::draw(render::Context& context) const noexcept
{
    std::lock_guard<std::mutex> _(m_accessMutex);

    // Render world
    render::World::render(context);

    // Render cells
    for (const auto& cell : m_cells)
    {
        assert(cell);
        cell->draw(context);
    }
}

/* ************************************************************************ */

}

/* ************************************************************************ */

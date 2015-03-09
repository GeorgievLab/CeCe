
/* ************************************************************************ */

// Declaration
#include "World.hpp"

// C++
#include <algorithm>

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
    m_stepNumber = 0;
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
    // Increase step number
    m_stepNumber++;

    // Update all cells
    for (auto& cell : m_cells)
    {
        //assert(cell);
        // Update cell data
        // FIXME not null
        if (cell) cell->update();
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

#ifdef ENABLE_RENDER

void World::draw(render::Context& context)
{
    // Axis
    context.drawAxis({0, 0, 0}, 5);

    // Plane
    context.drawPlane({0, 0, 0}, getWidth().value(), getHeight().value(), {0.9f, 0.9f, 0.9f, 0.6f});

    // Draw barriers
    {
        //for (auto& barrier : getBarriers())
        //    context.drawBarrier(*barrier);
    }

    {
        // Draw cells in order to camera view

        // Get camera position and order cells by distance from camera
        const auto& cam_pos = context.getCamera().getPosition();

        std::vector<Cell*> cells;

        // Copy cell pointers
        for (const auto& cell : getCells())
        {
            cells.push_back(cell.get());
        }

        // Order cells
        std::sort(cells.begin(), cells.end(), [&cam_pos](const Cell* c1, const Cell* c2) {
            auto distance = [](const Position& pos1, const Position& pos2) -> float {
                return sqrt(
                    ((pos1.x.value() - pos2.x.value()) * (pos1.x.value() - pos2.x.value())) +
                    ((pos1.y.value() - pos2.y.value()) * (pos1.y.value() - pos2.y.value())) +
                    ((pos1.z.value() - pos2.z.value()) * (pos1.z.value() - pos2.z.value()))
                );
            };

            const auto cpos = Position{Length(cam_pos.x), Length(cam_pos.y), Length(cam_pos.z)};
            auto d1 = distance(c1->getPosition(), cpos);
            auto d2 = distance(c2->getPosition(), cpos);

            return d1 < d2;
        });

        // Draw cells
        for (const auto& cell : cells)
            cell->draw(context);
    }
}

#endif

/* ************************************************************************ */

}

/* ************************************************************************ */

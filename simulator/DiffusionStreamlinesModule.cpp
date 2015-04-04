
/* ************************************************************************ */

// Declaration
#include "simulator/DiffusionStreamlinesModule.hpp"

// C++
#include <limits>
#include <algorithm>

// Simulator
#include "core/Matrix.hpp"
#include "simulator/World.hpp"

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

DiffusionStreamlinesModule::DiffusionStreamlinesModule()
{
    // Nothing to do
}

/* ************************************************************************ */

DiffusionStreamlinesModule::~DiffusionStreamlinesModule()
{
    // Nothing to do
}

/* ************************************************************************ */

void DiffusionStreamlinesModule::update(units::Duration dt, World& world)
{
    StreamlinesModule::update(dt, world);
    DiffusionModule::update(dt, world);

    static constexpr float PI = 3.14159265359f;

    auto& grid = DiffusionModule::getGrid();
    auto& velocityGrid = StreamlinesModule::getGrid();

    // Precompute values
    const auto start = world.getStartPosition();
    const auto step = world.calcStep(grid.getSize());

    // Grid for changes
    Grid<float> diffGrid(grid.getWidth(), grid.getHeight());

    // Sizes must match
    assert(std::distance(grid.begin(), grid.end()) == std::distance(diffGrid.begin(), diffGrid.end()));

    // Generate initial diff grid values (remove previous values)
    std::transform(grid.begin(), grid.end(), diffGrid.begin(), std::negate<float>());

    // Transform i, j coordinates to position
    auto getPosition = [&start, &step](const Vector<unsigned int>& pos) {
        // Cell center position
        const Vector<float> coord = Vector<float>(pos) + 0.5f;
        // Real position
        return start + step * coord;
    };

    for (decltype(grid.getHeight()) j = 0; j < grid.getHeight(); ++j)
    {
        for (decltype(grid.getWidth()) i = 0; i < grid.getWidth(); ++i)
        {
            const Vector<unsigned> ij(i, j);

            auto& signal = grid[ij];

            // No signal to send
            if (signal == 0)
                continue;

            auto& velocity = velocityGrid[ij];

            // Calculate new position
            const Vector<float> new_pos = getPosition(ij) + velocity * dt * getFlowSpeed();

            // Get grid position
            const Vector<unsigned> new_ij = (new_pos - start) / step;

            // Out of range
            if (!grid.inRange(new_ij))
                continue;

            diffGrid[new_ij] += signal;
        }
    }

    // Add changes to the main grid
    std::transform(grid.begin(), grid.end(), diffGrid.begin(), grid.begin(), std::plus<float>());
}

/* ************************************************************************ */

#ifdef ENABLE_RENDER
void DiffusionStreamlinesModule::renderInit(render::Context& context)
{
    DiffusionModule::renderInit(context);
    StreamlinesModule::renderInit(context);
}
#endif

/* ************************************************************************ */

#ifdef ENABLE_RENDER
void DiffusionStreamlinesModule::render(render::Context& context, const World& world)
{
    DiffusionModule::render(context, world);
    StreamlinesModule::render(context, world);
}
#endif

/* ************************************************************************ */

}

/* ************************************************************************ */

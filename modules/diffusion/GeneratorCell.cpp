
// Declaration
#include "GeneratorCell.hpp"

// C++
#include <cassert>

// Simulator
#include "simulator/Cell.hpp"
#include "simulator/World.hpp"

// Module
#include "Module.hpp"

/* ************************************************************************ */

namespace module {
namespace diffusion {

/* ************************************************************************ */

void GeneratorCell::update(units::Duration dt, simulator::World& world)
{
    constexpr float SOURCE_STRENGTH = 200.f;

    assert(m_diffusionModule);
    auto& grid = m_diffusionModule->getGrid();

    const Vector<float> start = world.getSize() * -0.5;
    const auto step = world.getSize() / grid.getSize();

    // Foreach all cells
    for (auto& obj : world.getObjects())
    {
        auto ptr = dynamic_cast<simulator::Cell*>(obj.get());
        if (!ptr)
            continue;

        // Get cell position
        const auto pos = ptr->getPosition() - start;
        const unsigned int signal = ptr->getId() % Signal::COUNT;

        // Cell is out of world
        if ((pos.getX() < 0 || pos.getY() < 0) ||
            (pos.getX() >= world.getWidth() || pos.getY() >= world.getHeight()))
            continue;

        // Get grid position
        const auto coord = pos / step;

        // Add signal
        grid[coord][signal] += SOURCE_STRENGTH * dt;
    }
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

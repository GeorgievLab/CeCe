
// Declaration
#include "GeneratorCell.hpp"

// C++
#include <cassert>

// Simulator
#include "simulator/Simulation.hpp"

// Module
#include "Module.hpp"

// Module "cell"
#include "../cell/Cell.hpp"

/* ************************************************************************ */

namespace module {
namespace diffusion {

/* ************************************************************************ */

void GeneratorCell::update(units::Duration dt, simulator::Simulation& simulation)
{
    constexpr float SOURCE_STRENGTH = 100.f;

    assert(m_diffusionModule);
    auto& grid = m_diffusionModule->getGrid();

    const Vector<float> start = simulation.getWorldSize() * -0.5;
    const auto step = simulation.getWorldSize() / grid.getSize();

    // Foreach all cells
    for (auto& obj : simulation.getObjects())
    {
        // It's not cell
        if (!obj->is<module::cell::Cell>())
            continue;

        // Cast to cell
        auto ptr = obj->cast<module::cell::Cell>();

        // Get cell position
        const auto pos = ptr->getPosition() - start;
        const unsigned int signal = ptr->getId() % Signal::COUNT;

        // TODO: improve
        if ((pos.getX() < 0 || pos.getY() < 0) ||
            (pos.getX() >= simulation.getWorldSize().getWidth() || pos.getY() >= simulation.getWorldSize().getHeight()))
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

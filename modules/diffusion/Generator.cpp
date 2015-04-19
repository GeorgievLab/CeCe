
// Declaration
#include "Generator.hpp"

// C++
#include <cassert>

// Module
#include "Module.hpp"

/* ************************************************************************ */

namespace module {
namespace diffusion {

/* ************************************************************************ */

void Generator::update(units::Duration dt, simulator::Simulation& simulation)
{
    constexpr float SOURCE_STRENGTH = 50.f;

    assert(m_diffusionModule);
    auto& grid = m_diffusionModule->getGrid();

    const float grid_half = grid.getHeight() / 2;
    const int off = grid.getHeight() * 0.05f;

    for (int i = -6; i <= 6; ++i)
        grid(0, grid_half + i * off)[(i + 6) % Signal::COUNT] += SOURCE_STRENGTH * dt;
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

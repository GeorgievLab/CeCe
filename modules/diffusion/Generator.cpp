
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

Generator::~Generator()
{
    // Nothing to do
}

/* ************************************************************************ */

void Generator::update(units::Duration dt, simulator::World& world)
{
    constexpr float SOURCE_STRENGTH = 1000.f;

    assert(m_diffusionModule);
    auto& grid = m_diffusionModule->getGrid();

    const float grid_half = grid.getHeight() / 2;
    const int off = grid.getHeight() * 0.1f;

    for (int i = -1; i < 1; ++i)
        grid(0, grid_half + (i + 0.5) * off) += SOURCE_STRENGTH * dt;
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

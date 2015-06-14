
// Declaration
#include "Generator.hpp"

// C++
#include <cassert>

// Simulator
#include "core/TimeMeasurement.hpp"
#include "simulator/Simulation.hpp"

// Module
#include "Module.hpp"

/* ************************************************************************ */

namespace module {
namespace diffusion {

/* ************************************************************************ */

void Generator::update(units::Duration dt, simulator::Simulation& simulation)
{
    auto _ = measure_time("diffusion.generator", [&simulation](std::ostream& out, const std::string& name, Clock::duration dt) {
        out << name << ";" << simulation.getStepNumber() << ";" << std::chrono::duration_cast<std::chrono::microseconds>(dt).count() << "\n";
    });

    CONSTEXPR float SOURCE_STRENGTH = 500.f;

    assert(m_diffusionModule);
    auto& grid = m_diffusionModule->getGrid();

    const float grid_half = grid.getSize().getHeight() / 2;
    const int off = grid.getSize().getHeight() * 0.05f;

    for (int i = -6; i <= 6; ++i)
        grid[Vector<unsigned int>{0u, (unsigned) grid_half + i * off}][(i + 6) % Signal::COUNT] += SOURCE_STRENGTH * dt.value();
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

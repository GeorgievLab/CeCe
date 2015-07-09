
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

namespace plugin {
namespace diffusion {

/* ************************************************************************ */

void Generator::update(units::Duration dt, simulator::Simulation& simulation)
{
    auto _ = measure_time("diffusion.generator", simulator::TimeMeasurementIterationOutput(simulation));

    CONSTEXPR float SOURCE_STRENGTH = 500.f;

    assert(m_diffusionModule);

    // Grid size
    const auto& size = m_diffusionModule->getGridSize();

    const float grid_half = size.getHeight() / 2;
    const int off = size.getHeight() * 0.05f;

    for (int i = -6; i <= 6; ++i)
    {
        const auto id = (i + 6) % m_diffusionModule->getSignalCount();
        const auto coord = Vector<unsigned int>{0u, (unsigned) grid_half + i * off};
        m_diffusionModule->getSignal(id, coord) += SOURCE_STRENGTH * dt.value();
    }
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

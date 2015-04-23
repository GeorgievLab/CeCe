
// Declaration
#include "Generator.hpp"

// C++
#include <random>

// Simulator
#include "core/TimeMeasurement.hpp"
#include "simulator/Simulation.hpp"

// Module
#include "Cell.hpp"
#include "Yeast.hpp"

/* ************************************************************************ */

namespace module {
namespace cell {

/* ************************************************************************ */

void Generator::update(units::Duration dt, simulator::Simulation& simulation)
{
    auto _ = measure_time("cell.generator", [&simulation](std::ostream& out, const std::string& name, Clock::duration dt) {
        out << name << ";" << simulation.getStepNumber() << ";" << std::chrono::duration_cast<std::chrono::microseconds>(dt).count() << "\n";
    });

    const float half = simulation.getWorldSize().getHeight() / 2.f;

    std::random_device rd;
    std::default_random_engine eng(rd());

    std::bernoulli_distribution d(3.f * dt);
    std::uniform_real_distribution<float> volume_d(30.f, 43.f);

    // If cell should be generated
    if (d(eng))
    {
        std::uniform_real_distribution<float> dist(-half, half);
        float y = dist(eng);

        // Create cell
        module::cell::Yeast* c = simulation.createObject<module::cell::Yeast>();
        c->setVolume(units::um3(volume_d(eng)));
        c->setVelocity({10, 0});
        c->setPosition({-simulation.getWorldSize().getWidth() / 2.f + 0.1f, y});
        c->setGfp(20);
    }
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

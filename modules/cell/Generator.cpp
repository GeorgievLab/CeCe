
// Declaration
#include "Generator.hpp"

// C++
#include <random>

// Simulator
#include "simulator/Simulation.hpp"

// Module
#include "Module.hpp"
#include "Cell.hpp"
#include "Yeast.hpp"

/* ************************************************************************ */

namespace module {
namespace cell {

/* ************************************************************************ */

void Generator::update(units::Duration dt, simulator::Simulation& simulation)
{
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
        c->setGfp(10000);
    }
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

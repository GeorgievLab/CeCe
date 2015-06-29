
// Declaration
#include "Generator.hpp"

// C++
#include <random>

// Simulator
#include "core/TimeMeasurement.hpp"
#include "parser/Parser.hpp"
#include "simulator/Simulation.hpp"
#include "simulator/Object.hpp"

/* ************************************************************************ */

namespace plugin {
namespace cell {

/* ************************************************************************ */

void Generator::update(units::Duration dt, simulator::Simulation& simulation)
{
    auto _ = measure_time("cell.generator", simulator::TimeMeasurementIterationOutput(&simulation));

    const units::Length half = simulation.getWorldSize().getHeight() / 2.f;

    std::random_device rd;
    std::default_random_engine eng(rd());

    std::bernoulli_distribution genDist(getProbability());
    std::uniform_real_distribution<float> volume_d(30.f, 43.f);

    // If object should be generated
    if (genDist(eng))
    {
        std::uniform_real_distribution<float> dist(-half.value(), half.value());
        const auto y = units::Length(dist(eng));

        // Create object
        auto* object = simulation.buildObject(getObjectClass());
        //c->setVolume(units::um3(volume_d(eng)));
        object->setVelocity(getStartVelocity());
        object->setPosition({-simulation.getWorldSize().getWidth() / 2.f + units::um(0.1f), y});
    }
}

/* ************************************************************************ */

void Generator::configure(const simulator::Configuration& config, simulator::Simulation& simulation)
{
    config.callString("object", [this](const String& value) {
        setObjectClass(value);
    });

    config.callIfSetString("probability", [this](const String& value) {
        setProbability(parser::parse_value<units::Probability>(value));
    });

    config.callIfSetString("start-velocity", [this](const String& value) {
        setStartVelocity(parser::parse_vector<VelocityVector::value_type>(value));
    });
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

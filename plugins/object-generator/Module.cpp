/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* ************************************************************************ */

// Declaration
#include "Module.hpp"

// C++
#include <cassert>
#include <random>

// Simulator
#include "core/TimeMeasurement.hpp"
#include "parser/Parser.hpp"
#include "simulator/Simulation.hpp"
#include "simulator/Object.hpp"

/* ************************************************************************ */

namespace plugin {
namespace object_generator {

/* ************************************************************************ */

void Module::update(units::Duration dt, simulator::Simulation& simulation)
{
    auto _ = measure_time("object-generator", simulator::TimeMeasurementIterationOutput(&simulation));

    // Random engine
    std::default_random_engine eng;

    // Foreach generated objects
    for (const auto& desc : m_objects)
    {
        std::bernoulli_distribution distSpawn(desc.probability);

        // Spawn?
        if (!distSpawn(eng))
            continue;

        // Create object
        auto* object = simulation.buildObject(desc.className);
        assert(object);

        // Generate position
        std::uniform_real_distribution<float> distX(desc.positionMin.getX().value(), desc.positionMax.getX().value());
        std::uniform_real_distribution<float> distY(desc.positionMin.getY().value(), desc.positionMax.getY().value());
        object->setPosition({units::Length(distX(eng)), units::Length(distY(eng))});
    }

}

/* ************************************************************************ */

void Module::configure(const simulator::Configuration& config, simulator::Simulation& simulation)
{
    const auto half = simulation.getWorldSize() * 0.5f;
    const auto minPos = PositionVector{-half.getX(), -half.getY()};
    const auto maxPos = PositionVector{-half.getX() * 0.95, half.getY()};

    for (auto cfg : config.getConfigurations("object"))
    {
        ObjectDesc desc;
        desc.className = cfg->getString("class");
        desc.probability = parser::parse_value<units::Probability>(cfg->getString("probability"));
        desc.positionMin = minPos;
        desc.positionMax = maxPos;

        cfg->callIfSetString("position-min", [&desc] (const String& value) {
           desc.positionMin = parser::parse_vector<units::Length>(value);
        });

        cfg->callIfSetString("position-max", [&desc] (const String& value) {
           desc.positionMax = parser::parse_vector<units::Length>(value);
        });

        add(std::move(desc));
    }
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

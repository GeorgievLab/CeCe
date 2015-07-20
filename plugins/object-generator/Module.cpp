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
#include <string>
#include <sstream>

// Simulator
#include "core/TimeMeasurement.hpp"
#include "parser/Parser.hpp"
#include "simulator/Simulation.hpp"
#include "simulator/Object.hpp"

/* ************************************************************************ */

namespace plugin {
namespace object_generator {

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

/**
 * @brief Split string into multiple strings separated by separator.
 *
 * @param value
 * @param separator
 *
 * @return
 */
DynamicArray<String> split(String value, char separator) noexcept
{
    DynamicArray<String> elems;
    std::istringstream ss(std::move(value));
    String item;

    while (std::getline(ss, item, separator))
    {
        elems.push_back(item);
    }

    return elems;
}

/* ************************************************************************ */

}

/* ************************************************************************ */

void Module::update(units::Duration dt, simulator::Simulation& simulation)
{
    auto _ = measure_time("object-generator", simulator::TimeMeasurementIterationOutput(&simulation));

    // Random engine
    std::random_device rd;
    std::default_random_engine gen(rd());

    // Foreach generated objects
    for (const auto& desc : m_objects)
    {
        std::bernoulli_distribution distSpawn(desc.probability);

        // Spawn?
        if (!distSpawn(gen))
            continue;

        // Create object
        auto* object = simulation.buildObject(desc.className);
        assert(object);

        // Generate position
        std::uniform_real_distribution<float> distX(desc.positionMin.getX().value(), desc.positionMax.getX().value());
        std::uniform_real_distribution<float> distY(desc.positionMin.getY().value(), desc.positionMax.getY().value());
        object->setPosition({units::Length(distX(gen)), units::Length(distY(gen))});
        object->setVelocity({units::um_s(10), Zero});

        // Create programs
        for (const auto& name : desc.programs)
        {
            auto program = simulation.getProgram(name);

            if (program)
                object->addProgram(std::move(program));
            else
                Log::warning("Unable to create program: ", name);
        }
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

        cfg->callIfSetString("programs", [&desc] (const String& value) {
           desc.programs = split(value, ' ');
        });

        add(std::move(desc));
    }
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

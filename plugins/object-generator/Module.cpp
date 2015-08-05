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
    auto _ = measure_time("object-generator", simulator::TimeMeasurementIterationOutput(simulation));

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
        object->setVelocity(desc.velocity);

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
    const auto minPos = PositionVector{       -half.getX(), 0.9 * -half.getY()};
    const auto maxPos = PositionVector{0.95 * -half.getX(), 0.9 *  half.getY()};

    for (auto&& cfg : config.getConfigurations("object"))
    {
        ObjectDesc desc;
        desc.className = cfg.get("class");
        desc.probability = cfg.get<units::Probability>("probability");
        desc.positionMax = maxPos;
        desc.positionMin = cfg.get("position-min", minPos);
        desc.positionMax = cfg.get("position-max", maxPos);
        desc.velocity = cfg.get("velocity", VelocityVector{units::Velocity(10), Zero});
        desc.programs = split(cfg.get("programs", String()), ' ');

        add(std::move(desc));
    }
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

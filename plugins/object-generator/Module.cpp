/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/*                                                                          */
/* This file is part of CeCe.                                               */
/*                                                                          */
/* CeCe is free software: you can redistribute it and/or modify             */
/* it under the terms of the GNU General Public License as published by     */
/* the Free Software Foundation, either version 3 of the License, or        */
/* (at your option) any later version.                                      */
/*                                                                          */
/* CeCe is distributed in the hope that it will be useful,                  */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of           */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            */
/* GNU General Public License for more details.                             */
/*                                                                          */
/* You should have received a copy of the GNU General Public License        */
/* along with CeCe.  If not, see <http://www.gnu.org/licenses/>.            */
/*                                                                          */
/* ************************************************************************ */

// Declaration
#include "Module.hpp"

// C++
#include <cassert>
#include <random>
#include <string>

// Simulator
#include "core/Pair.hpp"
#include "core/StringStream.hpp"
#include "core/TimeMeasurement.hpp"
#include "simulator/Simulation.hpp"
#include "simulator/Object.hpp"

/* ************************************************************************ */

namespace plugin {
namespace object_generator {

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

// Random engine
std::random_device g_rd;

/* ************************************************************************ */

std::default_random_engine g_gen(g_rd());

/* ************************************************************************ */

/**
 * @brief Parse list of active iterations.
 *
 * @param str
 *
 * @return
 */
DynamicArray<Pair<simulator::IterationNumber, simulator::IterationNumber>> parseActive(String str)
{
    DynamicArray<Pair<simulator::IterationNumber, simulator::IterationNumber>> res;

    InStringStream iss(std::move(str));

    while (true)
    {
        simulator::IterationNumber it;

        if (!(iss >> it))
            break;

        if (iss.peek() == '-')
        {
            simulator::IterationNumber itEnd;
            iss.ignore();
            iss >> itEnd;

            res.push_back({it, itEnd});
        }
        else
        {
            // Single item range
            res.push_back({it, it});
        }
    }

    return res;
}

/* ************************************************************************ */

/**
 * @brief Check if iteration is in range.
 *
 * @param list
 * @param it
 *
 * @return
 */
bool inRange(const DynamicArray<Pair<simulator::IterationNumber, simulator::IterationNumber>>& list, simulator::IterationNumber it)
{
    // No limitation
    if (list.empty())
        return true;

    for (const auto& p : list)
    {
        if (it >= p.first && it <= p.second)
            return true;
    }

    return false;
}

/* ************************************************************************ */

}

/* ************************************************************************ */

void Module::update(units::Duration dt, simulator::Simulation& simulation)
{
    auto _ = measure_time("object-generator", simulator::TimeMeasurementIterationOutput(simulation));

    // Get current iteration number
    const auto iteration = simulation.getIteration();

    // Foreach generated objects
    for (const auto& desc : m_objects)
    {
        // Skip inactive generators
        if (!inRange(desc.active, iteration))
            continue;

        std::bernoulli_distribution distSpawn(desc.probability);

        // Spawn?
        if (!distSpawn(g_gen))
            continue;

        // Create object
        auto object = simulation.buildObject(desc.className);
        assert(object);

        // Generate position
        std::uniform_real_distribution<float> distX(desc.positionMin.getX().value(), desc.positionMax.getX().value());
        std::uniform_real_distribution<float> distY(desc.positionMin.getY().value(), desc.positionMax.getY().value());
        object->setPosition({units::Length(distX(g_gen)), units::Length(distY(g_gen))});
        object->configure(desc.config, simulation);
    }

}

/* ************************************************************************ */

void Module::configure(const simulator::Configuration& config, simulator::Simulation& simulation)
{
    const auto half = simulation.getWorldSize() * 0.5f;
    const auto minPos = PositionVector{       -half.getX(), 0.8 * -half.getY()};
    const auto maxPos = PositionVector{0.95 * -half.getX(), 0.8 *  half.getY()};

    for (auto&& cfg : config.getConfigurations("object"))
    {
        ObjectDesc desc;
        desc.className = cfg.get("class");
        desc.probability = cfg.get<units::Probability>("probability");
        desc.positionMin = cfg.get("position-min", minPos);
        desc.positionMax = cfg.get("position-max", maxPos);
        desc.active = parseActive(cfg.get("active", String{}));
        desc.config = cfg.toMemory();

        add(std::move(desc));
    }
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

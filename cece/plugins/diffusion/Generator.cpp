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
#include "cece/plugins/diffusion/Generator.hpp"

// cece
#include "cece/core/Assert.hpp"
#include "cece/core/TimeMeasurement.hpp"
#include "cece/core/Shape.hpp"
#include "cece/core/ShapeToGrid.hpp"
#include "cece/core/StringStream.hpp"
#include "cece/core/UnitIo.hpp"
#include "cece/simulator/TimeMeasurement.hpp"
#include "cece/simulator/Simulation.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace diffusion {

/* ************************************************************************ */

namespace {

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

void Generator::update()
{
    auto _ = measure_time("diffusion.generator", simulator::TimeMeasurement(getSimulation()));

    // Get current iteration number
    const auto iteration = getSimulation().getIteration();

    Assert(m_diffusionModule);
    const auto gridSize = m_diffusionModule->getGridSize();

    // Step size
    const PositionVector start = getSimulation().getWorldSize() * -0.5f;
    const auto step = getSimulation().getWorldSize() / gridSize;

    // Foreach sources
    for (const auto& source : m_sources)
    {
        // Skip inactive generators
        if (!inRange(source.active, iteration))
            continue;

        const auto id = m_diffusionModule->getSignalId(source.name);

        // Transform from [-size / 2, size / 2] to [0, size] space
        const auto pos = source.position - start;

        // Check if position is in range
        if (!pos.inRange(Zero, getSimulation().getWorldSize()))
            continue;

        // Source shape
        const auto shape = Shape::makeRectangle(source.size);

        // Get coordinate to grid
        const auto coord = diffusion::Module::Coordinate(pos / step);

        // Read all coordinates
        DynamicArray<plugin::diffusion::Module::Coordinate> coords;
        mapShapeToGrid(std::inserter(coords, coords.end()), shape, step, coord, units::Angle(0), gridSize);

        // No coordinates
        if (coords.empty())
            coords.push_back(coord);

        const auto dt = getSimulation().getTimeStep();
        const auto add = (source.production * dt) / coords.size();

        // Add signal
        for (auto& c: coords)
            m_diffusionModule->getSignal(id, c) += add;
    }
}

/* ************************************************************************ */

void Generator::loadConfig(const config::Configuration& config)
{
    m_diffusionModule = getSimulation().getModule("diffusion");

    if (!m_diffusionModule)
        throw RuntimeException("Diffusion module required!");

    // Foreach signal configurations
    for (auto&& cfg : config.getConfigurations("source"))
    {
        m_sources.push_back(Source{
            cfg.get("signal"),
            cfg.get<PositionVector>("position"),
            cfg.get<ProductionRate>("production"),
            cfg.get<PositionVector>("size"),
            parseActive(cfg.get("active", String{}))
        });
    }
}

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */

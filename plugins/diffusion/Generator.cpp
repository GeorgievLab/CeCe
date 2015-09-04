/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */

// Declaration
#include "Generator.hpp"

// C++
#include <cassert>

// Simulator
#include "core/TimeMeasurement.hpp"
#include "simulator/Shape.hpp"
#include "simulator/ShapeToGrid.hpp"

/* ************************************************************************ */

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

void Generator::update(units::Time dt, simulator::Simulation& simulation)
{
    auto _ = measure_time("diffusion.generator", simulator::TimeMeasurementIterationOutput(simulation));

    // Get current iteration number
    const auto iteration = simulation.getIteration();

    assert(m_diffusionModule);
    const auto gridSize = m_diffusionModule->getGridSize();

    // Step size
    const PositionVector start = simulation.getWorldSize() * -0.5f;
    const auto step = simulation.getWorldSize() / gridSize;

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
        if (!pos.inRange(Zero, simulation.getWorldSize()))
            continue;

        // Source shape
        const auto shape = simulator::Shape::makeRectangle(source.size);

        // Get coordinate to grid
        const auto coord = diffusion::Module::Coordinate(pos / step);

        // Read all coordinates
        DynamicArray<plugin::diffusion::Module::Coordinate> coords;
        mapShapeToGrid(std::inserter(coords, coords.end()), shape, step, coord, 0, gridSize);

        // No coordinates
        if (coords.empty())
            coords.push_back(coord);

        const auto add = (source.production * dt) / coords.size();

        // Add signal
        for (auto& c: coords)
            m_diffusionModule->getSignal(id, c) += add;
    }
}

/* ************************************************************************ */

void Generator::configure(const simulator::Configuration& config, simulator::Simulation& simulation)
{
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

/* ************************************************************************ */

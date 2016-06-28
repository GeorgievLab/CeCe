/* ************************************************************************ */
/* Georgiev Lab (c) 2015-2016                                               */
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
#include "cece/plugins/diffusion/ExportModule.hpp"

// C++
#include <algorithm>

// CeCe
#include "cece/core/Assert.hpp"
#include "cece/core/IteratorRange.hpp"
#include "cece/core/VectorRange.hpp"
#include "cece/core/Exception.hpp"
#include "cece/core/InStream.hpp"
#include "cece/core/OutStream.hpp"
#include "cece/core/StringStream.hpp"
#include "cece/config/Configuration.hpp"
#include "cece/simulator/Simulation.hpp"

// Plugin
#include "cece/plugins/diffusion/Module.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace diffusion {

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

/**
 * @brief Parse array of values from string.
 *
 * @param str Source string.
 *
 * @return
 */
DynamicArray<String> split(String str)
{
    DynamicArray<String> array;
    InStringStream is(std::move(str));
    String val;

    // Read values
    while (is >> val)
        array.push_back(std::move(val));

    return array;
}

/* ************************************************************************ */

/**
 * @brief Cast array of values to string.
 *
 * @param array Array of values.
 *
 * @return
 */
String join(const DynamicArray<String>& array)
{
    String res;

    for (auto it = array.begin(); it != array.end(); ++it)
    {
        if (it != array.begin())
            res += " ";

        res += *it;
    }

    return res;
}

/* ************************************************************************ */

}

/* ************************************************************************ */

bool ExportModule::isExported(StringView name) const noexcept
{
    // In case of no expoted signals, export all
    if (m_signals.empty())
        return true;

    auto it = std::find_if(m_signals.begin(), m_signals.end(), [&](const String& signal) {
        return signal == name;
    });

    return it != m_signals.end();
}

/* ************************************************************************ */

void ExportModule::loadConfig(const config::Configuration& config)
{
    module::ExportModule::loadConfig(config);

    // Get exported signals
    setSignals(split(config.get("signals", String{})));
    setExportObstacles(config.get("obstacles", isExportedObstacles()));
}

/* ************************************************************************ */

void ExportModule::storeConfig(config::Configuration& config) const
{
    module::ExportModule::storeConfig(config);

    config.set("signals", join(getSignals()));
    config.set("obstacles", isExportedObstacles());
}

/* ************************************************************************ */

void ExportModule::init()
{
    // Get diffusion module
    m_module = getSimulation().getModule("diffusion");

    if (!m_module)
        throw RuntimeException("Diffusion module required!");

    module::ExportModule::init();

    // Write output header
    if (isExportedObstacles())
        writeHeader("iteration", "time", "x", "y", "signal", "concentration", "obstacle");
    else
        writeHeader("iteration", "time", "x", "y", "signal", "concentration");
}

/* ************************************************************************ */

void ExportModule::update()
{
    Assert(m_module);

    // Get simulation
    const auto& sim = getSimulation();
    const auto iteration = sim.getIteration();
    const auto totalTime = sim.getTotalTime();

    // Module is not active
    if (!isActive(iteration))
        return;

    // Foreach coordinates
    for (auto&& c : range(m_module->getGridSize()))
    {
        // Foreach signals
        for (auto signalId : m_module->getSignalIds())
        {
            // Skip non-exported signals
            if (!isExported(m_module->getSignalName(signalId)))
                continue;

            // Write record
            if (isExportedObstacles())
            {
                writeRecord(
                    iteration,
                    totalTime.value(),
                    c.getX(),
                    c.getY(),
                    m_module->getSignalName(signalId),
                    m_module->getSignal(signalId, c).value(),
                    m_module->isObstacle(c)
                );
            }
            else
            {
                writeRecord(
                    iteration,
                    totalTime.value(),
                    c.getX(),
                    c.getY(),
                    m_module->getSignalName(signalId),
                    m_module->getSignal(signalId, c).value()
                );
            }
        }
    }
}

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */

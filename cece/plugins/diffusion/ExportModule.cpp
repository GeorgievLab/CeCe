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
#include "cece/core/Range.hpp"
#include "cece/core/VectorRange.hpp"
#include "cece/core/Exception.hpp"
#include "cece/core/InStream.hpp"
#include "cece/core/OutStream.hpp"
#include "cece/config/Configuration.hpp"
#include "cece/simulator/Simulation.hpp"

// Plugin
#include "cece/plugins/diffusion/Module.hpp"

/* ************************************************************************ */

namespace cece {
inline namespace core {
namespace {

/* ************************************************************************ */

/**
 * @brief Cast array of values from string.
 *
 * @param value Source value.
 *
 * @return
 */
InStream& operator>>(InStream& is, DynamicArray<String>& array)
{
    String val;

    // Read values
    while (is >> val)
        array.push_back(std::move(val));

    return is;
}

/* ************************************************************************ */

/**
 * @brief Cast array of values to string.
 *
 * @param array Array of values.
 *
 * @return
 */
OutStream& operator<<(OutStream& os, const DynamicArray<String>& array)
{
    for (auto it = array.begin(); it != array.end(); ++it)
    {
        if (it != array.begin())
            os << " ";

        os << *it;
    }

    return os;
}

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace diffusion {

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
    setSignals(config.get("signals", getSignals()));
    setExportObstacles(config.get("obstacles", isExportedObstacles()));
}

/* ************************************************************************ */

void ExportModule::storeConfig(config::Configuration& config) const
{
    module::ExportModule::storeConfig(config);

    config.set("signals", getSignals());
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
    writeHeader("iteration", "time", "x", "y", "signal", "concentration",
        CsvFile::cond(isExportedObstacles(), "obstacle")
    );
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
            writeRecord(
                iteration,
                totalTime.value(),
                c.getX(),
                c.getY(),
                m_module->getSignalName(signalId),
                m_module->getSignal(signalId, c).value(),
                CsvFile::cond(isExportedObstacles(), m_module->isObstacle(c))
            );
        }
    }
}

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */

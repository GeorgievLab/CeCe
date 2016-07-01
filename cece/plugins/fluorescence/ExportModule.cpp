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
#include "cece/plugins/fluorescence/ExportModule.hpp"

// C++
#include <algorithm>

// CeCe
#include "cece/core/Assert.hpp"
#include "cece/core/UnitIo.hpp"
#include "cece/core/IteratorRange.hpp"
#include "cece/core/VectorRange.hpp"
#include "cece/config/Configuration.hpp"
#include "cece/simulator/Simulation.hpp"

#ifdef CECE_ENABLE_RENDER
#  include "cece/simulator/Visualization.hpp"
#endif

// Plugins
#include "cece/plugins/fluorescence/Module.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace fluorescence {

/* ************************************************************************ */

void ExportModule::loadConfig(const config::Configuration& config)
{
    module::ExportModule::loadConfig(config);

    setPosition(config.get("position", getPosition()));
    setSize(config.get("size", getSimulation().getWorldSize()));
    setTotal(config.get("total", isTotal()));

#ifdef CECE_ENABLE_RENDER
    setLayerName(config.get("layer", getLayerName()));
    setColor(config.get("color", getColor()));
#endif
}

/* ************************************************************************ */

void ExportModule::storeConfig(config::Configuration& config) const
{
    module::ExportModule::storeConfig(config);

    config.set("position", getPosition());
    config.set("size", getSize());
    config.set("total", isTotal());

#ifdef CECE_ENABLE_RENDER
    config.set("layer", getLayerName());
    config.get("color", getColor());
#endif
}

/* ************************************************************************ */

void ExportModule::init()
{
    // Get fluorescence module
    m_module = getSimulation().getModule("fluorescence");

    if (!m_module)
        throw RuntimeException("'fluorescence' module required!");

    module::ExportModule::init();

#ifdef CECE_ENABLE_RENDER
    setZOrder(1001);
#endif

    // Write output header
    if (isTotal())
        writeHeader("iteration", "time", "protein", "amount", "cells", "average");
    else
        writeHeader("iteration", "time", "x", "y", "xw", "yw", "protein", "amount", "average", "cells", "average");
}

/* ************************************************************************ */

void ExportModule::update()
{
    // Get simulation
    const auto& sim = getSimulation();
    const auto iteration = sim.getIteration();
    const auto totalTime = sim.getTotalTime();

    // Module is not active
    if (!isActive(iteration))
        return;

    Assert(m_module);

    // Get fluorescence grid
    const auto& grid = m_module->getGrid();

    const auto worldSize = sim.getWorldSize();
    const auto worldSizeH = worldSize * 0.5;
    const auto cellSize = worldSize / grid.getSize();

    // Calculate active area
    const units::SizeVector areaHalf = m_size * 0.5;
    const units::PositionVector areaMin = m_position - areaHalf;
    const units::PositionVector areaMax = m_position + areaHalf;

    using GridType = fluorescence::Module::GridType;

    const auto sizeMin = GridType::CoordinateType((areaMin + worldSizeH) / cellSize);
    const auto sizeMax = GridType::CoordinateType((areaMax + worldSizeH) / cellSize);

    if (isTotal())
    {
        // Accumulate values from given area

        GridType::ValueType total{0, 0, 0};

        // Foreach grid
        for (auto&& c : range(sizeMin, sizeMax))
        {
            if (!grid.inRange(c))
                continue;

            // Get data
            const auto record = grid[c];
            const auto& proteins = record.proteins;

            // No data
            if (!(proteins.rfp || proteins.gfp || proteins.yfp))
                continue;

            total.proteins.rfp += proteins.rfp;
            total.proteins.gfp += proteins.gfp;
            total.proteins.yfp += proteins.yfp;
            total.count += record.count;
        }

        // Write record
        writeRecord(iteration, totalTime.value(), "RFP",
            total.proteins.rfp, total.count, total.count ? (double) total.proteins.rfp / total.count : 0
        );
        writeRecord(iteration, totalTime.value(), "GFP",
            total.proteins.gfp, total.count, total.count ? (double) total.proteins.gfp / total.count : 0
        );
        writeRecord(iteration, totalTime.value(), "YFP",
            total.proteins.yfp, total.count, total.count ? (double) total.proteins.yfp / total.count : 0
        );
    }
    else
    {
        // Store each value from given area

        // Foreach grid
        for (auto&& c : range(sizeMin, sizeMax))
        {
            if (!grid.inRange(c))
                continue;

            // Get data
            const auto record = grid[c];
            const auto& proteins = record.proteins;

            // No data
            if (!(proteins.rfp || proteins.gfp || proteins.yfp))
                continue;

            // Write record
            writeRecord(iteration, totalTime.value(), c.getX(), c.getY(),
                cellSize.getWidth() * c.getX(), cellSize.getHeight() * c.getY(),
                "RFP", proteins.rfp, record.count, record.count ? (double) proteins.rfp / record.count : 0
            );
            writeRecord(iteration, totalTime.value(), c.getX(), c.getY(),
                cellSize.getWidth() * c.getX(), cellSize.getHeight() * c.getY(),
                "GFP", proteins.gfp, record.count, record.count ? (double) proteins.gfp / record.count : 0
            );
            writeRecord(iteration, totalTime.value(), c.getX(), c.getY(),
                cellSize.getWidth() * c.getX(), cellSize.getHeight() * c.getY(),
                "YFP", proteins.yfp, record.count, record.count ? (double) proteins.yfp / record.count : 0
            );
        }
    }
}

/* ************************************************************************ */

#ifdef CECE_ENABLE_RENDER
void ExportModule::draw(const simulator::Visualization& visualization, render::Context& context)
{
    // If visualization is disabled, do nothing
    if (!visualization.isEnabled(getLayerName()))
        return;

    if (!m_drawable)
        m_drawable.create(context);

    Assert(m_drawable);

    context.matrixPush();
    context.matrixTranslate(getPosition());
    context.matrixScale(getSize() / units::Length(1));
    context.colorPush();
    context.setColor(render::colors::WHITE);
    context.enableAlpha();
    m_drawable->draw(context, getColor(), render::PrimitiveType::LineLoop);
    context.disableAlpha();
    context.colorPop();
    context.matrixPop();
}
#endif

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */

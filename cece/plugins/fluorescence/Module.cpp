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
#include "cece/plugins/fluorescence/Module.hpp"

// C++
#include <algorithm>

// CeCe
#include "cece/core/Log.hpp"
#include "cece/core/UnitIo.hpp"
#include "cece/core/VectorRange.hpp"
#include "cece/config/Configuration.hpp"
#include "cece/simulator/Simulation.hpp"

#ifdef CECE_ENABLE_RENDER
#  include "cece/render/Color.hpp"
#  include "cece/simulator/Visualization.hpp"
#endif

// Plugins
#include "cece/plugins/cell/CellBase.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace fluorescence {

/* ************************************************************************ */

void Module::loadConfig(const config::Configuration& config)
{
    // Configure parent
    module::Module::loadConfig(config);

    // Set grid size
    m_grid.resize(config.get<GridType::CoordinateType>("size"));

#ifdef CECE_ENABLE_RENDER
    m_layerName = config.get("layer", String{});
#endif
}

/* ************************************************************************ */

void Module::storeConfig(config::Configuration& config) const
{
    module::Module::storeConfig(config);
}

/* ************************************************************************ */

void Module::init()
{
    module::Module::init();

#ifdef CECE_ENABLE_RENDER
    setZOrder(1000);
#endif
}

/* ************************************************************************ */

void Module::update()
{
    const auto& sim = getSimulation();
    const auto worldSize = sim.getWorldSize();
    const auto worldSizeH = worldSize * 0.5;
    const auto cellSize = worldSize / m_grid.getSize();

    std::fill(m_grid.begin(), m_grid.end(), Fluorescence{});

    // Foreach all objects
    for (const auto& object : sim.getObjects())
    {
        // Not a cell
        if (!object->is<cell::CellBase>())
            continue;

        // Get object position
        const auto position = object->getPosition();

        // Cast to cell
        const auto cell = object->cast<cell::CellBase>();
        Assert(cell);

        // Calculate grid coordinates
        const auto coordinates = GridType::CoordinateType((position + worldSizeH) / cellSize);
        Assert(m_grid.inRange(coordinates));

        const auto gfp = cell->getMoleculeCount("GFP");
        const auto rfp = cell->getMoleculeCount("RFP");
        const auto yfp = cell->getMoleculeCount("YFP");

        // Sum of proteins
        auto sum = gfp + rfp + yfp;

        if (sum == 0)
            continue;

        // TODO: conversion
        Fluorescence fluorescence = sum / 50.0;

        m_grid[coordinates] += fluorescence;
    }
}

/* ************************************************************************ */

#ifdef CECE_ENABLE_RENDER
void Module::draw(const simulator::Visualization& visualization, render::Context& context)
{
    if (!m_layerName.empty() && !visualization.isEnabled(m_layerName))
        return;

    if (!m_drawable)
        m_drawable.create(context, m_grid.getSize());

    Assert(m_drawable);

    // Foreach grid
    for (auto&& c : range(m_grid.getSize()))
    {
        const auto fluorescence = m_grid[c];

        // Set color
        m_drawable->set(c, render::Color::fromGray(fluorescence, fluorescence));
    }

    // Synchronize
    m_drawable->sync();

    context.matrixPush();
    context.matrixScale(getSimulation().getWorldSize() / units::Length(1));
    context.colorPush();
    context.setColor(render::colors::WHITE);
    context.enableAlpha();
    m_drawable->draw(context);
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

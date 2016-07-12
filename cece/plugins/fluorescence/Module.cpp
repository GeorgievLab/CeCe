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
    String layerName = config.get("layer", String{});
    m_layerNames.rfp = config.get("layer-RFP", layerName);
    m_layerNames.gfp = config.get("layer-GFP", layerName);
    m_layerNames.yfp = config.get("layer-YFP", layerName);

    DensityType saturation = config.get("saturation", DensityType(100));
    m_saturations.rfp = config.get("saturation-RFP", saturation);
    m_saturations.gfp = config.get("saturation-GFP", saturation);
    m_saturations.yfp = config.get("saturation-YFP", saturation);
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

    std::fill(m_grid.begin(), m_grid.end(), Record{});

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

        // Object is out of range
        if (!m_grid.inRange(coordinates))
            continue;

        const auto gfp = cell->getMoleculeCount("GFP");
        const auto rfp = cell->getMoleculeCount("RFP");
        const auto yfp = cell->getMoleculeCount("YFP");

        // Add molecules
        m_grid[coordinates].proteins.rfp += rfp;
        m_grid[coordinates].proteins.gfp += gfp;
        m_grid[coordinates].proteins.yfp += yfp;
        m_grid[coordinates].count += 1;
    }
}

/* ************************************************************************ */

#ifdef CECE_ENABLE_RENDER
void Module::draw(const simulator::Visualization& visualization, render::Context& context)
{
    // If visualization is disabled, do nothing
    if (!(visualization.isEnabled(m_layerNames.rfp) ||
          visualization.isEnabled(m_layerNames.gfp) ||
          visualization.isEnabled(m_layerNames.yfp)))
        return;

    if (!m_drawable)
        m_drawable.create(context, m_grid.getSize());

    Assert(m_drawable);

    RenderState& state = m_drawableState.getFront();

    // Set image data
    m_drawable->setImage(state.image);

    context.matrixPush();
    context.matrixScale(state.scale);
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

#ifdef CECE_ENABLE_RENDER
void Module::drawStoreState()
{
    RenderState& state = m_drawableState.getBack();

    state.scale = getSimulation().getWorldSize() / units::Length(1);

    // Calculate cell area
    const auto cellSize = getSimulation().getWorldSize() / m_grid.getSize();
    const units::Area cellArea = cellSize.getWidth() * cellSize.getHeight();

    // Resize image
    state.image.resize(m_grid.getSize());

    // Foreach grid
    for (auto&& c : range(m_grid.getSize()))
    {
        // Get number of proteins
        const auto record = m_grid[c];
        const auto& proteins = record.proteins;

        // Calculate density
        const auto rfpDensity = proteins.rfp / cellArea;
        const auto gfpDensity = proteins.gfp / cellArea;
        const auto yfpDensity = proteins.yfp / cellArea;

        // Colors
        const auto rfp = rfpDensity / m_saturations.rfp;
        const auto gfp = gfpDensity / m_saturations.gfp;
        const auto yfp = yfpDensity / m_saturations.yfp;

        auto red    = rfp + 0.5 * yfp;
        auto green  = gfp + 0.5 * yfp;
        auto blue   = RealType(0);
        auto alpha  = std::max({red, green, blue});

        // Set color
        state.image.set(c, render::Color(red, green, blue, alpha));
    }
}
#endif

/* ************************************************************************ */

#ifdef CECE_ENABLE_RENDER
void Module::drawSwapState()
{
    m_drawableState.swap();
}
#endif

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */

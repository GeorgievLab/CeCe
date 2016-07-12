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
#include "Module.hpp"

// C++
#include <algorithm>

// CeCe
#include "cece/core/UnitIo.hpp"
#include "cece/config/Configuration.hpp"
#include "cece/simulator/Simulation.hpp"

#ifdef CECE_ENABLE_RENDER
#  include "cece/simulator/Visualization.hpp"
#endif

// Plugins
#include "cece/plugins/cell/CellBase.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace cytometry {

/* ************************************************************************ */

void Module::loadConfig(const config::Configuration& config)
{
    // Configure parent
    module::ExportModule::loadConfig(config);

    setName(config.get("name"));
    setPosition(config.get<units::PositionVector>("position"));
    setSize(config.get<units::SizeVector>("size"));

#ifdef CECE_ENABLE_RENDER
    setLayerName(config.get("layer", getLayerName()));
    setColor(config.get("color", getColor()));
#endif
}

/* ************************************************************************ */

void Module::storeConfig(config::Configuration& config) const
{
    module::ExportModule::storeConfig(config);

    config.set("name", getName());
    config.set("position", getPosition());
    config.set("size", getSize());

#ifdef CECE_ENABLE_RENDER
    config.set("layer", getLayerName());
    config.get("color", getColor());
#endif
}

/* ************************************************************************ */

void Module::init()
{
    module::ExportModule::init();

    // Write output header
    writeHeader("iteration", "time", "object_id", "size", "gfp", "rfp", "yfp");
}

/* ************************************************************************ */

void Module::update()
{
    const auto& sim = getSimulation();
    const auto iteration = sim.getIteration();
    const auto totalTime = sim.getTotalTime();

    // Module is not active
    if (!isActive(iteration))
        return;

    // Calculate active area
    const units::SizeVector areaHalf = getSize() * 0.5;
    const units::PositionVector areaMin = getPosition() - areaHalf;
    const units::PositionVector areaMax = getPosition() + areaHalf;

    Set<ViewPtr<object::Object>> objects;

    // Foreach all objects
    for (const auto& object : sim.getObjects())
    {
        // Not a cell
        if (!object->is<cell::CellBase>())
            continue;

        // Get object position
        const auto position = object->getPosition();

        // Object is outside
        if (!position.inRange(areaMin, areaMax))
            continue;

        // Store object
        objects.insert(object);
    }

    // Objects that left measurement window
    DynamicArray<ViewPtr<object::Object>> leftObjects;

    // Find objects that left measurement window
    std::set_difference(
        m_objects.begin(), m_objects.end(),
        objects.begin(), objects.end(),
        std::back_inserter(leftObjects)
    );

    // Store left objects data
    for (const auto& object : leftObjects)
    {
        Assert(object->is<cell::CellBase>());

        // Cast to cell
        const auto cell = object->cast<cell::CellBase>();
        Assert(cell);

        const auto gfp = cell->getMoleculeCount("GFP");
        const auto rfp = cell->getMoleculeCount("RFP");
        const auto yfp = cell->getMoleculeCount("YFP");

        // Store record
        writeRecord(iteration, totalTime.value(),
            cell->getId(), cell->getRadius(),
            rfp, gfp, yfp
        );
    }

    // Replace current objects
    std::swap(m_objects, objects);
}

/* ************************************************************************ */

#ifdef CECE_ENABLE_RENDER
void Module::draw(const simulator::Visualization& visualization, render::Context& context)
{
    // If visualization is disabled, do nothing
    if (!visualization.isEnabled(getLayerName()))
        return;

    if (!m_drawable)
        m_drawable.create(context);

    Assert(m_drawable);

    // Get current state
    const RenderState& state = m_drawableState.getFront();

    context.matrixPush();
    context.matrixTranslate(state.position);
    context.matrixScale(state.size / units::Length(1));
    context.colorPush();
    context.setColor(render::colors::WHITE);
    context.enableAlpha();
    m_drawable->draw(context, state.color, render::PrimitiveType::LineLoop);
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

    state.position = getPosition();
    state.size = getSize();
    state.color = getColor();
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

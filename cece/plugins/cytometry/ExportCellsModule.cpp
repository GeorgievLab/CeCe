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
#include "cece/plugins/fluorescence/ExportCellsModule.hpp"

// CeCe
#include "cece/core/Assert.hpp"
#include "cece/core/UnitIo.hpp"
#include "cece/simulator/Simulation.hpp"

// CeCe Plugins
#include "cece/plugins/cell/CellBase.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace fluorescence {

/* ************************************************************************ */

void ExportCellsModule::init()
{
    ExportModule::init();

    // Write output header
    writeHeader("iteration", "time", "cell-id", "x", "y", "rfp", "gfp", "yfp");
}

/* ************************************************************************ */

void ExportCellsModule::update()
{
    // Get simulation
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

        // Cast to cell
        const auto cell = object->cast<cell::CellBase>();
        Assert(cell);

        const auto gfp = cell->getMoleculeCount("GFP");
        const auto rfp = cell->getMoleculeCount("RFP");
        const auto yfp = cell->getMoleculeCount("YFP");

        // Store record
        writeRecord(iteration, totalTime.value(),
            cell->getId(), position.getX(), position.getY(),
            rfp, gfp, yfp
        );
    }
}

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */

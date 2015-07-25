/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* ************************************************************************ */

// Declaration
#include "StoreMolecules.hpp"

// Simulator
#include "core/DataTable.hpp"

// Plugin
#include "CellBase.hpp"

/* ************************************************************************ */

namespace plugin {
namespace cell {

/* ************************************************************************ */

void StoreMolecules::operator()(simulator::Object& object, simulator::Simulation& simulation, units::Duration)
{
    // Cast to cell
    auto& cell = object.castThrow<CellBase>("Cell object required");

    // Get data table
    auto& table = simulation.getDataTable("store-molecules");

    // Create new row
    // iteration;totalTime;id;molecules...
    table.addRow(
        makePair("iteration", simulation.getIteration()),
        makePair("totalTime", simulation.getTotalTime().value()),
        makePair("id", object.getId()),
        cell.getMolecules()
    );
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

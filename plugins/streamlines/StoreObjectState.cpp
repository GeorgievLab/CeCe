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
#include "StoreObjectState.hpp"

// Simulator
#include "core/DataTable.hpp"
#include "simulator/Simulation.hpp"

/* ************************************************************************ */

namespace plugin {
namespace streamlines {

/* ************************************************************************ */

void StoreObjectState::operator()(simulator::Object& object, simulator::Simulation& simulation, units::Duration)
{
    // Get data table
    auto& table = simulation.getDataTable("object-state");

    // Get states
    const auto pos = object.getPosition();
    const auto vel = object.getVelocity();

    // Create new row
    table.addRow(
        makePair("iteration", simulation.getIteration()),
        makePair("totalTime", simulation.getTotalTime().value()),
        makePair("id", object.getId()),
        makePair("x", pos.getX().value()),
        makePair("y", pos.getY().value()),
        makePair("velX", vel.getX().value()),
        makePair("velY", vel.getY().value())
    );
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

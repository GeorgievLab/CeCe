/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
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

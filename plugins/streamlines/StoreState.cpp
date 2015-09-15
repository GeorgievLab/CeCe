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
#include "StoreState.hpp"

// Simulator
#include "core/TimeMeasurement.hpp"
#include "core/Range.hpp"
#include "core/VectorRange.hpp"
#include "simulator/Simulation.hpp"

/* ************************************************************************ */

namespace plugin {
namespace streamlines {

/* ************************************************************************ */

void StoreState::update(units::Duration dt, simulator::Simulation& simulation)
{
    auto _ = measure_time("streamlines.store-state", simulator::TimeMeasurementIterationOutput(simulation));

    // Get data table
    auto& table = simulation.getDataTable("streamlines");

    // Get lattice
    const auto& lattice = m_streamlinesModule->getLattice();

    // Physical size of one lattice cell
    const auto dl = simulation.getWorldSize() / lattice.getSize();

    // Calculate maximum flow velocity
    const auto vMax = m_streamlinesModule->calculateMaxVelocity(dl);

    // Foreach coordinates
    for (auto&& c : range(lattice.getSize()))
    {
        // Normalized LB velocity
        const auto vel = vMax * lattice[c].calcVelocity();

        // Create new row
        table.addRow(
            makePair("iteration", simulation.getIteration()),
            makePair("totalTime", simulation.getTotalTime().value()),
            makePair("x", c.getX()),
            makePair("y", c.getY()),
            makePair("velX", vel.getX().value()),
            makePair("velY", vel.getY().value())
        );
    }
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

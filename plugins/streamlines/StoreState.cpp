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
    const auto v_max = LatticeData::MAX_SPEED * dl / dt;

    // Foreach coordinates
    for (auto&& c : range(lattice.getSize()))
    {
        // Normalized LB velocity
        const auto vel = v_max * lattice[c + 1].calcVelocityNormalized();

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

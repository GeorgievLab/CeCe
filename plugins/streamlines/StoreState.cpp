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

    // Modified time step
    const auto dtInner = m_streamlinesModule->getCoefficient() * dt;

    // Smooth time step
    const auto dtSmooth = dtInner / m_streamlinesModule->getIterations();

    // Calculate maximum flow velocity
    const auto vMax = LatticeData::MAX_SPEED * dl / dtSmooth;

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

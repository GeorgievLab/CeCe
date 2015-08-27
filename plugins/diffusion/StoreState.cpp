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
namespace diffusion {

/* ************************************************************************ */

void StoreState::update(units::Duration dt, simulator::Simulation& simulation)
{
    auto _ = measure_time("diffusion.store-state", simulator::TimeMeasurementIterationOutput(simulation));

    // Get data table
    auto& table = simulation.getDataTable("diffusion");

    // Foreach coordinates
    for (auto&& c : range(m_diffusionModule->getGridSize()))
    {
        // Create new row
        auto row = table.addRow(
            makePair("iteration", simulation.getIteration()),
            makePair("totalTime", simulation.getTotalTime().value()),
            makePair("x", c.getX()),
            makePair("y", c.getY())
        );

        // Foreach signals
        for (auto signalId : m_diffusionModule->getSignalIds())
        {
            table.setData(row,
                makePair(
                    m_diffusionModule->getSignalName(signalId),
                    m_diffusionModule->getSignal(signalId, c).value()
                )
            );
        }
    }
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */


/* ************************************************************************ */

// Declaration
#include "CellBase.hpp"

// Simulator
#include "core/Units.hpp"

/* ************************************************************************ */

namespace plugin {
namespace cell {

/* ************************************************************************ */

void CellBase::configure(const simulator::Configuration& config,
                         simulator::Simulation& simulation)
{
    Object::configure(config, simulation);

    // Initial volume
    setVolume(config.get("volume", getVolume()));

    // Growth rate
    setGrowthRate(config.get("growth-rate", getGrowthRate()));
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

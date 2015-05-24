
/* ************************************************************************ */

// Declaration
#include "CellBase.hpp"

// Simulator
#include "core/Units.hpp"
#include "parser/Parser.hpp"

/* ************************************************************************ */

namespace module {
namespace cell {

/* ************************************************************************ */

void CellBase::configure(const simulator::ConfigurationBase& config,
                         simulator::Simulation& simulation)
{
    Object::configure(config, simulation);

    config.callIfSetString("volume", [this](const std::string& value) {
		setVolume(parser::parse_value<core::units::Volume>(value));
    });
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

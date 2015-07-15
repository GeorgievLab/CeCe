
/* ************************************************************************ */

// Declaration
#include "CellBase.hpp"

// Simulator
#include "core/Units.hpp"
#include "parser/Parser.hpp"

/* ************************************************************************ */

namespace parser {

/* ************************************************************************ */

template<>
struct value_constructor<plugin::cell::CellBase::GrowthRate>
{
    static plugin::cell::CellBase::GrowthRate construct(float val, const String& suffix)
    {
        if (suffix.empty())
            return plugin::cell::CellBase::GrowthRate(val);

        if (suffix == "m3/s")
            return units::m3(val) / units::s(1);

        if (suffix == "mm3/s")
            return units::mm3(val) / units::s(1);

        if (suffix == "um3/s")
            return units::um3(val) / units::s(1);

        throw Exception("Unsupported suffix: " + suffix);
    }
};

/* ************************************************************************ */

}

/* ************************************************************************ */

namespace plugin {
namespace cell {

/* ************************************************************************ */

void CellBase::configure(const simulator::Configuration& config,
                         simulator::Simulation& simulation)
{
    Object::configure(config, simulation);

    // Initial volume
    config.callIfSetString("volume", [this](const String& value) {
        setVolume(parser::parse_value<units::Volume>(value));
    });

    // Growth rate
    config.callIfSetString("growth-rate", [this](const String& value) {
        setGrowthRate(parser::parse_value<GrowthRate>(value));
    });

    // Molecule initializations
    for (auto cfg : config.getConfigurations("molecule"))
    {
        auto name = cfg->getString("name");
        auto amount = cfg->getInteger("amount");

        // Set molecules
        setMoleculeCount(name, amount);
    }
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

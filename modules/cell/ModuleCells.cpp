
/* ************************************************************************ */

// Declaration
#include "ModuleCells.hpp"

// Simulator
#include "core/Units.hpp"
#include "simulator/Object.hpp"
#include "simulator/Simulation.hpp"
#include "simulator/Configuration.hpp"
#include "parser/Parser.hpp"

// Module
#include "Cell.hpp"
#include "Yeast.hpp"

/* ************************************************************************ */

namespace module {
namespace cell {

/* ************************************************************************ */

/**
 * @brief Basic cell initialization.
 *
 * @param obj
 * @param config
 */
static void baseInit(CellBase& obj, const simulator::ConfigurationBase& config)
{
    // Position
    {
        auto position = config.getString("position");

        if (!position.empty())
            obj.setPosition(parser::parse_vector<units::Length>(position));
    }

    // Volume
    {
        auto volume = config.getString("volume");

        if (!volume.empty())
            obj.setVolume(parser::parse_value<units::Volume>(volume));
    }

    // GFP
    {
        obj.setGfp(config.getInteger("gfp"));
    }

    // RFP
    {
        obj.setRfp(config.getInteger("rfp"));
    }

    // YFP
    {
        obj.setYfp(config.getInteger("yfp"));
    }
}

/* ************************************************************************ */

static std::unique_ptr<simulator::Object> createCell(simulator::Simulation& simulation, const simulator::ConfigurationBase& config)
{
    auto type = (config.getString("type") == "static") ? simulator::Object::Type::Static : simulator::Object::Type::Dynamic;

    auto ptr = std::unique_ptr<Cell>{new Cell{simulation, type}};
    baseInit(*ptr, config);

    return std::unique_ptr<simulator::Object>{ptr.release()};
}

/* ************************************************************************ */

static std::unique_ptr<simulator::Object> createYeast(simulator::Simulation& simulation, const simulator::ConfigurationBase& config)
{
    auto type = (config.getString("type") == "static") ? simulator::Object::Type::Static : simulator::Object::Type::Dynamic;

    auto ptr = std::unique_ptr<Yeast>{new Yeast{simulation, type}};
    baseInit(*ptr, config);

    return std::unique_ptr<simulator::Object>{ptr.release()};
}

/* ************************************************************************ */

ModuleCells::ModuleCells(simulator::Simulation& simulation)
{
    // Register builders
    simulation.registerObjectBuilder("Cell", &createCell);
    simulation.registerObjectBuilder("Yeast", &createYeast);
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

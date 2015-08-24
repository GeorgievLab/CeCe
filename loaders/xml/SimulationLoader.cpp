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
#include "SimulationLoader.hpp"

// Linux
#if __linux__
#include <libgen.h>
#endif

// pugixml
#include "pugixml/pugixml.hpp"

// Simulator
#include "core/Log.hpp"
#include "core/Units.hpp"
#include "core/FilePath.hpp"
#include "simulator/Simulation.hpp"
#include "simulator/PluginManager.hpp"

// Loader
#include "Configuration.hpp"

/* ************************************************************************ */

namespace loader {
namespace xml {

/* ************************************************************************ */

UniquePtr<simulator::Simulation> SimulationLoader::fromStream(
    InStream& source, const FilePath& filename) const
{
    auto simulation = makeUnique<simulator::Simulation>();

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load(source);

    if (!result)
        throw simulator::LoaderException("XML parse error: " + String(result.description()));

    {
        // Register file path as module library
#if __linux__
        char buffer[1024];
        strncpy(buffer, filename.c_str(), sizeof(buffer));

        simulator::PluginManager::addDirectory(dirname(buffer));
#endif
    }

    // Create configuration
    const simulator::Configuration config(
        makeUnique<loader::xml::ConfigImplementation>(doc.document_element()),
        filename
    );

    // Configure simulation
    simulation->configure(config);

    return simulation;
}

/* ************************************************************************ */

void SimulationLoader::toStream(OutStream& os, const simulator::Simulation& simulation, const FilePath& filename) const
{
    // TODO: implement
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

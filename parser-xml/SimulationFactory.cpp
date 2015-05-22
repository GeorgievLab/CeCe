
/* ************************************************************************ */

// Declaration
#include "parser-xml/SimulationFactory.hpp"

// C++
#include <cassert>
#include <cstring>
#include <map>

// Linux
#include <libgen.h>

// pugixml
#include "pugixml/pugixml.hpp"

// Simulator
#include "core/Log.hpp"
#include "parser/Parser.hpp"
#include "simulator/Library.hpp"

// Parser
#include "parser-xml/ImmutableConfiguration.hpp"

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

/**
 * @brief Process object node.
 *
 * @param node
 * @param simulation
 */
void process_object_node(const pugi::xml_node& node, simulator::Simulation& simulation, const std::string& filename)
{
    assert(!strcmp(node.name(), "object"));

    // Create configuration
    const parser::xml::ImmutableConfiguration configuration(node, filename);

    if (!configuration.hasValue("class"))
        throw parser::Exception("Missing attribute 'class' in 'object' element");

    bool isStatic = configuration.getString("type") == "static";

    // Create object
    simulator::Object* object = simulation.buildObject(configuration.getString("class"), !isStatic);

    if (object)
        object->configure(configuration, simulation);
}

/* ************************************************************************ */

/**
 * @brief Process module node.
 *
 * @param node
 * @param simulation
 */
void process_module_node(const pugi::xml_node& node, simulator::Simulation& simulation, const std::string& filename)
{
    assert(!strcmp(node.name(), "module"));

    // Create configuration
    const parser::xml::ImmutableConfiguration configuration(node, filename);

    // Module name
    {
        if (!configuration.hasValue("name"))
            throw parser::Exception("Missing attribute 'name' in 'module' element");

        // Create module by given name
        simulator::Module* module = simulation.useModule(configuration.getString("name"));

        // Configure module
        if (module)
            module->configure(configuration);
    }
}

/* ************************************************************************ */

/**
 * @brief Process simulation node.
 *
 * @param node
 * @param simulation
 */
void process_simulation_node(const pugi::xml_node& node, simulator::Simulation& simulation, const std::string& filename)
{
    assert(!strcmp(node.name(), "simulation"));

    // Resize world
    {
        auto size = parser::parse_vector<units::Length>(node.attribute("world-size").value());

        if (size.getWidth() == 0 || size.getHeight() == 0)
            throw parser::Exception("Width or height is zero!");

        simulation.setWorldSize(size);
    }

    // Time step
    {
        std::string dtStr = node.attribute("dt").value();

        // Real-time time step
        if (dtStr.empty() || dtStr == "auto")
        {
            simulation.setTimeStepRealTime(true);
        }
        else
        {
            // Parse time step
            auto dt = parser::parse_value<units::Duration>(dtStr);
            simulation.setTimeStep(dt);
            simulation.setTimeStepRealTime(false);
        }
    }

    // Number of iterations
    {
        auto attr = node.attribute("iterations");

        if (!attr.empty())
            simulation.setIterations(attr.as_ullong());
    }

    // Parse modules
    for (const auto& module : node.children("module"))
    {
        process_module_node(module, simulation, filename);
    }

    // Parse objects
    for (const auto& object : node.children("object"))
    {
        process_object_node(object, simulation, filename);
    }

}

/* ************************************************************************ */

}

/* ************************************************************************ */

namespace parser {
namespace xml {

/* ************************************************************************ */

std::unique_ptr<simulator::Simulation> SimulationFactory::fromStream(
    std::istream& source, const std::string& filename) const
{
    std::unique_ptr<simulator::Simulation> simulation(new simulator::Simulation());

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load(source);

    if (!result)
        throw Exception("XML parse error: " + std::string(result.description()));

    {
        char buffer[1024];
        strcpy(buffer, filename.c_str());

        // Register file path as module library
        simulator::Library::addLibraryPath(dirname(buffer));
    }

    // Parse DOM
    process_simulation_node(doc.document_element(), *simulation, filename);

    return simulation;
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

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

// C++
#include <cassert>
#include <cstring>

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
#include "simulator/Plugin.hpp"
#include "simulator/PluginApi.hpp"
#include "simulator/PluginManager.hpp"
#include "simulator/Object.hpp"
#include "simulator/Obstacle.hpp"
#include "simulator/Program.hpp"

// Parser
#include "Configuration.hpp"

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

/**
 * @brief Parse value attribute.
 *
 * @param object
 * @param name
 * @param value
 */
void parse_attribute_value(simulator::Object& object, const String& name, const String& value)
{
    static const String PREFIX = "value";

    // Not a value attribute
    if (name.substr(0, PREFIX.length()) != PREFIX || name.length() <= PREFIX.length())
        return;

    // Type specifier
    switch (name[PREFIX.length()])
    {
    case '.':
    {
        const auto START = PREFIX.length() + 1;

        // Find '-'
        auto pos = name.find('-', START);

        if (pos != String::npos)
        {
            // Value type
            const String type = name.substr(START, pos - PREFIX.length() - 1);

            if (type == "int")
                object.setValue(name.substr(pos + 1), std::stoi(value));
            else if (type == "float")
                object.setValue(name.substr(pos + 1), std::stof(value));
            else
                object.setValue(name.substr(pos + 1), value);
        }
        else
        {
            Log::warning("Invalid value attribute name: ", name);
        }

        break;
    }
    case '-':
        object.setValue(name.substr(PREFIX.size() + 1), value);
        break;

    default:
        Log::warning("Invalid value attribute name: ", name);
        break;
    }
}

/* ************************************************************************ */

/**
 * @brief Process parameter node.
 *
 * @param node
 * @param simulation
 */
void process_parameter_node(const pugi::xml_node& node, simulator::Simulation& simulation, const FilePath& filename)
{
    assert(!strcmp(node.name(), "parameter"));

    // Create configuration
    const simulator::Configuration config(makeUnique<loader::xml::ConfigImplementation>(node), filename);

    // Set parameter
    simulation.setParameter(config.get("name"), config.get<float>("value"));
}

/* ************************************************************************ */

/**
 * @brief Process init node.
 *
 * @param node
 * @param simulation
 */
void process_init_node(const pugi::xml_node& node, simulator::Simulation& simulation, const FilePath& filename)
{
    assert(!strcmp(node.name(), "init"));

    // Create configuration
    const simulator::Configuration config(makeUnique<loader::xml::ConfigImplementation>(node), filename);

    // Plugin is required
    auto api = simulation.requirePlugin(config.get("language"));

    // Register program
    simulation.addInitializer(api->createInitializer(simulation, config.getContent()));
}

/* ************************************************************************ */

/**
 * @brief Process program node.
 *
 * @param node
 * @param simulation
 */
void process_program_node(const pugi::xml_node& node, simulator::Simulation& simulation, const FilePath& filename)
{
    assert(!strcmp(node.name(), "program"));

    // Create configuration
    const simulator::Configuration config(makeUnique<loader::xml::ConfigImplementation>(node), filename);

    // Plugin is required
    auto api = simulation.requirePlugin(config.get("language"));

    // Program name
    auto name = config.get("name");

    // Register program
    simulation.addProgram(name, api->createProgram(simulation, name, config.getContent()));
}

/* ************************************************************************ */

/**
 * @brief Process object node.
 *
 * @param node
 * @param simulation
 */
void process_object_node(const pugi::xml_node& node, simulator::Simulation& simulation, const FilePath& filename)
{
    assert(!strcmp(node.name(), "object"));

    // Create configuration
    const simulator::Configuration config(makeUnique<loader::xml::ConfigImplementation>(node), filename);

    // Create object
    simulator::Object* object = simulation.buildObject(config.get("class"), config.get("type", String("dynamic")) != "static");

    if (object)
    {
        object->configure(config, simulation);

        // Set object values
        // TODO: remove
        for (const pugi::xml_attribute& attr : node.attributes())
        {
            parse_attribute_value(*object, attr.name(), attr.value());
        }
    }
}

/* ************************************************************************ */

/**
 * @brief Process obstacle node.
 *
 * @param node
 * @param simulation
 */
void process_obstacle_node(const pugi::xml_node& node, simulator::Simulation& simulation, const FilePath& filename)
{
    assert(!strcmp(node.name(), "obstacle"));

    // Create configuration
    const simulator::Configuration config(makeUnique<loader::xml::ConfigImplementation>(node), filename);

    // Create object
    simulator::Object* object = simulation.createObject<simulator::Obstacle>();

    if (object)
        object->configure(config, simulation);
}

/* ************************************************************************ */

/**
 * @brief Process module node.
 *
 * @param node
 * @param simulation
 */
void process_module_node(const pugi::xml_node& node, simulator::Simulation& simulation, const FilePath& filename)
{
    assert(!strcmp(node.name(), "module"));

    // Create configuration
    const simulator::Configuration config(makeUnique<loader::xml::ConfigImplementation>(node), filename);

    // Create module by given name
    simulator::Module* module = simulation.useModule(config.get("name"));

    // Configure module
    if (module)
        module->configure(config, simulation);
}

/* ************************************************************************ */

/**
 * @brief Process plugin node.
 *
 * @param node
 * @param simulation
 */
void process_plugin_node(const pugi::xml_node& node, simulator::Simulation& simulation, const FilePath& filename)
{
    assert(!strcmp(node.name(), "plugin"));

    // Create configuration
    const simulator::Configuration config(makeUnique<loader::xml::ConfigImplementation>(node), filename);

    // Load plugin
    simulation.requirePlugin(config.get("name"));
}

/* ************************************************************************ */

/**
 * @brief Process simulation node.
 *
 * @param node
 * @param simulation
 */
void process_simulation_node(const pugi::xml_node& node, simulator::Simulation& simulation, const FilePath& filename)
{
    assert(!strcmp(node.name(), "simulation"));

    // Create configuration
    const simulator::Configuration config(makeUnique<loader::xml::ConfigImplementation>(node), filename);

    // Resize world
    {
        auto size = config.get<SizeVector>("world-size");

        if (size.getWidth() == Zero || size.getHeight() == Zero)
            throw simulator::ConfigException("Width or height is zero!");

        simulation.setWorldSize(size);
    }

    // Time step
    {
        String dtStr = config.get("dt");

        // Real-time time step
        if (dtStr.empty() || dtStr == "auto")
        {
            simulation.setTimeStep(Zero);
        }
        else
        {
            // Parse time step
            simulation.setTimeStep(config.get<units::Time>("dt"));
        }
    }

    // Number of iterations
    {
        simulation.setIterations(config.get("iterations", simulation.getIterations()));
    }

    // Parse parameters
    for (const auto& parameter : node.children("parameter"))
    {
        process_parameter_node(parameter, simulation, filename);
    }

    // Parse init
    for (const auto& init : node.children("init"))
    {
        process_init_node(init, simulation, filename);
    }

    // Parse plugins
    for (const auto& plugin : node.children("plugin"))
    {
        process_plugin_node(plugin, simulation, filename);
    }

    // Parse modules
    for (const auto& module : node.children("module"))
    {
        process_module_node(module, simulation, filename);
    }

    // Parse programs
    for (const auto& program : node.children("program"))
    {
        process_program_node(program, simulation, filename);
    }

    // Parse objects
    for (const auto& object : node.children("object"))
    {
        process_object_node(object, simulation, filename);
    }

    // Parse obstacles
    for (const auto& obstacle : node.children("obstacle"))
    {
        process_obstacle_node(obstacle, simulation, filename);
    }

}

/* ************************************************************************ */

}

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
        strcpy(buffer, filename.c_str());

        simulator::PluginManager::addDirectory(dirname(buffer));
#endif
    }

    // Parse DOM
    process_simulation_node(doc.document_element(), *simulation, filename);

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

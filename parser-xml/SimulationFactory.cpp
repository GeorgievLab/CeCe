/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* ************************************************************************ */

// Declaration
#include "parser-xml/SimulationFactory.hpp"

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
#include "parser/Parser.hpp"
#include "simulator/Simulation.hpp"
#include "simulator/Plugin.hpp"
#include "simulator/PluginApi.hpp"
#include "simulator/Object.hpp"
#include "simulator/Obstacle.hpp"
#include "simulator/Program.hpp"

// Parser
#include "parser-xml/ImmutableConfiguration.hpp"

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
    const parser::xml::ImmutableConfiguration configuration(node, filename);

    // Module name
    {
        if (!configuration.hasValue("name"))
            throw parser::Exception("Missing attribute 'name' in 'parameter' element");

        if (!configuration.hasValue("value"))
            throw parser::Exception("Missing attribute 'value' in 'parameter' element");

        // Load plugin
        simulation.setParameter(configuration.getString("name"), configuration.getFloat("value"));
    }
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
    const parser::xml::ImmutableConfiguration configuration(node, filename);

    if (!configuration.hasValue("language"))
        throw parser::Exception("Missing attribute 'language' in 'init' element");

    const auto lang = configuration.getString("language");
    const auto code = configuration.getText();

    // Get library by language name
    auto api = simulation.loadPlugin(lang);

    if (!api)
        throw parser::Exception("Unable to load library for language '" + lang + "'");

    // Register program
    simulation.addInitializer(api->createInitializer(simulation, code));
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
    const parser::xml::ImmutableConfiguration configuration(node, filename);

    // Global name of the program
    if (!configuration.hasValue("name"))
        throw parser::Exception("Missing attribute 'name' in 'program' element");

    // Program language
    if (!configuration.hasValue("language"))
        throw parser::Exception("Missing attribute 'language' in 'program' element");

    // Program code
    if (!configuration.hasText())
        throw parser::Exception("Missing program element code");

    const auto name = configuration.getString("name");
    const auto lang = configuration.getString("language");
    const auto code = configuration.getText();

    // Get library by language name
    auto api = simulation.loadPlugin(lang);

    if (!api)
        throw parser::Exception("Unable to load library for language '" + lang + "'");

    // Register program
    simulation.addProgram(name, api->createProgram(simulation, name, code));
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
    const parser::xml::ImmutableConfiguration configuration(node, filename);

    if (!configuration.hasValue("class"))
        throw parser::Exception("Missing attribute 'class' in 'object' element");

    bool isStatic = configuration.getString("type") == "static";

    // Create object
    simulator::Object* object = simulation.buildObject(configuration.getString("class"), !isStatic);

    if (object)
    {
        object->configure(configuration, simulation);

        // Set object values
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
    const parser::xml::ImmutableConfiguration configuration(node, filename);

    // Create object
    simulator::Object* object = simulation.createObject<simulator::Obstacle>();

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
void process_module_node(const pugi::xml_node& node, simulator::Simulation& simulation, const FilePath& filename)
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
            module->configure(configuration, simulation);
    }
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
    const parser::xml::ImmutableConfiguration configuration(node, filename);

    // Module name
    {
        if (!configuration.hasValue("name"))
            throw parser::Exception("Missing attribute 'name' in 'plugin' element");

        // Load plugin
        simulation.requirePlugin(configuration.getString("name"));
    }
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

    // Resize world
    {
        auto size = parser::parse_vector<units::Length>(node.attribute("world-size").value());

        if (size.getWidth() == units::um(0) || size.getHeight() == units::um(0))
            throw parser::Exception("Width or height is zero!");

        simulation.setWorldSize(size);
    }

    // Time step
    {
        String dtStr = node.attribute("dt").value();

        // Real-time time step
        if (dtStr.empty() || dtStr == "auto")
        {
            simulation.setTimeStep(Zero);
        }
        else
        {
            // Parse time step
            auto dt = parser::parse_value<units::Duration>(dtStr);
            simulation.setTimeStep(dt);
        }
    }

    // Number of iterations
    {
        auto attr = node.attribute("iterations");

        if (!attr.empty())
            simulation.setIterations(attr.as_ullong());
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

namespace parser {
namespace xml {

/* ************************************************************************ */

UniquePtr<simulator::Simulation> SimulationFactory::fromStream(
    InStream& source, const FilePath& filename) const
{
    UniquePtr<simulator::Simulation> simulation(new simulator::Simulation());

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load(source);

    if (!result)
        throw Exception("XML parse error: " + String(result.description()));

    {
        // Register file path as module library
#if __linux__
        char buffer[1024];
        strcpy(buffer, filename.c_str());

        simulator::Plugin::addDirectory(dirname(buffer));
#endif
    }

    // Parse DOM
    process_simulation_node(doc.document_element(), *simulation, filename);

    return simulation;
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

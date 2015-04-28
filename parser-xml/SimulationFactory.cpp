
/* ************************************************************************ */

// Declaration
#include "parser-xml/SimulationFactory.hpp"

// C++
#include <cassert>
#include <map>

// pugixml
#include "pugixml/pugixml.hpp"

// Simulator
#include "core/Log.hpp"
#include "parser/Parser.hpp"

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

/**
 * @brief Read attributes from XML node.
 *
 * @param node
 *
 * @return
 */
std::map<std::string, std::string> get_attributes(const pugi::xml_node& node)
{
    // Get attributes
    std::map<std::string, std::string> res;

    // Store node attributes
    for (auto attr : node.attributes())
    {
        res.insert(std::make_pair(attr.name(), attr.value()));
    }

    return res;
}

/* ************************************************************************ */

/**
 * @brief Process object node.
 *
 * @param node
 * @param simulation
 */
void process_object_node(const pugi::xml_node& node, simulator::Simulation& simulation)
{
    assert(!strcmp(node.name(), "object"));

    // Get attributes
    auto attributes = get_attributes(node);

    // TODO: finish
}

/* ************************************************************************ */

/**
 * @brief Process module node.
 *
 * @param node
 * @param simulation
 */
void process_module_node(const pugi::xml_node& node, simulator::Simulation& simulation)
{
    assert(!strcmp(node.name(), "module"));

    // Get attributes
    auto attributes = get_attributes(node);

    // Module name
    {
        auto it = attributes.find("name");
        if (it == attributes.end())
            throw parser::Exception("Missing attribute 'name' in 'module' element");

        // Create module by given name
        simulator::Module* module = simulation.useModule(it->second);

        // Configure module
        if (module)
            module->configure(attributes);
    }
}

/* ************************************************************************ */

/**
 * @brief Process simulation node.
 *
 * @param node
 * @param simulation
 */
void process_simulation_node(const pugi::xml_node& node, simulator::Simulation& simulation)
{
    assert(!strcmp(node.name(), "simulation"));

    // Resize world
    {
        auto size = parser::parse_vector<units::Length>(node.attribute("world-size").value());

        if (size.getWidth() == 0 || size.getHeight() == 0)
            throw parser::Exception("Width or height is zero!");

        simulation.setWorldSize(size);
    }

    // Parse objects
    for (const auto& object : node.children("object"))
    {
        process_object_node(object, simulation);
    }

    // Parse modules
    for (const auto& module : node.children("module"))
    {
        process_module_node(module, simulation);
    }
}

/* ************************************************************************ */

}

/* ************************************************************************ */

namespace parser {
namespace xml {

/* ************************************************************************ */

std::unique_ptr<simulator::Simulation> SimulationFactory::fromStream(simulator::Simulator& simulator, std::istream& source) const
{
    std::unique_ptr<simulator::Simulation> simulation(new simulator::Simulation(simulator));

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load(source);

    if (!result)
        throw Exception("XML parse error: " + std::string(result.description()));

    // Parse DOM
    process_simulation_node(doc.document_element(), *simulation);

    return simulation;
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

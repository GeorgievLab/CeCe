
/* ************************************************************************ */

// Declaration
#include "parser-xml/SimulationFactory.hpp"

// pugixml
#include "pugixml/pugixml.hpp"

// Simulator
#include "parser/Parser.hpp"

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

/**
 * @brief Process simulation node.
 *
 * @param node
 * @param simulation
 */
void process_simulation_node(const pugi::xml_node& node, simulator::Simulation& simulation)
{
    // Resize world
    {
        auto size = parser::parse_vector<units::Length>(node.attribute("world-size").value());

        if (size.getWidth() == 0 || size.getHeight() == 0)
            throw parser::Exception("Width or height is zero!");

        simulation.setWorldSize(size);
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


/* ************************************************************************ */

// Declaration
#include "parser/Parser.hpp"

// C++
#include <string>
#include <istream>
#include <cstring>
#include <random>

// pugixml
#include "pugixml/pugixml.hpp"

// Simulator
#include "core/Units.hpp"
#include "core/Vector.hpp"
#include "simulator/Simulation.hpp"

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

template<typename T>
struct value_parser
{
    static T parse(const char* str, char*& end)
    {
        auto val = std::strtof(str, &end);

        // Possible suffixes
        if (!strncmp(end, "um", 2))
        {
            end += 2;
            return units::um(val);
        }
        else
        {
            return val;
        }
    }
};

/* ************************************************************************ */

template<typename T>
Vector<T> parse_vector(const char* value)
{
    char* end;
    auto x = value_parser<T>::parse(value, end);
    auto y = value_parser<T>::parse(end, end);

    return {x, y};
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
    // Resize world
    {
        auto size = parse_vector<units::Length>(node.attribute("world-size").value());

        if (size.getWidth() == 0 || size.getHeight() == 0)
            throw parser::Exception("Width or height is zero!");

        simulation.setWorldSize(size);
    }
}

/* ************************************************************************ */

}

/* ************************************************************************ */

namespace parser {

/* ************************************************************************ */

std::unique_ptr<simulator::Simulation> fromStream(std::istream& source)
{
    std::unique_ptr<simulator::Simulation> simulation(new simulator::Simulation());

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

/* ************************************************************************ */


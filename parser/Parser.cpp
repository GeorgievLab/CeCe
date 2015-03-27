
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
#include "simulator/World.hpp"
#include "simulator/Cell.hpp"

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

units::Length parse_length(const char* value)
{
    char* end;
    float val = std::strtof(value, &end);

    // Possible suffixes
    if (!strcmp(end, "um"))
    {
        return units::um(val);
    }
    else
    {
        return val;
    }
}

/* ************************************************************************ */

units::Volume parse_volume(const char* value)
{
    char* end;
    float val = std::strtof(value, &end);

    // Possible suffixes
    if (!strcmp(end, "um3"))
    {
        return units::um3(val);
    }
    else
    {
        return val;
    }
}

/* ************************************************************************ */

/**
 * @brief Process cell node.
 *
 * @param node
 * @param world
 */
void process_cell_node(const pugi::xml_node& node, simulator::World& world)
{
    // Create cell
    auto cell = world.createObject<simulator::Cell>();

    // Set cell volume
    cell->setVolume(parse_volume(node.attribute("volume").value()));

    // Set position
    cell->setPosition({
        parse_length(node.attribute("position-x").value()),
        parse_length(node.attribute("position-y").value())
    });

    // Set velocity
    cell->setVelocity({
        parse_length(node.attribute("velocity-x").value()),
        parse_length(node.attribute("velocity-y").value())
    });
}

/* ************************************************************************ */

/**
 * @brief Process world node.
 *
 * @param node
 * @param world
 */
void process_world_node(const pugi::xml_node& node, simulator::World& world)
{
    // Resize world
    {
        auto width = parse_length(node.attribute("width").value());
        auto height = parse_length(node.attribute("height").value());

        if (width == 0 || height == 0)
            throw parser::Exception("Width or height is zero!");

        world.resize(width, height);
    }

    // Resize grid
    {
        auto grid_width = node.attribute("grid-width").as_int(0);
        auto grid_height = node.attribute("grid-height").as_int(0);

        if (grid_width == 0 || grid_height == 0)
            throw parser::Exception("Grid width or height is zero!");

        world.getGrid().resize(grid_width, grid_height);

        auto& grid = world.getGrid();
        const auto R = units::um(100);
        const auto U = 10;

        for (decltype(grid.getWidth()) i = 0; i < grid.getWidth(); ++i)
        {
            for (decltype(grid.getHeight()) j = 0; j < grid.getHeight(); ++j)
            {
                auto& cell = grid(i, j);

                float x = i - grid.getWidth() / 2.f;
                float y = j - grid.getHeight() / 2.f;

                auto r2 = x * x + y * y;
                //auto theta = atan2(i2, j2);
                //auto ur = 1 * cos(theta) * (1 - R * R / (r * r));
                //auto ut = -1 * sin(theta) * (1 + R * R / (r * r));
                //auto u = ur * cos(theta) - ut * sin(theta);
                //auto v = ur * sin(theta) + ut * cos(theta);

                if (r2 <= R)
                    continue;

                cell.velocity.x = U * (1 + (R * R) / (r2) + 2 * (x * x * R * R) / (r2 * r2));
                cell.velocity.y = U * -2 * (R * R * x * y) / (r2 * r2);
            }
        }
    }

    for (const auto& child : node.children("cell"))
    {
        process_cell_node(child, world);
    }
}

/* ************************************************************************ */

}

/* ************************************************************************ */

namespace parser {

/* ************************************************************************ */

std::unique_ptr<simulator::World> fromStream(std::istream& source)
{
    std::unique_ptr<simulator::World> world(new simulator::World());

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load(source);

    if (!result)
        throw Exception("XML parse error: " + std::string(result.description()));

    // Parse DOM
    process_world_node(doc.document_element(), *world);

    return world;
}

/* ************************************************************************ */

}

/* ************************************************************************ */


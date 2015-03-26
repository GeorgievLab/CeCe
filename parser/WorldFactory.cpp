
/* ************************************************************************ */

// Declaration
#include "parser/WorldFactory.hpp"

// C++
#include <fstream>
#include <sstream>

// Simulator
#include "parser/Parser.hpp"

/* ************************************************************************ */

namespace parser {

/* ************************************************************************ */

std::unique_ptr<simulator::World> WorldFactory::fromFile(const std::string& filename) const
{
    std::ifstream file(filename, std::ios::in);

    return parser::fromStream(file);
}

/* ************************************************************************ */

std::unique_ptr<simulator::World> WorldFactory::fromSource(const std::string& source) const
{
    std::istringstream iss(source);

    return parser::fromStream(iss);
}

/* ************************************************************************ */

}

/* ************************************************************************ */

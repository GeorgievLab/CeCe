
/* ************************************************************************ */

// Declaration
#include "parser/SimulationFactory.hpp"

// C++
#include <fstream>
#include <sstream>

// Simulator
#include "parser/Parser.hpp"

/* ************************************************************************ */

namespace parser {

/* ************************************************************************ */

std::unique_ptr<simulator::Simulation> SimulationFactory::fromFile(const std::string& filename) const
{
    std::ifstream file(filename, std::ios::in);

    return parser::fromStream(file);
}

/* ************************************************************************ */

std::unique_ptr<simulator::Simulation> SimulationFactory::fromSource(const std::string& source) const
{
    std::istringstream iss(source);

    return parser::fromStream(iss);
}

/* ************************************************************************ */

}

/* ************************************************************************ */

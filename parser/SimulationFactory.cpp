
/* ************************************************************************ */

// Declaration
#include "parser/SimulationFactory.hpp"

// C++
#include <fstream>
#include <sstream>

/* ************************************************************************ */

namespace parser {

/* ************************************************************************ */

std::unique_ptr<simulator::Simulation> SimulationFactory::fromFile(const std::string& filename) const
{
    std::ifstream file(filename, std::ios::in);
    return fromStream(file, filename);
}

/* ************************************************************************ */

std::unique_ptr<simulator::Simulation> SimulationFactory::fromSource(
    const std::string& source, const std::string& filename) const
{
    std::istringstream iss(source);
    return fromStream(iss, filename);
}

/* ************************************************************************ */

}

/* ************************************************************************ */


/* ************************************************************************ */

// Declaration
#include "parser/SimulationFactory.hpp"

// C++
#include <fstream>
#include <sstream>

/* ************************************************************************ */

namespace parser {

/* ************************************************************************ */

std::unique_ptr<simulator::Simulation> SimulationFactory::fromFile(
    simulator::Simulator& simulator, const std::string& filename) const
{
    std::ifstream file(filename, std::ios::in);
    return fromStream(simulator, file, filename);
}

/* ************************************************************************ */

std::unique_ptr<simulator::Simulation> SimulationFactory::fromSource(
    simulator::Simulator& simulator, const std::string& source,
    const std::string& filename) const
{
    std::istringstream iss(source);
    return fromStream(simulator, iss, filename);
}

/* ************************************************************************ */

}

/* ************************************************************************ */

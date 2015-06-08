/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* ************************************************************************ */

// Declaration
#include "parser/SimulationFactory.hpp"

// C++
#include <fstream>
#include <sstream>

// Simulator
#include "simulator/Simulation.hpp"

/* ************************************************************************ */

namespace parser {

/* ************************************************************************ */

UniquePtr<simulator::Simulation> SimulationFactory::fromFile(const FilePath& filename) const
{
    std::ifstream file(filename, std::ios::in);
    return fromStream(file, filename);
}

/* ************************************************************************ */

UniquePtr<simulator::Simulation> SimulationFactory::fromSource(const String& source,
    const FilePath& filename) const
{
    std::istringstream iss(source);
    return fromStream(iss, filename);
}

/* ************************************************************************ */

}

/* ************************************************************************ */

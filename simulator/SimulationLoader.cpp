/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* ************************************************************************ */

// Declaration
#include "simulator/SimulationLoader.hpp"

// C++
#include <fstream>
#include <sstream>

// Simulator
#include "simulator/Simulation.hpp"

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

UniquePtr<Simulation> SimulationLoader::fromFile(const FilePath& filename) const
{
    std::ifstream file(filename.string(), std::ios::in);
    return fromStream(file, filename);
}

/* ************************************************************************ */

UniquePtr<Simulation> SimulationLoader::fromSource(const String& source, const FilePath& filename) const
{
    std::istringstream is(source, std::ios::in);
    return fromStream(is, filename);
}

/* ************************************************************************ */

void SimulationLoader::toFile(const Simulation& simulation, const FilePath& filename) const
{
    // Write code into file
    std::ofstream file(filename.string(), std::ios::out);
    toStream(file, simulation, filename);
}

/* ************************************************************************ */

String SimulationLoader::toSource(const Simulation& simulation, const FilePath& filename) const
{
    std::ostringstream os(std::ios::out);
    toStream(os, simulation, filename);
    return os.str();
}

/* ************************************************************************ */

}

/* ************************************************************************ */

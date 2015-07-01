/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* ************************************************************************ */

// Declaration
#include "simulator/SimulationFactory.hpp"

// C++
#include <fstream>

// Simulator
#include "simulator/Simulation.hpp"

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

UniquePtr<Simulation> SimulationFactory::createSimulation() const
{
    return makeUnique<Simulation>();
}

/* ************************************************************************ */

UniquePtr<Simulation> SimulationFactory::fromFile(const FilePath& filename) const
{
    String source;

    {
        std::ifstream file(filename, std::ios::in);

        String line;
        while (std::getline(file, line))
        {
            // Read source
            source.append(line);
        }
    }

    return fromSource(source);
}

/* ************************************************************************ */

}

/* ************************************************************************ */

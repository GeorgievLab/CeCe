
/* ************************************************************************ */

// Declaration
#include "simulator/WorldFactory.hpp"

// C++
#include <fstream>

// Simulator
#include "simulator/World.hpp"

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

std::unique_ptr<World> WorldFactory::createWorld() const
{
    return std::unique_ptr<World>(new World());
}

/* ************************************************************************ */

std::unique_ptr<World> WorldFactory::fromFile(const std::string& filename) const
{
    std::string source;

    {
        std::ifstream file(filename, std::ios::in);

        std::string line;
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

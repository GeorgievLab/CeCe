
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

std::unique_ptr<World> WorldFactory::createWorldFromFile(const std::string& filename) const
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

    return createWorldFromSource(source);
}

/* ************************************************************************ */

std::unique_ptr<World> WorldFactory::createWorldFromSource(const std::string& source) const
{
    auto world = createWorld();
    world->load(source);

    return world;
}

/* ************************************************************************ */

}

/* ************************************************************************ */

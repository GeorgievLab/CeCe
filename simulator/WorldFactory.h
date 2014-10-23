
#ifndef _SIMULATOR_WORLD_FACTORY_H_
#define _SIMULATOR_WORLD_FACTORY_H_

/* ************************************************************************ */

// C++
#include <memory>
#include <string>

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

class World;

/* ************************************************************************ */

/**
 * @brief WorldLoader interface.
 */
class WorldFactory
{

// Public Dtors
public:


    /**
     * @brief Virtual destructor.
     */
    virtual ~WorldFactory() {}


// Public Operations
public:


    /**
     * @brief Create a new world from source file.
     *
     * @param filename
     */
    virtual std::unique_ptr<World> createWorldFromFile(const std::string& filename) const;


    /**
     * @brief Create a new world from source code.
     *
     * @param source
     */
    virtual std::unique_ptr<World> createWorldFromSource(const std::string& source) const = 0;

};

/* ************************************************************************ */

}

/* ************************************************************************ */

#endif // _SIMULATOR_WORLD_FACTORY_H_

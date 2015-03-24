
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
     * @brief Create empty world.
     *
     * @return
     */
    virtual std::unique_ptr<World> createWorld() const;


    /**
     * @brief Create a new world from source file.
     *
     * @param filename
     */
    virtual std::unique_ptr<World> fromFile(const std::string& filename) const;


    /**
     * @brief Create a new world from source code.
     *
     * @param source
     */
    virtual std::unique_ptr<World> fromSource(const std::string& source) const = 0;

};

/* ************************************************************************ */

}

/* ************************************************************************ */

#endif // _SIMULATOR_WORLD_FACTORY_H_


#ifndef _LIBRARY_WORLD_LOADER_H_
#define _LIBRARY_WORLD_LOADER_H_

/* ************************************************************************ */

// C++
#include <memory>
#include <string>

/* ************************************************************************ */

class World;

/* ************************************************************************ */

/**
 * @brief WorldLoader interface.
 */
class WorldLoader
{
public:


    /**
     * @brief Create a new world from source file.
     *
     * @param filename
     */
    virtual std::unique_ptr<World> CreateWorldFromFile(const std::string& filename) const = 0;


    /**
     * @brief Create a new world from source code.
     *
     * @param source
     */
    virtual std::unique_ptr<World> CreateWorldFromSource(const std::string& source) const = 0;

};

/* ************************************************************************ */

#endif // _LIBRARY_WORLD_LOADER_H_

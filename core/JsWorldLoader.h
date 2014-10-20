
#ifndef _LIBRARY_JS_WORLD_LOADER_H_
#define _LIBRARY_JS_WORLD_LOADER_H_

/* ************************************************************************ */

// C++
#include <memory>
#include <string>

// Core
#include "WorldLoader.h"

/* ************************************************************************ */

class World;

/* ************************************************************************ */

/**
 * @brief WorldLoader for JavaScript (V8 engine)
 */
class JsWorldLoader : public WorldLoader
{
public:


    /**
     * @brief Create a new world from source file.
     *
     * @param filename
     */
    std::unique_ptr<World> CreateWorldFromFile(const std::string& filename) const override;


    /**
     * @brief Create a new world from source code.
     *
     * @param source
     */
    std::unique_ptr<World> CreateWorldFromSource(const std::string& source) const override;

};

/* ************************************************************************ */

#endif // _LIBRARY_JS_WORLD_LOADER_H_

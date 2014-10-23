
#ifndef _JAVASCRIPT_WORLD_FACTORY_H_
#define _JAVASCRIPT_WORLD_FACTORY_H_

/* ************************************************************************ */

// C++
#include <memory>
#include <string>

// Core
#include "simulator/WorldFactory.h"

/* ************************************************************************ */

namespace javascript {

/* ************************************************************************ */

/**
 * @brief WorldFactory for JavaScript (V8 engine)
 */
class WorldFactory : public simulator::WorldFactory
{
public:


    /**
     * @brief Create a new world from source code.
     *
     * @param source
     */
    std::unique_ptr<simulator::World> createWorldFromSource(const std::string& source) const override;

};

/* ************************************************************************ */

}

/* ************************************************************************ */

#endif // _JAVASCRIPT_WORLD_FACTORY_H_

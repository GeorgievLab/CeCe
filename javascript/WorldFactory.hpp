
#ifndef _JAVASCRIPT_WORLD_FACTORY_H_
#define _JAVASCRIPT_WORLD_FACTORY_H_

/* ************************************************************************ */

// C++
#include <memory>
#include <string>

// Core
#include "simulator/WorldFactory.hpp"

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
     * @brief Create empty world.
     *
     * @return
     */
    std::unique_ptr<simulator::World> createWorld() const override;

};

/* ************************************************************************ */

}

/* ************************************************************************ */

#endif // _JAVASCRIPT_WORLD_FACTORY_H_

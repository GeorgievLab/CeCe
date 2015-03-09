
/* ************************************************************************ */

// Declaration
#include "javascript/WorldFactory.hpp"

// Core
#include "javascript/World.hpp"

/* ************************************************************************ */

namespace javascript {

/* ************************************************************************ */

std::unique_ptr<simulator::World> WorldFactory::createWorld() const
{
    return std::unique_ptr<simulator::World>{new javascript::World{}};
}

/* ************************************************************************ */

}

/* ************************************************************************ */

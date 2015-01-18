
/* ************************************************************************ */

// Declaration
#include "javascript/WorldFactory.h"

// Core
#include "javascript/World.h"

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

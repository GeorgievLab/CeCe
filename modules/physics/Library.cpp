
/* ************************************************************************ */

// Declaration
#include "Library.hpp"

// Module
#include "Module.hpp"

/* ************************************************************************ */

DEFINE_LIBRARY_CREATE(simulation, name)
{
    return new module::physics::Module{};
}

/* ************************************************************************ */

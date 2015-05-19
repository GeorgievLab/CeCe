
/* ************************************************************************ */

// Declaration
#include "Library.hpp"

// Module
#include "Module.hpp"

/* ************************************************************************ */

DEFINE_LIBRARY_INIT(simulation)
{
    // Nothing to do
}

/* ************************************************************************ */

DEFINE_LIBRARY_FINALIZE(simulation)
{
    // Nothing to do
}

/* ************************************************************************ */

DEFINE_LIBRARY_CREATE_MODULE(simulation, name)
{
    return new module::streamlines::Module{};
}

/* ************************************************************************ */

DEFINE_LIBRARY_CREATE_OBJECT(simulation, name, flags)
{
    return nullptr;
}

/* ************************************************************************ */

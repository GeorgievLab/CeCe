
/* ************************************************************************ */

// Declaration
#include "Library.hpp"

// C++
#include <cstring>

// Module
#include "Module.hpp"
#include "Generator.hpp"

/* ************************************************************************ */

DEFINE_LIBRARY_CREATE(simulation, name)
{
    if (!strcmp(name, "generator"))
        return new module::cell::Generator{};

    return new module::cell::Module{};
}

/* ************************************************************************ */

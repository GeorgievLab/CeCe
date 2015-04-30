
/* ************************************************************************ */

// Declaration
#include "Library.hpp"

// C++
#include <cstring>

// Module
#include "Generator.hpp"
#include "ModuleCells.hpp"

/* ************************************************************************ */

DEFINE_LIBRARY_CREATE(simulation, name)
{
    if (!strcmp(name, "generator"))
        return new module::cell::Generator{};

    if (!strcmp(name, "cells"))
        return new module::cell::ModuleCells{*simulation};

    return nullptr;
}

/* ************************************************************************ */

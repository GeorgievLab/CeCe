
/* ************************************************************************ */

// Declaration
#include "Library.hpp"

// C++
#include <cstring>

// Simulator
#include "simulator/Simulation.hpp"

// Module
#include "Module.hpp"
#include "Generator.hpp"

/* ************************************************************************ */

DEFINE_LIBRARY_CREATE(simulation, name)
{
    if (!strcmp(name, "generator"))
        return new module::diffusion::Generator{simulation->useModule<module::diffusion::Module>("diffusion")};

    return new module::diffusion::Module{};
}

/* ************************************************************************ */


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
#include "GeneratorCell.hpp"

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
    if (!strcmp(name, "generator"))
        return new module::diffusion::Generator{simulation->useModule<module::diffusion::Module>("diffusion")};

    if (!strcmp(name, "generator-cell"))
        return new module::diffusion::GeneratorCell{simulation->useModule<module::diffusion::Module>("diffusion")};

    return new module::diffusion::Module{};
}

/* ************************************************************************ */

DEFINE_LIBRARY_CREATE_OBJECT(simulation, name, flags)
{
    return nullptr;
}

/* ************************************************************************ */

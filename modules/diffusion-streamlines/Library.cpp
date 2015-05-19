
/* ************************************************************************ */

// Declaration
#include "Library.hpp"

// Simulator
#include "simulator/Simulation.hpp"

// Module
#include "Module.hpp"

// Modules
#include "../diffusion/Module.hpp"
#include "../streamlines/Module.hpp"

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
    return new module::diffusion_streamlines::Module{
        simulation->useModule<module::diffusion::Module>("diffusion"),
        simulation->useModule<module::streamlines::Module>("streamlines")
    };
}

/* ************************************************************************ */

DEFINE_LIBRARY_CREATE_OBJECT(simulation, name, flags)
{
    return nullptr;
}

/* ************************************************************************ */

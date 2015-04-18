
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

DEFINE_LIBRARY_CREATE(simulation, name)
{
    return new module::diffusion_streamlines::Module{
        simulation->useModule<module::diffusion::Module>("diffusion"),
        simulation->useModule<module::streamlines::Module>("streamlines")
    };
}

/* ************************************************************************ */

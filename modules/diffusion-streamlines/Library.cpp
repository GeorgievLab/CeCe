
/* ************************************************************************ */

// Declaration
#include "Library.hpp"

// Simulator
#include "simulator/Library.hpp"
#include "simulator/LibraryApi.hpp"
#include "simulator/Simulation.hpp"

// Module
#include "Module.hpp"

// Modules
#include "../diffusion/Module.hpp"
#include "../streamlines/Module.hpp"

/* ************************************************************************ */

class DiffusionStreamlinesApi : public simulator::LibraryApi
{
    std::unique_ptr<simulator::Module> createModule(simulator::Simulation& simulation, const std::string& name) noexcept override
    {
        return std::unique_ptr<simulator::Module>(new module::diffusion_streamlines::Module{
            simulation.useModule<module::diffusion::Module>("diffusion"),
            simulation.useModule<module::streamlines::Module>("streamlines")
        });
    }
};

/* ************************************************************************ */

DEFINE_LIBRARY(diffusion_cylinder_streamlines, DiffusionStreamlinesApi)

/* ************************************************************************ */

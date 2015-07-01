
/* ************************************************************************ */

// Simulator
#include "core/compatibility.hpp"
#include "simulator/Plugin.hpp"
#include "simulator/PluginApi.hpp"
#include "simulator/Simulation.hpp"

// Module
#include "Module.hpp"

// Modules
#include "../diffusion/Module.hpp"
#include "../cylinder-streamlines/Module.hpp"

/* ************************************************************************ */

class DiffusionCylinderStreamlinesApi : public simulator::PluginApi
{
    std::unique_ptr<simulator::Module> createModule(simulator::Simulation& simulation, const std::string& name) NOEXCEPT override
    {
        return std::unique_ptr<simulator::Module>(new plugin::diffusion_cylinder_streamlines::Module{
            simulation.useModule<plugin::diffusion::Module>("diffusion"),
            simulation.useModule<plugin::cylinder_streamlines::Module>("cylinder-streamlines")
        });
    }
};

/* ************************************************************************ */

DEFINE_LIBRARY(diffusion_cylinder_streamlines, DiffusionCylinderStreamlinesApi)

/* ************************************************************************ */

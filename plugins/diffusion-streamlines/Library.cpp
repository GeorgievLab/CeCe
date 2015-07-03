/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */

// Simulator
#include "core/compatibility.hpp"
#include "simulator/Plugin.hpp"
#include "simulator/PluginApi.hpp"
#include "simulator/Simulation.hpp"

// Module
#include "Module.hpp"

// Modules
#include "plugins/diffusion/Module.hpp"
#include "plugins/streamlines/Module.hpp"

/* ************************************************************************ */

class DiffusionStreamlinesApi : public simulator::PluginApi
{
    UniquePtr<simulator::Module> createModule(simulator::Simulation& simulation, const String& name) NOEXCEPT override
    {
        return makeUnique<plugin::diffusion_streamlines::Module>(
            simulation.useModule<plugin::diffusion::Module>("diffusion"),
            simulation.useModule<plugin::streamlines::Module>("streamlines")
        );
    }
};

/* ************************************************************************ */

DEFINE_PLUGIN(diffusion_streamlines, DiffusionStreamlinesApi)

/* ************************************************************************ */

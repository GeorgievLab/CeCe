/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* ************************************************************************ */

// Simulator
#include "simulator/Plugin.hpp"
#include "simulator/PluginApi.hpp"
#include "simulator/Simulation.hpp"

// Plugin
#include "Module.hpp"

// Plugins
#include "plugins/diffusion/Module.hpp"
#include "plugins/streamlines/Module.hpp"

/* ************************************************************************ */

using namespace simulator;

/* ************************************************************************ */

class DiffusionStreamlinesApi : public PluginApi
{
    UniquePtr<Module> createModule(Simulation& simulation, const String& name) noexcept override
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

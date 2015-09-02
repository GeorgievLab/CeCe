/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */

// Simulator
#include "simulator/Simulation.hpp"
#include "simulator/Plugin.hpp"
#include "simulator/PluginApi.hpp"

// Plugin
#include "Module.hpp"
#include "Generator.hpp"
#include "StoreState.hpp"

/* ************************************************************************ */

using namespace simulator;

/* ************************************************************************ */

class DiffusionApi : public PluginApi
{
    UniquePtr<Module> createModule(Simulation& simulation, const String& name) noexcept override
    {
        if (name == "store-state")
            return makeUnique<plugin::diffusion::StoreState>(simulation.useModule<plugin::diffusion::Module>("diffusion"));
        else if (name == "generator")
            return makeUnique<plugin::diffusion::Generator>(simulation.useModule<plugin::diffusion::Module>("diffusion"));

        return makeUnique<plugin::diffusion::Module>();
    }
};

/* ************************************************************************ */

DEFINE_PLUGIN(diffusion, DiffusionApi)

/* ************************************************************************ */

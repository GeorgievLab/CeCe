/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */

// Simulator
#include "core/compatibility.hpp"
#include "simulator/Simulation.hpp"
#include "simulator/Plugin.hpp"
#include "simulator/PluginApi.hpp"

// Plugin
#include "wrapper_module.hpp"

/* ************************************************************************ */

using namespace simulator;

/* ************************************************************************ */

class DiffusionPythonApi : public PluginApi
{
    void initSimulation(Simulation& simulation) override
    {
        simulation.requirePlugin("python");
        python_wrapper_module();
    }
};

/* ************************************************************************ */

DEFINE_PLUGIN(diffusion_python, DiffusionPythonApi)

/* ************************************************************************ */

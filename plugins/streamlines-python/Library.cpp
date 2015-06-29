/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */

// Declaration
#include "simulator/Library.hpp"

// Simulator
#include "core/compatibility.hpp"
#include "simulator/Library.hpp"
#include "simulator/Simulation.hpp"
#include "simulator/PluginApi.hpp"

// Plugin
#include "wrapper_module.hpp"

/* ************************************************************************ */

using namespace simulator;

/* ************************************************************************ */

class StreamlinesPythonApi : public PluginApi
{
    void initSimulation(Simulation& simulation) override
    {
        // TODO: change to 'requirePlugin("python")'
        simulation.useModule("python");

        python_wrapper_module();
    }
};

/* ************************************************************************ */

DEFINE_LIBRARY(streamlines_python, StreamlinesPythonApi)

/* ************************************************************************ */

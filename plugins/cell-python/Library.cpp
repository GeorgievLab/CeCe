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
#include "wrapper_CellBase.hpp"
#include "wrapper_Yeast.hpp"

/* ************************************************************************ */

using namespace simulator;

/* ************************************************************************ */

class CellPythonApi : public PluginApi
{
    void initSimulation(Simulation& simulation) override
    {
        // Require python plugin
        simulation.requirePlugin("python");

        python_wrapper_cell_CellBase();
        python_wrapper_cell_Yeast();
    }
};

/* ************************************************************************ */

DEFINE_PLUGIN(cell_python, CellPythonApi)

/* ************************************************************************ */

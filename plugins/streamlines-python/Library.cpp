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
#include "simulator/PluginApi.hpp"

// Python
#include "../python/Python.hpp"

/* ************************************************************************ */

using namespace simulator;

/* ************************************************************************ */

class StreamlinesPythonApi : public PluginApi
{
    void initSimulation(Simulation& simulation) override
    {
        // Nothing to do
    }
};

/* ************************************************************************ */

DEFINE_LIBRARY(streamlines_python, StreamlinesPythonApi)

/* ************************************************************************ */

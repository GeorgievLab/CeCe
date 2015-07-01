/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */

// Simulator
#include "core/compatibility.hpp"
#include "core/UniquePtr.hpp"
#include "core/String.hpp"
#include "simulator/Plugin.hpp"
#include "simulator/PluginApi.hpp"

// Module
#include "Module.hpp"

/* ************************************************************************ */

using namespace simulator;

/* ************************************************************************ */

class StreamlinesApi : public PluginApi
{
    UniquePtr<Module> createModule(Simulation& simulation, const String& name) NOEXCEPT override
    {
        return makeUnique<plugin::streamlines::Module>();
    }
};

/* ************************************************************************ */

DEFINE_PLUGIN(streamlines, StreamlinesApi)

/* ************************************************************************ */

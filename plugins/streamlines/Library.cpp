/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* ************************************************************************ */

// Declaration
#include "Library.hpp"

// Simulator
#include "core/compatibility.hpp"
#include "core/UniquePtr.hpp"
#include "core/String.hpp"
#include "simulator/Library.hpp"
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
        return makeUniquePtr<module::streamlines::Module>();
    }
};

/* ************************************************************************ */

DEFINE_LIBRARY(streamlines, StreamlinesApi)

/* ************************************************************************ */

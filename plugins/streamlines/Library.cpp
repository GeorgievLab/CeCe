/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* Author: Hynek Kasl <hkasl@students.zcu.cz>                               */
/* ************************************************************************ */

// Simulator
#include "core/UniquePtr.hpp"
#include "core/String.hpp"
#include "simulator/Plugin.hpp"
#include "simulator/PluginApi.hpp"

// Plugin
#include "Module.hpp"
#include "StoreState.hpp"
#include "StoreObjectState.hpp"

/* ************************************************************************ */

using namespace simulator;

/* ************************************************************************ */

class StreamlinesApi : public PluginApi
{
    UniquePtr<Module> createModule(Simulation& simulation, const String& name) noexcept override
    {
        if (name == "store-state")
            return makeUnique<plugin::streamlines::StoreState>(simulation.useModule<plugin::streamlines::Module>("streamlines"));

        return makeUnique<plugin::streamlines::Module>();
    }


    /**
     * @brief Create program from current library.
     *
     * @param simulation Simulation for that module is created.
     * @param name       Program name.
     * @param code       Optional program code.
     *
     * @return Created program.
     */
    Program createProgram(Simulation& simulation, const String& name, String code = {}) override
    {
        if (name == "store-object-state")
            return plugin::streamlines::StoreObjectState(simulation.useModule<plugin::streamlines::Module>("streamlines"));

        return {};
    }

};

/* ************************************************************************ */

DEFINE_PLUGIN(streamlines, StreamlinesApi)

/* ************************************************************************ */

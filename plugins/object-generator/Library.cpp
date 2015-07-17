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
#include "simulator/Simulation.hpp"
#include "simulator/Plugin.hpp"
#include "simulator/PluginApi.hpp"

// Plugin
#include "Module.hpp"

/* ************************************************************************ */

using namespace simulator;

/* ************************************************************************ */

class ObjectGeneratorApi : public PluginApi
{
    UniquePtr<Module> createModule(Simulation& simulation, const String& name) noexcept override
    {
        return makeUnique<plugin::object_generator::Module>();
    }

};

/* ************************************************************************ */

DEFINE_PLUGIN(object_generator, ObjectGeneratorApi)

/* ************************************************************************ */

/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/*                                                                          */
/* This file is part of CeCe.                                               */
/*                                                                          */
/* CeCe is free software: you can redistribute it and/or modify             */
/* it under the terms of the GNU General Public License as published by     */
/* the Free Software Foundation, either version 3 of the License, or        */
/* (at your option) any later version.                                      */
/*                                                                          */
/* CeCe is distributed in the hope that it will be useful,                  */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of           */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            */
/* GNU General Public License for more details.                             */
/*                                                                          */
/* You should have received a copy of the GNU General Public License        */
/* along with CeCe.  If not, see <http://www.gnu.org/licenses/>.            */
/*                                                                          */
/* ************************************************************************ */

// This must be first
#include "cece/plugins/python/Python.hpp"

// CeCe
#include "cece/core/Log.hpp"
#include "cece/core/Exception.hpp"
#include "cece/simulator/Plugin.hpp"
#include "cece/simulator/PluginApi.hpp"
#include "cece/simulator/PluginManager.hpp"
#include "cece/simulator/SimulationListener.hpp"
#include "cece/simulator/ModuleFactoryManager.hpp"

// Plugin
#include "cece/plugins/python/Module.hpp"
#include "cece/plugins/python/Object.hpp"
#include "cece/plugins/python/Program.hpp"
#include "cece/plugins/python/Initializer.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace python {

/* ************************************************************************ */

void init_stdout(void);
void init_core(void);
void init_simulator(void);

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */

/**
 * @brief Python modules initialization table.
 */
static const struct _inittab INIT_TABLE[] = {
    {const_cast<char*>("cppout"),    cece::plugin::python::init_stdout},
    {const_cast<char*>("core"),      cece::plugin::python::init_core},
    {const_cast<char*>("simulator"), cece::plugin::python::init_simulator},
    {NULL, NULL}
};

/* ************************************************************************ */

using namespace cece;
using namespace cece::simulator;

/* ************************************************************************ */

class PythonApi : public PluginApi, public SimulationListener
{

    /**
     * @brief On plugin load.
     */
    void onLoad() override
    {
        ModuleFactoryManager::s().createForModule<plugin::python::Module>("python");
    }


    /**
     * @brief On plugin unload.
     */
    void onUnload() override
    {
        ModuleFactoryManager::s().remove("python");
    }


    /**
     * @brief Initialize simulation for Python interpreter.
     *
     * @param simulation Simulation.
     */
    void initSimulation(Simulation& simulation) noexcept override
    {
        if (PyImport_ExtendInittab(const_cast<struct _inittab*>(INIT_TABLE)) != 0)
            throw std::runtime_error("Unable to initialize Python import table");

        // Initialize Python interpreter
        Py_Initialize();

        // Register listener
        simulation.addListener(this);
    }


    /**
     * @brief Finalize simulation.
     *
     * @param simulation Simulation.
     */
    void finalizeSimulation(Simulation& simulation) noexcept override
    {
        Py_Finalize();
    }


    /**
     * @brief Create initializer.
     *
     * @param simulation Simulation for that module is created.
     * @param code       Program code.
     *
     * @return Created initializer.
     */
    Simulation::Initializer createInitializer(Simulation& simulation, String code) override
    {
        plugin::python::Initializer init;
        init.initSource(code);
        return init;
    }


    /**
     * @brief Create program from current library.
     *
     * @param simulation Simulation for that module is created.
     * @param name       Object name.
     * @param dynamic    If object should be dynamic.
     *
     * @return Created object.
     */
    Program createProgram(Simulation& simulation, const String& name, String code = {}) noexcept override
    {
        try
        {
            plugin::python::Program program;

            if (code.empty())
            {
                program.initFile(name);
            }
            else
            {
                program.initSource(code);
            }

            return program;
        }
        catch (const std::exception& e)
        {
            Log::warning(e.what());
        }

        return {};
    }


// Public Operations
public:


    /**
     * @brief New plugin is loaded.
     *
     * @param simulation Current simulation.
     * @param name       Plugin name.
     */
    void onPluginLoad(Simulation& simulation, const String& name) override
    {
        // Plugin name contains python suffix - do not load
        if (name.find("-python") != String::npos)
            return;

        const String pluginName = name + "-python";

        Log::debug("[python] Trying to load plugin: ", pluginName);

        // Test if plugin with suffix -python exists.
        if (PluginManager::s().isAvailable(pluginName))
            simulation.loadPlugin(pluginName);
    }

};

/* ************************************************************************ */

DEFINE_PLUGIN(python, PythonApi)

/* ************************************************************************ */

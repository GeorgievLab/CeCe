/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */

// Python requires to be included first because it sets some parameters to stdlib
#include "Python.hpp"

/* ************************************************************************ */

// Simulator
#include "core/Log.hpp"
#include "core/Exception.hpp"
#include "simulator/Plugin.hpp"
#include "simulator/PluginApi.hpp"
#include "simulator/PluginManager.hpp"
#include "simulator/SimulationListener.hpp"

// Module
#include "Module.hpp"
#include "Object.hpp"
#include "Program.hpp"
#include "Initializer.hpp"

// Wrappers
#include "wrapper_stdout.hpp"
#include "wrapper_core.hpp"
#include "wrapper_render.hpp"
#include "wrapper_parser.hpp"
#include "wrapper_simulator.hpp"

/* ************************************************************************ */

/**
 * @brief Python modules initialization table.
 */
static const struct _inittab INIT_TABLE[] = {
    {const_cast<char*>("cppout"), python_wrapper_stdout},
    {const_cast<char*>("core"), python_wrapper_core},
    {const_cast<char*>("simulator"), python_wrapper_simulator},
    {const_cast<char*>("render"), python_wrapper_render},
    {const_cast<char*>("parser"), python_wrapper_parser},
    {NULL, NULL}
};

/* ************************************************************************ */

using namespace simulator;

/* ************************************************************************ */

class PythonApi : public PluginApi, public SimulationListener
{

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
        try
        {
            plugin::python::Initializer init;
            init.initSource(code);

            return init;
        }
        catch (const Exception& e)
        {
            Log::warning(e.what());
        }

        return {};
    }


    /**
     * @brief Create module from current library.
     *
     * @param simulation Simulation for that module is created.
     * @param name       Module name.
     *
     * @return Created module.
     */
    std::unique_ptr<Module> createModule(Simulation& simulation, const String& name) noexcept override
    {
        try
        {
            return std::unique_ptr<Module>(new plugin::python::Module{name});
        }
        catch (const std::exception& e)
        {
            Log::warning(e.what());
        }

        return nullptr;
    }


    /**
     * @brief Create object from current library.
     *
     * @param simulation Simulation for that module is created.
     * @param name       Object name.
     * @param dynamic    If object should be dynamic.
     *
     * @return Created object.
     */
    std::unique_ptr<Object> createObject(Simulation& simulation, const String& name, Object::Type type = Object::Type::Dynamic) noexcept override
    {
        try
        {
            return std::unique_ptr<Object>(new plugin::python::Object{simulation, name});
        }
        catch (const std::exception& e)
        {
            Log::warning(e.what());
        }

        return nullptr;
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
        if (PluginManager::isAvailable(pluginName))
            simulation.loadPlugin(pluginName);
    }

};

/* ************************************************************************ */

DEFINE_PLUGIN(python, PythonApi)

/* ************************************************************************ */

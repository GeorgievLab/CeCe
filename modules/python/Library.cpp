
/* ************************************************************************ */

// Declaration
#include "Library.hpp"

// Simulator
#include "core/Log.hpp"

// Simulator
#include "simulator/Library.hpp"
#include "simulator/LibraryApi.hpp"

// Module
#include "Module.hpp"
#include "Object.hpp"
#include "Program.hpp"

// Python
#include <Python.h>

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

class PythonApi : public LibraryApi
{

    /**
     * @brief Initialize simulation for Python interpreter.
     *
     * @param simulation Simulation.
     */
    void initSimulation(Simulation& simulation) NOEXCEPT override
    {
        if (PyImport_ExtendInittab(const_cast<struct _inittab*>(INIT_TABLE)) != 0)
            throw std::runtime_error("Unable to initialize Python import table");

        // Initialize Python interpreter
        Py_Initialize();
    }


    /**
     * @brief Finalize simulation.
     *
     * @param simulation Simulation.
     */
    void finalizeSimulation(Simulation& simulation) NOEXCEPT override
    {
        Py_Finalize();
    }


    /**
     * @brief Create module from current library.
     *
     * @param simulation Simulation for that module is created.
     * @param name       Module name.
     *
     * @return Created module.
     */
    std::unique_ptr<Module> createModule(Simulation& simulation, const std::string& name) NOEXCEPT override
    {
        try
        {
            return std::unique_ptr<Module>(new module::python::Module{name});
        }
        catch (const std::exception& e)
        {
            core::Log::warning(e.what());
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
    std::unique_ptr<Object> createObject(Simulation& simulation, const std::string& name, bool dynamic = true) NOEXCEPT override
    {
        try
        {
            return std::unique_ptr<Object>(new module::python::Object{simulation, name});
        }
        catch (const std::exception& e)
        {
            core::Log::warning(e.what());
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
    Program createProgram(Simulation& simulation, const std::string& name, std::string code = {}) NOEXCEPT override
    {
        try
        {
            module::python::Program program;

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
            core::Log::warning(e.what());
        }

        return {};
    }

};

/* ************************************************************************ */

DEFINE_LIBRARY(python, PythonApi)

/* ************************************************************************ */

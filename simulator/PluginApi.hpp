/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* ************************************************************************ */

#pragma once

/* ************************************************************************ */

// Simulator
#include "core/compatibility.hpp"
#include "core/UniquePtr.hpp"
#include "core/String.hpp"
#include "simulator/Program.hpp"
#include "simulator/Module.hpp"
#include "simulator/Object.hpp"

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

class Simulation;

/* ************************************************************************ */

/**
 * @brief Library API type.
 */
class DLL_EXPORT PluginApi
{

// Public Ctors & Dtors
public:


    /**
     * @brief Destructor.
     */
    virtual ~PluginApi()
    {
        // Nothing to do
    }


// Public Operations
public:


    /**
     * @brief Init simulation.
     *
     * @param simulation Simulation.
     */
    virtual void initSimulation(Simulation& simulation)
    {
        // Nothing to do
    }


    /**
     * @brief Finalize simulation.
     *
     * @param simulation Simulation.
     */
    virtual void finalizeSimulation(Simulation& simulation)
    {
        // Nothing to do
    }


    /**
     * @brief Create module from current library.
     *
     * @param simulation Simulation for that module is created.
     * @param name       Module name.
     *
     * @return Created module.
     */
    virtual UniquePtr<Module> createModule(Simulation& simulation, const String& name)
    {
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
    virtual UniquePtr<Object> createObject(Simulation& simulation, const String& name, bool dynamic = true)
    {
        return nullptr;
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
    virtual Program createProgram(Simulation& simulation, const String& name, String code = {})
    {
        return {};
    }

};

/* ************************************************************************ */

}

/* ************************************************************************ */

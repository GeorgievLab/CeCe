/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
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
#include "core/UniquePtr.hpp"
#include "core/String.hpp"
#include "simulator/Program.hpp"
#include "simulator/Module.hpp"
#include "simulator/Object.hpp"
#include "simulator/Simulation.hpp"
#include "simulator/Configuration.hpp"

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

/**
 * @brief Library API type.
 */
class PluginApi
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
     * @brief Configure plugin.
     *
     * @param simulation Current simulation.
     * @param config     Plugin configuration.
     */
    virtual void configure(Simulation& simulation, const Configuration& config)
    {
        // Nothing to do
    }


    /**
     * @brief Create initializer.
     *
     * @param simulation Simulation for that module is created.
     * @param code       Program code.
     *
     * @return Created initializer.
     */
    virtual Simulation::Initializer createInitializer(Simulation& simulation, String code)
    {
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
    virtual UniquePtr<Module> createModule(Simulation& simulation, const String& name)
    {
        return nullptr;
    }


    /**
     * @brief Create object from current library.
     *
     * @param simulation Simulation for that module is created.
     * @param name       Object name.
     * @param type       Object type.
     *
     * @return Created object.
     */
    virtual UniquePtr<Object> createObject(Simulation& simulation, const String& name, Object::Type type = Object::Type::Dynamic)
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


#pragma once

/* ************************************************************************ */

// C++
#include <string>
#include <memory>

// Simulator
#include "core/compatibility.hpp"
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
class LibraryApi
{

// Public Ctors & Dtors
public:


    /**
     * @brief Destructor.
     */
    virtual ~LibraryApi()
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
    virtual void initSimulation(Simulation& simulation) NOEXCEPT
    {
        // Nothing to do
    }


    /**
     * @brief Finalize simulation.
     *
     * @param simulation Simulation.
     */
    virtual void finalizeSimulation(Simulation& simulation) NOEXCEPT
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
    virtual std::unique_ptr<Module> createModule(Simulation& simulation, const std::string& name) NOEXCEPT
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
    virtual std::unique_ptr<Object> createObject(Simulation& simulation, const std::string& name, bool dynamic = true) NOEXCEPT
    {
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
    virtual Program createProgram(Simulation& simulation, const std::string& name) NOEXCEPT
    {
        return {};
    }

};

/* ************************************************************************ */

}

/* ************************************************************************ */

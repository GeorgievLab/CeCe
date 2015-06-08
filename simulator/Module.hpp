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
#include "core/Units.hpp"

#ifdef ENABLE_RENDER
namespace render { class Context; }
#endif

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

class Simulation;
class Configuration;

/* ************************************************************************ */

/**
 * @brief Abstract class for simulation modules.
 */
class DLL_EXPORT Module
{


// Public Ctors & Dtors
public:


    /**
     * @brief Destructor.
     */
    virtual ~Module()
    {
        // Nothing to do
    }


// Public Operations
public:


    /**
     * @brief Update module state.
     *
     * @param dt         Simulation time step.
     * @param simulation Simulation object.
     */
    virtual void update(units::Duration dt, Simulation& simulation)
    {
        // Nothing to do
    }


    /**
     * @brief Configure module.
     *
     * @param config
     * @param simulation
     */
    virtual void configure(const Configuration& config, Simulation& simulation)
    {
        // Nothing to do
    }


#if ENABLE_RENDER
    /**
     * @brief Render module.
     *
     * @param context
     * @param simulation
     */
    virtual void draw(render::Context& context, const Simulation& simulation)
    {
        // Nothing to do
    }
#endif

};

/* ************************************************************************ */

}

/* ************************************************************************ */

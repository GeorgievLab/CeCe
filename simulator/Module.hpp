
#pragma once

/* ************************************************************************ */

// C++
#include <map>
#include <string>

// Simulator
#include "core/compatibility.hpp"
#include "core/Units.hpp"
#include "simulator/Configuration.hpp"

#ifdef ENABLE_RENDER
#include "render/Context.hpp"
#endif

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

class Simulation;

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
    virtual void update(core::units::Duration dt, Simulation& simulation)
    {
        // Nothing to do
    }


    /**
     * @brief Configure module.
     *
     * @param config
     * @param simulation
     */
    virtual void configure(const ConfigurationBase& config, simulator::Simulation& simulation)
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

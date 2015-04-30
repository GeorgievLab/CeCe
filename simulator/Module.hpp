
#pragma once

/* ************************************************************************ */

// C++
#include <map>
#include <string>

// Simulator
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
class Module
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
    virtual void update(units::Duration dt, Simulation& simulation) = 0;


    /**
     * @brief Configure module.
     *
     * @param config
     */
    virtual void configure(const ConfigurationBase& config)
    {
        // Nothing to do
    }


#ifdef ENABLE_RENDER
    /**
     * @brief Initialize module for rendering.
     *
     * @param context
     */
    virtual void drawInit(render::Context& context)
    {
        // Nothing to do
    }
#endif

#ifdef ENABLE_RENDER
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

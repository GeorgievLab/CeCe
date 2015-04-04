
#pragma once

/* ************************************************************************ */

// Simulator
#include "core/Units.hpp"

#ifdef ENABLE_RENDER
#include "render/Context.hpp"
#endif

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

class World;

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
     * @param dt    Simulation time step.
     * @param world World object.
     */
    virtual void update(units::Duration dt, World& world) = 0;


#ifdef ENABLE_RENDER
    /**
     * @brief Initialize module for rendering.
     *
     * @param context
     */
    virtual void renderInit(render::Context& context)
    {
        // Nothing to do
    }
#endif

#ifdef ENABLE_RENDER
    /**
     * @brief Render module.
     *
     * @param context
     * @param world
     */
    virtual void render(render::Context& context, const World& world)
    {
        // Nothing to do
    }
#endif

};

/* ************************************************************************ */

}

/* ************************************************************************ */

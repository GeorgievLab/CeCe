
#pragma once

/* ************************************************************************ */

// Simulator
#include "simulator/DiffusionModule.hpp"
#include "simulator/StreamlinesModule.hpp"

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

/**
 * @brief Module for diffuse & streamlines.
 */
class DiffusionStreamlinesModule : public DiffusionModule, public StreamlinesModule
{


// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     */
    DiffusionStreamlinesModule();


    /**
     * @brief Destructor.
     */
    virtual ~DiffusionStreamlinesModule();


// Public Accessors
public:



// Public Mutators
public:



// Public Operations
public:


    /**
     * @brief Update module state.
     *
     * @param dt    Simulation time step.
     * @param world World object.
     */
    void update(units::Duration dt, World& world) override;


#ifdef ENABLE_RENDER
    /**
     * @brief Initialize module for rendering.
     *
     * @param context
     */
    void renderInit(render::Context& context) override;
#endif


#ifdef ENABLE_RENDER
    /**
     * @brief Render module.
     *
     * @param context
     * @param world
     */
    void render(render::Context& context, const World& world) override;
#endif


// Private Data Members
private:

};

/* ************************************************************************ */

}

/* ************************************************************************ */

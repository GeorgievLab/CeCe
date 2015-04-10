
#pragma once

/* ************************************************************************ */

// Simulator
#include "simulator/Module.hpp"
#include "../diffusion/Module.hpp"
#include "../streamlines/Module.hpp"

/* ************************************************************************ */

namespace module {
namespace diffusion_streamlines {

/* ************************************************************************ */

/**
 * @brief Module for diffuse & streamlines.
 */
class Module : public simulator::Module
{


// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     */
    Module();


    /**
     * @brief Destructor.
     */
    virtual ~Module();


// Public Accessors
public:


    /**
     * @brief Returns diffusion module.
     *
     * @return
     */
    diffusion::Module& getDiffusion() noexcept
    {
        return m_diffusion;
    }


    /**
     * @brief Returns diffusion module.
     *
     * @return
     */
    const diffusion::Module& getDiffusion() const noexcept
    {
        return m_diffusion;
    }


    /**
     * @brief Returns streamlines module.
     *
     * @return
     */
    streamlines::Module& getStreamlines() noexcept
    {
        return m_streamlines;
    }


    /**
     * @brief Returns streamlines module.
     *
     * @return
     */
    const streamlines::Module& getStreamlines() const noexcept
    {
        return m_streamlines;
    }


// Public Operations
public:


    /**
     * @brief Update module state.
     *
     * @param dt    Simulation time step.
     * @param world World object.
     */
    void update(units::Duration dt, simulator::World& world) override;


#ifdef ENABLE_RENDER
    /**
     * @brief Initialize module for rendering.
     *
     * @param context
     */
    void drawInit(render::Context& context) override;
#endif


#ifdef ENABLE_RENDER
    /**
     * @brief Render module.
     *
     * @param context
     * @param world
     */
    void draw(render::Context& context, const simulator::World& world) override;
#endif


// Private Data Members
private:

    /// Diffusion module.
    diffusion::Module m_diffusion;

    /// Streamlines module.
    streamlines::Module m_streamlines;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */

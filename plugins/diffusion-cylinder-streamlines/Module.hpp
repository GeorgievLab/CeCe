
#pragma once

/* ************************************************************************ */

// Simulator
#include "simulator/Module.hpp"
#include "../diffusion/Module.hpp"
#include "../cylinder-streamlines/Module.hpp"

/* ************************************************************************ */

namespace module {
namespace diffusion_cylinder_streamlines {

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
     *
     * @param diffusion
     * @param streamlines
     */
    Module(diffusion::Module* diffusion, cylinder_streamlines::Module* streamlines)
        : m_diffusion(diffusion)
        , m_streamlines(streamlines)
    {
        // Nothing to do
    }


// Public Operations
public:


    /**
     * @brief Update module state.
     *
     * @param dt    Simulation time step.
     * @param simulation
     */
    void update(units::Duration dt, simulator::Simulation& simulation) override;


#if ENABLE_RENDER
    /**
     * @brief Render module.
     *
     * @param context
     * @param simulation
     */
    void draw(render::Context& context, const simulator::Simulation& simulation) override;
#endif


// Private Data Members
private:

    /// Diffusion module.
    diffusion::Module* m_diffusion;

    /// Streamlines module.
    cylinder_streamlines::Module* m_streamlines;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */

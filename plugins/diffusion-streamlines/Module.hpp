/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */

#pragma once

/* ************************************************************************ */

// Simulator
#include "core/ViewPtr.hpp"
#include "simulator/Module.hpp"

// Plugins
#include "plugins/diffusion/Module.hpp"
#include "plugins/streamlines/Module.hpp"

/* ************************************************************************ */

namespace plugin {
namespace diffusion_streamlines {

/* ************************************************************************ */

/**
 * @brief Module for diffusion & streamlines.
 */
class Module : public simulator::Module
{


// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param diffusion   Diffusion module.
     * @param streamlines Streamlines module.
     */
    Module(ViewPtr<diffusion::Module> diffusion, ViewPtr<streamlines::Module> streamlines)
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
     * @param dt         Simulation time step.
     * @param simulation Current simulation.
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
    ViewPtr<diffusion::Module> m_diffusion;

    /// Streamlines module.
    ViewPtr<streamlines::Module> m_streamlines;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */


#pragma once

/* ************************************************************************ */

// Simulator
#include "simulator/Module.hpp"
#include "simulator/Grid.hpp"
#include "simulator/World.hpp"

/* ************************************************************************ */

namespace module {
namespace diffusion {

/* ************************************************************************ */

class Module;

/* ************************************************************************ */

/**
 * @brief Diffusion generator.
 */
class Generator : public simulator::Module
{

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param module
     */
    explicit Generator(module::diffusion::Module* module) noexcept
        : m_diffusionModule(module)
    {
        // Nothing to do
    }


    /**
     * @brief Destructor.
     */
    virtual ~Generator();


// Public Operations
public:


    /**
     * @brief Update module state.
     *
     * @param dt     Simulation time step.
     * @param world  World object.
     */
    void update(units::Duration dt, simulator::World& world) override;


// Private Data Members
private:

    /// A pointer to diffusion module.
    module::diffusion::Module* m_diffusionModule;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */

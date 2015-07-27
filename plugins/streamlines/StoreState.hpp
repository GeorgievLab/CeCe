/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
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
#include "core/ViewPtr.hpp"
#include "simulator/Module.hpp"

// Plugin
#include "Module.hpp"

/* ************************************************************************ */

namespace plugin {
namespace streamlines {

/* ************************************************************************ */

/**
 * @brief Streamlines store state module.
 */
class StoreState : public simulator::Module
{

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param module A pointer to streamlines module.
     */
    explicit StoreState(ViewPtr<plugin::streamlines::Module> module) noexcept
        : m_streamlinesModule(module)
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
    void update(units::Duration dt, simulator::Simulation& simulation) override;


// Private Data Members
private:

    /// A pointer to streamlines module.
    ViewPtr<plugin::streamlines::Module> m_streamlinesModule;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */

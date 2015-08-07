/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* Author: Hynek Kasl <hkasl@students.zcu.cz>                               */
/* ************************************************************************ */

#pragma once

/* ************************************************************************ */

// Simulator
#include "core/ViewPtr.hpp"
#include "simulator/Program.hpp"

// Plugin
#include "Module.hpp"

/* ************************************************************************ */

namespace plugin {
namespace streamlines {

/* ************************************************************************ */

/**
 * @brief Object store state program.
 */
class StoreObjectState
{

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param module A pointer to streamlines module.
     */
    explicit StoreObjectState(ViewPtr<plugin::streamlines::Module> module) noexcept
        : m_streamlinesModule(module)
    {
        // Nothing to do
    }


// Public Operations
public:


    /**
     * @brief Update module state.
     *
     * @param object     Program owner.
     * @param dt         Simulation time step.
     * @param simulation Current simulation.
     */
    void operator()(simulator::Object& object, simulator::Simulation& simulation, units::Duration);


// Private Data Members
private:

    /// A pointer to streamlines module.
    ViewPtr<plugin::streamlines::Module> m_streamlinesModule;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */

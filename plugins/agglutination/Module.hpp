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

#if !ENABLE_PHYSICS
#error Agglutination requires physics engine
#endif

/* ************************************************************************ */

// C++
#include <utility>

// Simulator
#include "core/DynamicArray.hpp"
#include "core/String.hpp"
#include "simulator/Module.hpp"

// Physics
#include "Box2D/Box2D.h"

/* ************************************************************************ */

namespace plugin {
namespace agglutination {

/* ************************************************************************ */

/**
 * @brief Module for agglutination.
 */
class Module : public simulator::Module, public b2ContactListener
{

// Public Types
public:


// Public Contants
public:


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
    void update(units::Duration dt, simulator::Simulation& simulation) override;


    /**
     * @brief Configure module.
     *
     * @param config
     * @param simulation
     */
    void configure(const simulator::Configuration& config, simulator::Simulation& simulation) override;


    /**
     * @brief Called when two fixtures begin to touch.
     *
     * @param contact
     */
    void BeginContact(b2Contact* contact) override;


    /**
     * @brief Called when two fixtures cease to touch.
     *
     * @param contact
     */
    void EndContact(b2Contact* contact) override;


// Private Data Members
private:

    /// List of bodies to join
    // TODO: use Vector<b2Body*, 2>.
    DynamicArray<std::pair<b2Body*, b2Body*>> m_toJoin;
};

/* ************************************************************************ */

}
}

/* ************************************************************************ */

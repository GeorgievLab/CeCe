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
#include "core/Real.hpp"
#include "core/Tuple.hpp"
#include "simulator/Module.hpp"
#include "../cell/CellBase.hpp"


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
    // TODO: use Vector<b2Body*, 3>.
    DynamicArray<Tuple<b2Body*, b2Body*, RealType>> m_toJoin;
    units::Duration step;

// Public Data Members
public:
    struct Bond
    {
        RealType aConst;
        RealType dConst;
        String ligand;
        String receptor;
        RealType receptorSize;
        Bond(RealType association, RealType disassiociation, String ligand_name, String receptor_name, RealType size) noexcept
        : aConst(association), dConst(disassiociation), ligand(ligand_name), receptor(receptor_name), receptorSize(size)
        {
            // Nothing to do
        }
    };
    DynamicArray<Bond> m_bonds;
};

struct jointUserData
{
    char guard = '@';
    Module* module;
    RealType Kd;
};

/* ************************************************************************ */

}
}

/* ************************************************************************ */

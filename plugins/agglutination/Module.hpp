/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/*                                                                          */
/* This file is part of CeCe.                                               */
/*                                                                          */
/* CeCe is free software: you can redistribute it and/or modify             */
/* it under the terms of the GNU General Public License as published by     */
/* the Free Software Foundation, either version 3 of the License, or        */
/* (at your option) any later version.                                      */
/*                                                                          */
/* CeCe is distributed in the hope that it will be useful,                  */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of           */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            */
/* GNU General Public License for more details.                             */
/*                                                                          */
/* You should have received a copy of the GNU General Public License        */
/* along with CeCe.  If not, see <http://www.gnu.org/licenses/>.            */
/*                                                                          */
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


// Private Structures
private:


    /**
     * @brief Structure for storing bonds.
     */
    struct Bond
    {
        RealType aConst;
        RealType dConst;
        String ligand;
        String receptor;
    };


    /**
     * @brief User data for joint.
     */
    struct JointUserData
    {
        const char guard = '@';
        Module* module;
        RealType Kd;
    };


    /**
     * @brief Joint definition.
     */
    struct JointDef
    {
        b2Body* bodyA;
        b2Body* bodyB;
        RealType dConst;
    };


// Private Data Members
private:

    /// List of bodies to join
    DynamicArray<JointDef> m_toJoin;

    /// Used time step.
    units::Duration m_step;

    /// List of created bonds.
    DynamicArray<Bond> m_bonds;
};

/* ************************************************************************ */

}
}

/* ************************************************************************ */

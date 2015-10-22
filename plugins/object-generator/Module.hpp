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

// Simulator
#include "core/String.hpp"
#include "core/Units.hpp"
#include "core/VectorUnits.hpp"
#include "core/DynamicArray.hpp"
#include "simulator/Module.hpp"
#include "simulator/Simulation.hpp"
#include "simulator/Configuration.hpp"

/* ************************************************************************ */

namespace plugin {
namespace object_generator {

/* ************************************************************************ */

/**
 * @brief Structure for storing created object parameters.
 */
struct ObjectDesc
{
    // Generation probability per second.
    using Probability = typename units::Divide<units::Probability, units::Time>::type;


    /// Object class name.
    String className;

    /// Probability of object creation.
    Probability probability;

    /// Minimum spawn position.
    PositionVector positionMin;

    /// Maximum spawn position.
    PositionVector positionMax;

    /// List of iteration ranges when the generator is active.
    DynamicArray<Pair<simulator::IterationNumber, simulator::IterationNumber>> active;

    /// Object configuration
    simulator::Configuration config;
};

/* ************************************************************************ */

/**
 * @brief Object generator module.
 */
class Module : public simulator::Module
{

// Public Mutators
public:


    /**
     * @brief Register object.
     *
     * @param desc Object description.
     */
    void add(ObjectDesc desc)
    {
        m_objects.push_back(std::move(desc));
    }


// Public Operations
public:


    /**
     * @brief Update module state.
     *
     * @param dt     Simulation time step.
     * @param world  World object.
     */
    void update(units::Duration dt, simulator::Simulation& simulation) override;


    /**
     * @brief Configure module.
     *
     * @param config
     */
    void configure(const simulator::Configuration& config, simulator::Simulation& simulation) override;


// Private Data Members
private:


    /// List of generated objects.
    DynamicArray<ObjectDesc> m_objects;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */

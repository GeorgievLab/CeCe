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
#include "core/Real.hpp"
#include "core/String.hpp"
#include "core/Units.hpp"
#include "core/ViewPtr.hpp"
#include "core/Pair.hpp"
#include "simulator/Module.hpp"
#include "simulator/Simulation.hpp"

// Plugin
#include "Module.hpp"

/* ************************************************************************ */

namespace plugin {
namespace diffusion {

/* ************************************************************************ */

/**
 * @brief Module for diffusion generation.
 */
class Generator : public simulator::Module
{

// Public Types
public:


    /// Signal production.
    using ProductionRate = units::Divide<units::MolarConcentration, units::Time>::type;


// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param module A pointer to diffusion module.
     */
    explicit Generator(ViewPtr<plugin::diffusion::Module> module) noexcept
        : m_diffusionModule(module)
    {
        // Nothing to do
    }


// Public Operations
public:


    /**
     * @brief Update module state.
     *
     * @param dt         Simulation time step.
     * @param simulation Simulation object.
     */
    void update(units::Time dt, simulator::Simulation& simulation) override;


    /**
     * @brief Configure module.
     *
     * @param config
     * @param simulation
     */
    void configure(const simulator::Configuration& config, simulator::Simulation& simulation) override;


// Private Structures
private:


    /**
     * @brief Diffusion source.
     */
    struct Source
    {
        /// Signal name.
        String name;

        /// Source position.
        PositionVector position;

        /// Signal production.
        ProductionRate production;

        /// Rectangle size.
        PositionVector size;

        /// List of iteration ranges when the generator is active.
        DynamicArray<Pair<simulator::IterationNumber, simulator::IterationNumber>> active;
    };


// Private Data Members
private:


    /// Diffusion sources.
    DynamicArray<Source> m_sources;

    /// A pointer to diffusion module.
    ViewPtr<plugin::diffusion::Module> m_diffusionModule;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */

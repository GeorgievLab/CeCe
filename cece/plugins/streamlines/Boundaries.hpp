/* ************************************************************************ */
/* Georgiev Lab (c) 2015-2016                                               */
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

// C++
#include <cstddef>

// CeCe
#include "cece/core/Units.hpp"
#include "cece/core/StaticArray.hpp"

// Plugin
#include "cece/plugins/streamlines/Boundary.hpp"

/* ************************************************************************ */

namespace cece { namespace config { class Configuration; } }
namespace cece { namespace simulator { class Simulation; } }

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace streamlines {

/* ************************************************************************ */

/**
 * @brief A set of streamlines boundaries.
 */
class Boundaries
{


// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param simulation
     */
    explicit Boundaries(simulator::Simulation& simulation);


// Public Operators
public:


    /**
     * @brief Get boundary by position.
     *
     * @param position
     *
     * @return
     */
    Boundary& operator[](std::size_t position) noexcept
    {
        return get(position);
    }


    /**
     * @brief Get boundary by position.
     *
     * @param position
     *
     * @return
     */
    const Boundary& operator[](std::size_t position) const noexcept
    {
        return get(position);
    }


    /**
     * @brief Get boundary by position.
     *
     * @param position
     *
     * @return
     */
    Boundary& operator[](Boundary::Position position) noexcept
    {
        return get(position);
    }


    /**
     * @brief Get boundary by position.
     *
     * @param position
     *
     * @return
     */
    const Boundary& operator[](Boundary::Position position) const noexcept
    {
        return get(position);
    }


// Public Accessors
public:


    /**
     * @brief Return simulation.
     *
     * @return
     */
    simulator::Simulation& getSimulation() const noexcept
    {
        return m_simulation;
    }


    /**
     * @brief Get boundary by position.
     *
     * @param position
     *
     * @return
     */
    Boundary& get(std::size_t position) noexcept
    {
        return m_boundaries[position];
    }


    /**
     * @brief Get boundary by position.
     *
     * @param position
     *
     * @return
     */
    const Boundary& get(std::size_t position) const noexcept
    {
        return m_boundaries[position];
    }


    /**
     * @brief Get boundary by position.
     *
     * @param position
     *
     * @return
     */
    Boundary& get(Boundary::Position position) noexcept
    {
        return get(static_cast<std::size_t>(position));
    }


    /**
     * @brief Get boundary by position.
     *
     * @param position
     *
     * @return
     */
    const Boundary& get(Boundary::Position position) const noexcept
    {
        return get(static_cast<std::size_t>(position));
    }


    /**
     * @brief Find maximum inlet velocity.
     *
     * @return
     */
    units::Velocity getMaxInletVelocity() const noexcept;


// Public Operations
public:


    /**
     * @brief Initialize boundaries.
     */
    void init();

    /**
     * @brief Apply boundary conditions.
     *
     * @param converter
     * @param lattice
     * @param fluidDynamics
     */
    void applyConditions(Converter& converter, Lattice& lattice, ViewPtr<Dynamics> fluidDynamics);


    /**
     * @brief Load module configuration.
     *
     * @param config Source configuration.
     */
    void loadConfig(const config::Configuration& config);


    /**
     * @brief Store module configuration.
     *
     * @param config Output configuration.
     */
    void storeConfig(config::Configuration& config) const;


// Private Data Members
private:

    /// Simulation reference.
    simulator::Simulation& m_simulation;

    /// Stored boundaries.
    StaticArray<Boundary, 4> m_boundaries;
};

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */

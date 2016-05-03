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

// CeCe
#include "cece/core/Real.hpp"
#include "cece/core/ViewPtr.hpp"

// CeCe Plugins
#include "cece/plugins/streamlines/Dynamics.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace streamlines {

/* ************************************************************************ */

/**
 * @brief BGK dynamics.
 */
class BgkDynamics : public Dynamics
{

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param omega Relaxation frequency.
     */
    explicit BgkDynamics(RealType omega)
        : m_omega(omega)
    {
        // Nothing to do
    }


// Public Accessors
public:


    /**
     * @brief Return relaxation frequency.
     *
     * @return
     */
    RealType getOmega() const noexcept
    {
        return m_omega;
    }


// Public Mutators
public:


    /**
     * @brief Set relaxation frequency.
     *
     * @param omega
     */
    void setOmega(RealType omega) noexcept
    {
        m_omega = omega;
    }


// Public Operations
public:


    /**
     * @brief Compute macroscopic density in node.
     *
     * @param data Lattice data.
     *
     * @return Macroscopic density.
     */
    DensityType computeDensity(const DataType& data) const noexcept;


    /**
     * @brief Compute macroscopic velocity in node.
     *
     * @param data Lattice data.
     *
     * @return Macroscopic velocity.
     */
    VelocityType computeVelocity(const DataType& data) const noexcept;


    /**
     * @brief Compute macroscopic momentum in node.
     *
     * @param data Lattice data.
     *
     * @return Macroscopic momentum.
     */
    MomentumType computeMomentum(const DataType& data) const noexcept;


    /**
     * @brief Initialize node equilibrum.
     *
     * @param iPop     Direction index.
     * @param density  Macroscopic density.
     * @param velocity Macroscopic velocity.
     *
     * @return Equilibrum distribution.
     */
    DensityType computeEquilibrium(DirectionType iPop, DensityType density,
        VelocityType velocity) const noexcept;


    /**
     * @brief Initialize node equilibrum.
     *
     * @param data     Lattice data.
     * @param velocity Macroscopic velocity.
     * @param density  Macroscopic density.
     */
    void initEquilibrium(DataType& data, VelocityType velocity, DensityType density) const noexcept;


    /**
     * @brief Define node macroscopic density.
     *
     * @param data    Lattice data.
     * @param density Required macroscopic density.
     */
    void defineDensity(DataType& data, DensityType density) const noexcept;


    /**
     * @brief Define node macroscopic velocity.
     *
     * @param data    Lattice data.
     * @param velocity Required macroscopic velocity.
     */
    void defineVelocity(DataType& data, VelocityType velocity) const noexcept;


    /**
     * @brief Perform node collision.
     *
     * @param data Lattice data.
     */
    void collide(DataType& data) const noexcept;


// Private Data Members
private:

    /// Relaxation frequency.
    RealType m_omega;

};

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */

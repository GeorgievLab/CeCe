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

// CeCe Plugins
#include "cece/plugins/streamlines/Descriptor.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace streamlines {

/* ************************************************************************ */

/**
 * @brief Abstract class for lattice node dynamics.
 */
class Dynamics
{

// Public Types
public:


    /// Population index type.
    using DirectionType = Descriptor::DirectionType;

    /// Density type.
    using DensityType = Descriptor::DensityType;

    /// Momentum type.
    using MomentumType = Descriptor::MomentumType;

    /// Velocity type.
    using VelocityType = Descriptor::VelocityType;

    /// Type of stored data.
    using DataType = Descriptor::DataType;


// Public Ctors & Dtors
public:


    /**
     * @brief Destructor.
     */
    virtual ~Dynamics();


// Public Operations
public:


    /**
     * @brief Compute macroscopic density in node.
     *
     * @param data Lattice data.
     *
     * @return Macroscopic density.
     */
    virtual DensityType computeDensity(const DataType& data) const noexcept = 0;


    /**
     * @brief Compute macroscopic velocity in node.
     *
     * @param data Lattice data.
     *
     * @return Macroscopic velocity.
     */
    virtual VelocityType computeVelocity(const DataType& data) const noexcept = 0;


    /**
     * @brief Compute macroscopic momentum in node.
     *
     * @param data Lattice data.
     *
     * @return Macroscopic momentum.
     */
    virtual MomentumType computeMomentum(const DataType& data) const noexcept = 0;


    /**
     * @brief Initialize node equilibrum.
     *
     * @param density  Macroscopic density.
     * @param iPop     Direction index.
     * @param velocity Macroscopic velocity.
     *
     * @return Equilibrum distribution.
     */
    virtual DensityType computeEquilibrium(DirectionType iPop, DensityType density,
        VelocityType velocity) const noexcept = 0;


    /**
     * @brief Initialize node equilibrum.
     *
     * @param data     Lattice data.
     * @param velocity Macroscopic velocity.
     * @param density  Macroscopic density.
     */
    virtual void initEquilibrium(DataType& data, VelocityType velocity, DensityType density) const noexcept = 0;


    /**
     * @brief Define node macroscopic density.
     *
     * @param data    Lattice data.
     * @param density Required macroscopic density.
     */
    virtual void defineDensity(DataType& data, DensityType density) const noexcept = 0;


    /**
     * @brief Define node macroscopic velocity.
     *
     * @param data    Lattice data.
     * @param velocity Required macroscopic velocity.
     */
    virtual void defineVelocity(DataType& data, VelocityType velocity) const noexcept = 0;


    /**
     * @brief Perform node collision.
     *
     * @param data Lattice data.
     */
    virtual void collide(DataType& data) const noexcept = 0;


};

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */

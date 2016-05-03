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
#include "cece/core/ViewPtr.hpp"

// Plugin
#include "cece/plugins/streamlines/Descriptor.hpp"
#include "cece/plugins/streamlines/Dynamics.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace streamlines {

/* ************************************************************************ */

/**
 * @brief Class for storing lattice data and perform LB dynamics.
 */
class Node
{

// Public Types
public:

    /// Population index type.
    using DirectionType = typename Dynamics::DirectionType;

    /// Density type.
    using DensityType = typename Dynamics::DensityType;

    /// Momentum type.
    using MomentumType = typename Dynamics::MomentumType;

    /// Velocity type.
    using VelocityType = typename Dynamics::VelocityType;

    /// Data type.
    using DataType = typename Dynamics::DataType;


// Public Operators
public:


    /**
     * @brief Returns microscopic density population in required direction.
     *
     * @param iPop Direction index.
     *
     * @return
     */
    DensityType operator[](DirectionType iPop) const noexcept
    {
        return get(iPop);
    }


    /**
     * @brief Returns microscopic density population in required direction.
     *
     * @param iPop Direction index.
     *
     * @return
     */
    DensityType& operator[](DirectionType iPop) noexcept
    {
        return get(iPop);
    }


// Public Accessors
public:


    /**
     * @brief Returns node dynamics.
     *
     * @return
     */
    ViewPtr<Dynamics> getDynamics() const noexcept
    {
        return m_dynamics;
    }


    /**
     * @brief Returns microscopic density population in required direction.
     *
     * @param iPop Direction index.
     *
     * @return
     */
    DensityType& get(DirectionType iPop) noexcept
    {
        return m_data[iPop];
    }


    /**
     * @brief Returns microscopic density population in required direction.
     *
     * @param iPop Direction index.
     *
     * @return
     */
    DensityType get(DirectionType iPop) const noexcept
    {
        return m_data[iPop];
    }


    /**
     * @brief Returns node data.
     *
     * @return
     */
    DataType& getData() noexcept
    {
        return m_data;
    }


    /**
     * @brief Returns node data.
     *
     * @return
     */
    const DataType& getData() const noexcept
    {
        return m_data;
    }


// Public Mutators
public:


    /**
     * @brief Change node dynamics.
     *
     * @param dynamics A pointer to new dynamics.
     *
     * @note Node doesn't take ownership.
     */
    void setDynamics(ViewPtr<Dynamics> dynamics) noexcept
    {
        m_dynamics = dynamics;
    }


    /**
     * @brief Define node's velocity.
     *
     * @param velocity Required macroscopic velocity.
     */
    void defineVelocity(const VelocityType& velocity) noexcept
    {
        Assert(m_dynamics);
        return m_dynamics->defineVelocity(m_data, velocity);
    }


    /**
     * @brief Define node's density.
     *
     * @param context  Computation context.
     * @param density  Required macroscopic density.
     */
    void defineDensity(DensityType density) noexcept
    {
        Assert(m_dynamics);
        return m_dynamics->defineDensity(m_data, density);
    }


// Public Operations
public:


    /**
     * @brief Compute macroscopic density.
     *
     * @return
     */
    DensityType computeDensity() const noexcept
    {
        Assert(m_dynamics);
        return m_dynamics->computeDensity(m_data);
    }


    /**
     * @brief Compute macroscopic momentum.
     *
     * @return
     */
    MomentumType computeMomentum() const noexcept
    {
        Assert(m_dynamics);
        return m_dynamics->computeMomentum(m_data);
    }


    /**
     * @brief Compute macroscopic velocity.
     *
     * @return
     */
    VelocityType computeVelocity() const noexcept
    {
        Assert(m_dynamics);
        return m_dynamics->computeVelocity(m_data);
    }


    /**
     * @brief Initialize node by equilibrium populations.
     *
     * @param velocity Cell velocity vector.
     * @param density  Cell density.
     */
    void initEquilibrium(VelocityType velocity = Zero, DensityType density = Descriptor::DEFAULT_DENSITY) noexcept
    {
        Assert(m_dynamics);
        m_dynamics->initEquilibrium(m_data, velocity, density);
    }


    /**
     * @brief Perform node's populations collision.
     */
    void collide() noexcept
    {
        Assert(m_dynamics);
        return m_dynamics->collide(m_data);
    }


// Private Data Members
public:

    /// Node dynamics type.
    ViewPtr<Dynamics> m_dynamics = nullptr;

    /// Node data.
    DataType m_data;

};

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */

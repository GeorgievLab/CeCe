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
#include "cece/plugins/streamlines/BgkDynamics.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace streamlines {

/* ************************************************************************ */

/**
 * @brief Zou He boundary condition dynamics.
 */
class ZouHeDynamics : public BgkDynamics
{

// Public Enums
public:


    /**
     * @brief Boundary condition position.
     */
    enum class Position
    {
        Right  = 0,
        Left   = 1,
        Top    = 2,
        Bottom = 3
    };


// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param omega    Relaxation frequency.
     * @param position Boundary condition position.
     */
    explicit ZouHeDynamics(RealType omega, Position position)
        : BgkDynamics(omega)
        , m_position(position)
    {
        // Nothing to do
    }


// Public Operations
public:


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
     * @brief Calculate density from velocity.
     *
     * @param data     Node data.
     * @param velocity Fixed velocity.
     *
     * @return
     */
    DensityType calcDensity(DataType& data, const VelocityType& velocity) const noexcept;


    /**
     * @brief Calculate velocity from density.
     *
     * @param data    Node data.
     * @param density Fixed density.
     *
     * @return
     */
    VelocityType calcVelocity(DataType& data, DensityType density) const noexcept;


    /**
     * @brief Initialize data.
     *
     * @param data     Node data.
     * @param velocity Required velocity.
     * @param density  Required density.
     */
    void init(DataType& data, VelocityType velocity, DensityType density) const noexcept;


// Private Data Members
private:

    /// Boundary position.
    Position m_position;
};

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */

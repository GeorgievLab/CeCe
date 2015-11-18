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

// CeCe
#include "cece/core/Units.hpp"
#include "cece/core/Vector.hpp"
#include "cece/core/Grid.hpp"
#include "cece/core/StaticArray.hpp"

// Plugin
#include "cece/plugins/streamlines/LatticeCell.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace streamlines {

/* ************************************************************************ */

/**
 * @brief Implementation of Lattice Boltzman.
 *
 * Memory and speedup improvement taken from OpenLB TR1:
 * @link http://optilb.com/openlb/wp-content/uploads/2011/12/olb-tr1.pdf
 */
class Lattice
{


// Public Types
public:


    /**
     * @brief Container type.
     */
    using ContainerType = core::Grid<LatticeCell>;


    /**
     * @brief Size type.
     */
    using SizeType = ContainerType::SizeType;


    /**
     * @brief Coordinates type.
     */
    using CoordinateType = ContainerType::CoordinateType;


    /**
     * @brief Size type.
     */
    using Size = Vector<SizeType>;


// Public Operators
public:


    /**
     * @brief Array operator.
     *
     * @param coord
     *
     * @return
     */
    LatticeCell& operator[](const CoordinateType& coord) noexcept
    {
        return get(coord);
    }


    /**
     * @brief Array operator.
     *
     * @param coord
     *
     * @return
     */
    const LatticeCell& operator[](const CoordinateType& coord) const noexcept
    {
        return get(coord);
    }


// Public Accessors
public:


    /**
     * @brief Returns lattice size.
     *
     * @return
     */
    Size getSize() const noexcept
    {
        return m_data.getSize();
    }


    /**
     * @brief Check if coordinates are in range.
     *
     * @param coord
     *
     * @return
     */
    bool inRange(const CoordinateType& coord) const noexcept
    {
        return coord.inRange(CoordinateType{Zero}, getSize());
    }


    /**
     * @brief Get item from front buffer.
     *
     * @param coord Item coordinate.
     *
     * @return
     */
    LatticeCell& get(const CoordinateType& coord) noexcept
    {
        return m_data[coord];
    }


    /**
     * @brief Get item from front buffer.
     *
     * @param coord Item coordinate.
     *
     * @return
     */
    const LatticeCell& get(const CoordinateType& coord) const noexcept
    {
        return m_data[coord];
    }


#if !DEV_PLUGIN_streamlines_SWAP_TRICK
    /**
     * @brief Get item from back buffer.
     *
     * @param coord Item coordinate.
     *
     * @return
     */
    LatticeCell& getBack(const CoordinateType& coord) noexcept
    {
        return m_dataBack[coord];
    }
#endif


#if !DEV_PLUGIN_streamlines_SWAP_TRICK
    /**
     * @brief Get item from back buffer.
     *
     * @param coord Item coordinate.
     *
     * @return
     */
    const LatticeCell& getBack(const CoordinateType& coord) const noexcept
    {
        return m_dataBack[coord];
    }
#endif


// Public Mutators
public:


    /**
     * @brief Resize grid.
     *
     * @param size
     */
    void setSize(Size size)
    {
#if !DEV_PLUGIN_streamlines_SWAP_TRICK
        m_data.resize(size);
        m_dataBack.resize(size);
#endif
    }


// Public Operations
public:


    /**
     * @brief Compute lattice collisions.
     *
     * @param omega
     */
    void collide(LatticeCell::ValueType omega);


    /**
     * @brief Steam lattice data.
     */
    void stream();


    /**
     * @brief Call collide and stream functions.
     *
     * @param omega
     */
    void collideAndStream(LatticeCell::ValueType omega);


    /**
     * @brief Set dynamics for all cells.
     *
     * @param dynamics
     */
    void setDynamics(LatticeCell::Dynamics dynamics);


    /**
     * @brief Fixup inner obstacle types.
     *
     * @param dynamics
     */
    void fixupObstacles(LatticeCell::Dynamics dynamics) noexcept;


// Private Data Members
public:

    /// Current lattice data.
    core::Grid<LatticeCell> m_data;

#if !DEV_PLUGIN_streamlines_SWAP_TRICK
    /// Temporaty lattice data.
    core::Grid<LatticeCell> m_dataBack;
#endif
};

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */

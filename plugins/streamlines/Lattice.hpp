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
#include "core/Units.hpp"
#include "core/Vector.hpp"
#include "core/Grid.hpp"
#include "core/StaticArray.hpp"

// Module
#include "LatticeCell.hpp"

/* ************************************************************************ */

namespace plugin {
namespace streamlines {

/* ************************************************************************ */

/**
 * @brief Lattice Boltzman lattice class.
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


// Public Mutators
public:


    /**
     * @brief Resize grid.
     *
     * @param size
     */
    void setSize(Size size)
    {
        m_data.resize(size);
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
     * @brief Set type for all cells.
     *
     * @param type
     */
    void setType(LatticeCell::Type type);


    /**
     * @brief Fixup inner obstacle types.
     *
     * @param type
     */
    void fixupObstacles(LatticeCell::Type type) noexcept;


// Private Data Members
public:

    /// Current lattice data.
    core::Grid<LatticeCell> m_data;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */

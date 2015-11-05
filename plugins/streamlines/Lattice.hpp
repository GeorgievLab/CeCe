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

// C++
#include <algorithm>

// Simulator
#include "core/Units.hpp"
#include "core/Vector.hpp"
#include "core/Grid.hpp"
#include "core/StaticArray.hpp"

// Module
#include "LatticeData.hpp"

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
    using ContainerType = core::Grid<LatticeData>;


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
    LatticeData& operator[](const CoordinateType& coord) noexcept
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
    const LatticeData& operator[](const CoordinateType& coord) const noexcept
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
        // Lattice itself have 1 cell margin
        return m_dataFront.getSize() - 2;
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
    LatticeData& getFront(const CoordinateType& coord) noexcept
    {
        return m_dataFront[coord + 1];
    }


    /**
     * @brief Get item from front buffer.
     *
     * @param coord Item coordinate.
     *
     * @return
     */
    const LatticeData& getFront(const CoordinateType& coord) const noexcept
    {
        return m_dataFront[coord + 1];
    }


    /**
     * @brief Get item from back buffer.
     *
     * @param coord Item coordinate.
     *
     * @return
     */
    LatticeData& getBack(const CoordinateType& coord) noexcept
    {
        return m_dataBack[coord + 1];
    }


    /**
     * @brief Get item from back buffer.
     *
     * @param coord Item coordinate.
     *
     * @return
     */
    const LatticeData& getBack(const CoordinateType& coord) const noexcept
    {
        return m_dataBack[coord + 1];
    }


    /**
     * @brief Get item from front buffer.
     *
     * @param coord Item coordinate.
     *
     * @return
     */
    LatticeData& get(const CoordinateType& coord) noexcept
    {
        return getFront(coord);
    }


    /**
     * @brief Get item from front buffer.
     *
     * @param coord Item coordinate.
     *
     * @return
     */
    const LatticeData& get(const CoordinateType& coord) const noexcept
    {
        return getFront(coord);
    }


// Public Mutators
public:


    /**
     * @brief Resize grid.
     *
     * @param size
     */
    void setSize(Size size);


// Public Operations
public:


    /**
     * @brief Compute lattice collisions.
     *
     * @param omega
     */
    void collide(LatticeData::ValueType omega);


    /**
     * @brief Propagate lattice data.
     */
    void propagate();


    /**
     * @brief Call collide and propagate functions.
     *
     * @param omega
     */
    void collideAndPropagate(LatticeData::ValueType omega)
    {
        collide(omega);
        propagate();
    }


    /**
     * @brief Set type for all cells.
     *
     * @param type
     */
    void setType(LatticeData::Type type);


    /**
     * @brief Fixup inner obstacle types.
     *
     * @param type
     */
    void fixupObstacles(LatticeData::Type type) noexcept;


// Private Data Members
public:


    /// Current lattice data.
    core::Grid<LatticeData> m_dataFront;

    /// Temporaty lattice data.
    core::Grid<LatticeData> m_dataBack;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */

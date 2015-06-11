
#pragma once

/* ************************************************************************ */

// C++
#include <algorithm>

// Simulator
#include "core/compatibility.hpp"
#include "core/Units.hpp"
#include "core/Vector.hpp"
#include "core/Grid.hpp"
#include "core/StaticArray.hpp"

// Module
#include "LatticeData.hpp"

/* ************************************************************************ */

namespace module {
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


// Public Constants
public:


// Public Ctors & Dtors
public:


// Public Operators
public:


    /**
     * @brief Array operator.
     *
     * @param coord
     *
     * @return
     */
    LatticeData& operator[](const CoordinateType& coord) NOEXCEPT
    {
        return m_data[coord];
    }


    /**
     * @brief Array operator.
     *
     * @param coord
     *
     * @return
     */
    const LatticeData& operator[](const CoordinateType& coord) const NOEXCEPT
    {
        return m_data[coord];
    }


// Public Accessors
public:


    /**
     * @brief Returns lattice size.
     *
     * @return
     */
    Size getSize() const NOEXCEPT
    {
        // Lattice itself have 1 cell margin
        return getRealSize() - 2;
    }


    /**
     * @brief Returns real lattice size.
     *
     * @return
     */
    const Size& getRealSize() const NOEXCEPT
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
    bool inRange(const CoordinateType& coord) const NOEXCEPT
    {
        return m_data.inRange(coord);
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
     * @brief Set dynamic obstacle flag to false for all cells.
     */
    void clearDynamicObstacles();


// Private Data Members
public:


    /// Current lattice data.
    core::Grid<LatticeData> m_data;

    /// Temporaty lattice data.
    core::Grid<LatticeData> m_dataTmp;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */

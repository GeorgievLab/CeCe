
#pragma once

/* ************************************************************************ */

// C++
#include <array>
#include <algorithm>

// Simulator
#include "core/Units.hpp"
#include "core/Vector.hpp"
#include "core/Grid.hpp"

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
    LatticeData& operator[](const CoordinateType& coord) noexcept
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
    const LatticeData& operator[](const CoordinateType& coord) const noexcept
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
    Vector<SizeType> getSize() const noexcept
    {
        // Lattice itself have 1 cell margin
        return getRealSize() - 2;
    }


    /**
     * @brief Returns real lattice size.
     *
     * @return
     */
    const Vector<SizeType>& getRealSize() const noexcept
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
    bool inRange(const Vector<SizeType>& coord) const noexcept
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
    void setSize(Vector<SizeType> size);


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

    /// Temporaty lattice data
    core::Grid<LatticeData> m_dataTmp;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */

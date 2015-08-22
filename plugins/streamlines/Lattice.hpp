/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* Author: Hynek Kasl <hkasl@students.zcu.cz>                               */
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
     * @brief Set dynamic obstacle flag to false for all cells.
     */
    void clearDynamicObstacles();


    /**
     * @brief Set static obstacle flag to false for all cells.
     */
    void clearStaticObstacles();


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

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

// C++
#include <vector>

// CeCe
#include "cece/core/Vector.hpp"
#include "cece/core/DynamicArray.hpp"
#include "cece/core/AlignedAllocator.hpp"

/* ************************************************************************ */

namespace cece {
inline namespace core {

/* ************************************************************************ */

/**
 * @brief Class for storing data in grid.
 *
 * Data is stored in linear array in layout where each row is together,
 * e.g. <row0><row1>...
 *
 * @tparam T     Stored value type.
 * @tparam Alloc Type of used allocator.
 */
template<typename T, typename Alloc = AlignedAllocator<T>>
class Grid
{


// Public Types
public:


    /**
     * @brief Value type.
     */
    using ValueType = T;


    /**
     * @brief Allocator type.
     */
    using AllocatorType = Alloc;


    /**
     * @brief Size type.
     */
    using SizeType = unsigned int;


    /**
     * @brief Container type.
     */
    using ContainerType = DynamicArray<ValueType, AllocatorType>;


    /**
     * @brief Coordinates type.
     */
    using CoordinateType = Vector<SizeType>;


// Public Ctors & Dtors
public:


    /**
     * @brief Default constructor.
     */
    Grid() = default;


    /**
     * @brief Constructor.
     *
     * @param size
     */
    explicit Grid(Vector<SizeType> size)
    {
        resize(std::move(size));
    }


    /**
     * @brief Constructor.
     *
     * @param size
     */
    explicit Grid(SizeType size)
        : Grid(Vector<SizeType>::createSingle(size))
    {
        // Nothing to do
    }


// Public Operators
public:


    /**
     * @brief Array operator.
     *
     * @param offset
     *
     * @return
     */
    ValueType& operator[](SizeType offset) noexcept
    {
        return m_data[offset];
    }


    /**
     * @brief Array operator.
     *
     * @param offset
     *
     * @return
     */
    const ValueType& operator[](SizeType offset) const noexcept
    {
        return m_data[offset];
    }


    /**
     * @brief Array operator.
     *
     * @param coord
     *
     * @return
     */
    ValueType& operator[](const CoordinateType& coord) noexcept
    {
        return m_data[calcOffset(coord)];
    }


    /**
     * @brief Array operator.
     *
     * @param coord
     *
     * @return
     */
    const ValueType& operator[](const CoordinateType& coord) const noexcept
    {
        return m_data[calcOffset(coord)];
    }


// Public Accessors
public:


    /**
     * @brief Returns grid size.
     *
     * @return
     */
    const Vector<SizeType>& getSize() const noexcept
    {
        return m_size;
    }


    /**
     * @brief Returns grid plain data as linear array.
     *
     * @return
     */
    const T* getData() const noexcept
    {
        return m_data.data();
    }


    /**
     * @brief Returns grid inner container.
     *
     * @return
     */
    const ContainerType& getContainer() const noexcept
    {
        return m_data;
    }


    /**
     * @brief Returns grid inner container.
     *
     * @return
     */
    ContainerType& getContainer() noexcept
    {
        return m_data;
    }


    /**
     * @brief Returns begin iterator.
     *
     * @return
     */
    typename ContainerType::iterator begin() noexcept
    {
        return m_data.begin();
    }


    /**
     * @brief Returns begin constant iterator.
     *
     * @return
     */
    typename ContainerType::const_iterator begin() const noexcept
    {
        return m_data.begin();
    }


    /**
     * @brief Returns begin constant iterator.
     *
     * @return
     */
    typename ContainerType::const_iterator cbegin() const noexcept
    {
        return m_data.cbegin();
    }


    /**
     * @brief Returns end iterator.
     *
     * @return
     */
    typename ContainerType::iterator end() noexcept
    {
        return m_data.end();
    }


    /**
     * @brief Returns end constant iterator.
     *
     * @return
     */
    typename ContainerType::const_iterator end() const noexcept
    {
        return m_data.end();
    }


    /**
     * @brief Returns end constant iterator.
     *
     * @return
     */
    typename ContainerType::const_iterator cend() const noexcept
    {
        return m_data.cend();
    }


// Public Operations
public:


    /**
     * @brief Resize grid.
     *
     * @param size New size of the grid.
     */
    void resize(SizeType size)
    {
        resize(Vector<SizeType>::createSingle(size));
    }


    /**
     * @brief Resize grid.
     *
     * @param size  New size of the grid.
     * @param value The value to initialize the new elements with.
     */
    void resize(SizeType size, const ValueType& value)
    {
        resize(Vector<SizeType>::createSingle(size), value);
    }


    /**
     * @brief Resize grid.
     *
     * @param size New size of the grid.
     */
    void resize(Vector<SizeType> size)
    {
        m_size = std::move(size);
        m_data.resize(m_size.getWidth() * m_size.getHeight());
    }


    /**
     * @brief Resize grid.
     *
     * @param size  New size of the grid.
     * @param value The value to initialize the new elements with.
     */
    void resize(Vector<SizeType> size, const ValueType& value)
    {
        m_size = std::move(size);
        m_data.resize(m_size.getWidth() * m_size.getHeight(), value);
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
        // Unsigned type for SizeType is allways greater than or equal to zero,
        // the optimizer should remove that checks
        return
            coord.getX() >= 0 &&
            coord.getY() >= 0 &&
            coord.getX() < getSize().getWidth() &&
            coord.getY() < getSize().getHeight()
        ;
    }


    /**
     * @brief Calculate array offset.
     *
     * @param coord
     *
     * @return
     */
    SizeType calcOffset(const CoordinateType& coord) const noexcept
    {
        return coord.getX() + coord.getY() * getSize().getWidth();
    }


// Private Data Members
private:

    /// Grid data.
    ContainerType m_data;

    /// Grid size
    Vector<SizeType> m_size;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */

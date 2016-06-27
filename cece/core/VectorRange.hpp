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
#include "cece/core/IteratorRange.hpp"
#include "cece/core/Vector.hpp"

/* ************************************************************************ */

namespace cece {
inline namespace core {

/* ************************************************************************ */

/**
 * @brief Wrapper for vector iterator.
 *
 * @tparam T Element type of Vector.
 */
template<typename T>
struct IteratorVector
{

// Public Types
public:


    /// Iterator traits
    using iterator_category = std::input_iterator_tag;
    using value_type = Vector<T>;
    using difference_type = std::ptrdiff_t;
    using pointer = const value_type*;
    using reference = const value_type&;


// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param value Current value.
     * @param max   Maximum value.
     * @param min   Minimum value.
     */
    IteratorVector(Vector<T> value, Vector<T> max, Vector<T> min = Zero) noexcept
        : m_value(std::move(value))
        , m_max(std::move(max))
        , m_min(std::move(min))
    {
        // Nothing to do
    }


// Public Operators
public:


    /**
     * @brief Cast to value.
     */
    operator Vector<T>() const noexcept
    {
        return m_value;
    }


    /**
     * @brief Compares values.
     *
     * @param rhs Second value.
     *
     * @return Are values equal?
     */
    bool operator==(const IteratorVector& rhs) const noexcept
    {
        return m_value == rhs.m_value;
    }


    /**
     * @brief Compares values.
     *
     * @param rhs Second value.
     *
     * @return Aren't values equal?
     */
    bool operator!=(const IteratorVector& rhs) const noexcept
    {
        return !operator==(rhs);
    }


    /**
     * @brief Dereference operator.
     *
     * @return A reference to the current token.
     */
    reference operator*() const noexcept
    {
        return m_value;
    }


    /**
     * @brief Dereference operator.
     *
     * @return A pointer to the current token.
     */
    pointer operator->() const noexcept
    {
        return &m_value;
    }


    /**
     * @brief Increment operator.
     *
     * @return *this.
     */
    IteratorVector& operator++() noexcept
    {
        if (m_value.x() == m_max.x())
        {
            m_value.x() = m_min.x();
            ++m_value.y();
        }
        else
        {
            ++m_value.x();
        }

        return *this;
    }


    /**
     * @brief Post-increment operator.
     *
     * @note Is deleted (because I'm to lazy to write it).
     */
    IteratorVector operator++(int) = delete;


// Private Data Members
private:

    /// Current value.
    Vector<T> m_value;

    /// Max value.
    Vector<T> m_max;

    /// Min value.
    Vector<T> m_min;

};

/* ************************************************************************ */

/**
 * @brief Make Vector range.
 *
 * @tparam T Vector element type.
 *
 * @param begin Begin vector.
 * @param end   End vector.
 *
 * @return
 */
template<typename T>
constexpr IteratorRange<IteratorVector<T>> range(Vector<T> begin, Vector<T> end) noexcept
{
    return makeRange(
        IteratorVector<T>{begin, end - T{1}, begin},
        IteratorVector<T>{Vector<T>{begin.getX(), end.getY()}, Zero, Zero}
    );
}

/* ************************************************************************ */

/**
 * @brief Make Vector range.
 *
 * @tparam T Vector element type.
 *
 * @param end End vector.
 *
 * @return
 */
template<typename T>
constexpr IteratorRange<IteratorVector<T>> range(Vector<T> end) noexcept
{
    return range(Vector<T>(Zero), end);
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

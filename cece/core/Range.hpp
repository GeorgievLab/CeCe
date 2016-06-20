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
#include <type_traits>

/* ************************************************************************ */

namespace cece {
inline namespace core {

/* ************************************************************************ */

/**
 * @brief Value range class.
 */
template<typename T>
class Range
{

// Public Types
public:

    /// Value type.
    using ValueType = T;

    /// Difference type.
    using DifferenceType = typename std::make_signed<T>::type;


// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param first
     * @param last
     */
    constexpr Range(ValueType first, ValueType last) noexcept
        : m_first(first)
        , m_last(last)
    {
        // Nothing to do
    }


// Public Accessors
public:


    /**
     * @brief Returns if iterator range is empty.
     *
     * @return
     */
    bool isEmpty() const noexcept
    {
        return m_first == m_last;
    }


    /**
     * @brief Returns range size.
     *
     * @return
     */
    DifferenceType getSize() const noexcept
    {
        return m_last - m_first;
    }


// Public Operations
public:


    /**
     * @brief Returns the first value.
     *
     * @return
     */
    constexpr ValueType first() const noexcept
    {
        return m_first;
    }


    /**
     * @brief Returns the last value.
     *
     * @return
     */
    constexpr ValueType last() const noexcept
    {
        return m_last;
    }


// Private Data Members
private:

    /// The first value.
    ValueType m_first;

    // The last value.
    ValueType m_last;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */

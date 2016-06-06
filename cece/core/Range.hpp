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
#include <iterator>

// CeCe
#include "cece/core/ValueIterator.hpp"

/* ************************************************************************ */

namespace cece {
inline namespace core {

/* ************************************************************************ */

/**
 * @brief Iterator range class.
 */
template<typename T>
class IteratorRange
{

// Public Types
public:


    /// Iterator type
    using IteratorType = T;

    /// Iterator value type.
    using ValueType = typename std::iterator_traits<IteratorType>::value_type;

    /// Iterator difference type.
    using DifferenceType = typename std::iterator_traits<IteratorType>::difference_type;


// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param begin
     * @param end
     */
    constexpr IteratorRange(IteratorType begin, IteratorType end) noexcept
        : m_begin(begin)
        , m_end(end)
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
        return m_begin == m_end;
    }


    /**
     * @brief Returns range size.
     *
     * @return
     */
    unsigned long getSize() const noexcept
    {
        return std::distance(m_begin, m_end);
    }


    /**
     * @brief Returns reference to value at begin iterator.
     *
     * @return
     */
    const ValueType& front() const noexcept
    {
        return *m_begin;
    }


// Public Operations
public:


    /**
     * @brief Returns begin iterator.
     *
     * @return
     */
    constexpr IteratorType begin() const noexcept
    {
        return m_begin;
    }


    /**
     * @brief Returns end iterator.
     *
     * @return
     */
    constexpr IteratorType end() const noexcept
    {
        return m_end;
    }


    /**
     * @brief Advance begin iterator.
     *
     * @param n Number of steps to advance.
     *
     * @return *this.
     */
    IteratorRange& advanceBegin(DifferenceType n = 1) noexcept
    {
        std::advance(m_begin, n);
        return *this;
    }


    /**
     * @brief Advance begin iterator.
     *
     * @param n Number of steps to advance.
     *
     * @return *this.
     */
    IteratorRange& advanceEnd(DifferenceType n = 1) noexcept
    {
        std::advance(m_end, n);
        return *this;
    }


    /**
     * @brief Advance both iterator.
     *
     * @param n Number of steps to advance.
     *
     * @return *this.
     */
    IteratorRange& advanceBoth(DifferenceType n = 1) noexcept
    {
        advanceBegin(n);
        advanceEnd(n);
        return *this;
    }


// Private Data Members
private:


    /// Begin iterator.
    IteratorType m_begin;

    // End iterator.
    IteratorType m_end;

};

/* ************************************************************************ */

/**
 * @brief Make iterator range.
 *
 * @param begin Begin iterator.
 * @param end   End iterator.
 *
 * @return Iterator range for given range.
 */
template<typename Iter>
constexpr IteratorRange<Iter> makeRange(Iter begin, Iter end) noexcept
{
    return IteratorRange<Iter>{begin, end};
}

/* ************************************************************************ */

/**
 * @brief Make iterator range from container.
 *
 * @tparam Container Container type.
 *
 * @param c Container.
 *
 * @return Iterator range for whole container data.
 */
template<typename Container>
constexpr auto makeRange(Container& c) noexcept -> IteratorRange<decltype(std::begin(c))>
{
    using std::begin;
    using std::end;
    return IteratorRange<decltype(std::begin(c))>{begin(c), end(c)};
}

/* ************************************************************************ */

/**
 * @brief Make value range (for-loop like).
 *
 * @param begin Begin value.
 * @param end   End value.
 *
 * @note The end value is not a part of returned range, e.g. range(1, 10)
 * returns values {1, 2, 3, 4, 5, 6, 7, 8, 9}.
 *
 * @return
 */
template<typename T>
constexpr IteratorRange<ValueIterator<T>> range(T begin, T end) noexcept
{
    return IteratorRange<ValueIterator<T>>{ValueIterator<T>{begin}, ValueIterator<T>{end}};
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

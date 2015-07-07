/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* ************************************************************************ */

#pragma once

/* ************************************************************************ */

// C++
#include <iterator>

// Simulator
#include "core/compatibility.hpp"
#include "core/ValueIterator.hpp"

/* ************************************************************************ */

#ifndef _MSC_VER
inline namespace core {
#endif

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
    CONSTEXPR IteratorRange(IteratorType begin, IteratorType end) NOEXCEPT
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
    bool isEmpty() const NOEXCEPT
    {
        return m_begin == m_end;
    }


    /**
     * @brief Returns range size.
     *
     * @return
     */
    unsigned long getSize() const NOEXCEPT
    {
        return std::distance(m_begin, m_end);
    }


    /**
     * @brief Returns reference to value at begin iterator.
     *
     * @return
     */
    const ValueType& front() const NOEXCEPT
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
    CONSTEXPR IteratorType begin() const NOEXCEPT
    {
        return m_begin;
    }


    /**
     * @brief Returns end iterator.
     *
     * @return
     */
    CONSTEXPR IteratorType end() const NOEXCEPT
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
    IteratorRange& advanceBegin(DifferenceType n = 1) NOEXCEPT
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
    IteratorRange& advanceEnd(DifferenceType n = 1) NOEXCEPT
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
    IteratorRange& advanceBoth(DifferenceType n = 1) NOEXCEPT
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
CONSTEXPR IteratorRange<Iter> makeRange(Iter begin, Iter end) NOEXCEPT
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
CONSTEXPR auto makeRange(Container& c) NOEXCEPT -> IteratorRange<decltype(std::begin(c))>
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
CONSTEXPR IteratorRange<ValueIterator<T>> range(T begin, T end) NOEXCEPT
{
    return IteratorRange<ValueIterator<T>>{ValueIterator<T>{begin}, ValueIterator<T>{end}};
}

/* ************************************************************************ */

#ifndef _MSC_VER
}
#endif

/* ************************************************************************ */

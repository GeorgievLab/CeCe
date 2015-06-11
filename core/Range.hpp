/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* ************************************************************************ */

#pragma once

/* ************************************************************************ */

// Simulator
#include "core/compatibility.hpp"
#include "core/Iterator.hpp"

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
 * @return
 */
template<typename Iter>
CONSTEXPR IteratorRange<Iter> makeRange(Iter begin, Iter end) NOEXCEPT
{
    return IteratorRange<Iter>{begin, end};
}

/* ************************************************************************ */

/**
 * @brief Make value range (for-loop like).
 *
 * @param begin Begin value.
 * @param end   End value.
 *
 * @return
 */
template<typename T>
CONSTEXPR IteratorRange<Iterator<T>> range(T begin, T end) NOEXCEPT
{
    return IteratorRange<Iterator<T>>{begin, end};
}

/* ************************************************************************ */

#ifndef _MSC_VER
}
#endif

/* ************************************************************************ */

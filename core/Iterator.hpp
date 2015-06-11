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

/* ************************************************************************ */

#ifndef _MSC_VER
inline namespace core {
#endif

/* ************************************************************************ */

/**
 * @brief Iterator class.
 */
template<typename T>
class Iterator
{

// Public Types
public:


    /// Iterator traits
    using iterator_category = std::input_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = value_type*;
    using reference = value_type&;


// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param value.
     */
    explicit Iterator(value_type value) NOEXCEPT
        : m_value(std::move(value))
    {
        // Nothing to do
    }


// Public Operators
public:


    /**
     * @brief Compares two iterators.
     *
     * @param rhs Second iterator.
     *
     * @return Are iterators equal?
     */
    bool operator==(const Iterator& rhs) const NOEXCEPT
    {
        return m_value == rhs.m_value;
    }


    /**
     * @brief Compares two iterators.
     *
     * @param rhs Second iterator.
     *
     * @return Aren't iterators equal?
     */
    bool operator!=(const Iterator& rhs) const NOEXCEPT
    {
        return !operator==(rhs);
    }


    /**
     * @brief Dereference operator.
     *
     * @return A reference to the current token.
     */
    reference operator*() NOEXCEPT
    {
        return m_value;
    }


    /**
     * @brief Dereference operator.
     *
     * @return A pointer to the current token.
     */
    pointer operator->() const NOEXCEPT
    {
        return &m_value;
    }


    /**
     * @brief Pre-increment operator.
     *
     * @return Reference to this.
     */
    Iterator& operator++()
    {
        ++m_value;
        return *this;
    }


    /**
     * @brief Post-increment operator.
     *
     * @note Is deleted (because I'm to lazy to write it).
     */
    Iterator operator++(int) = delete;


// Private Data Members
private:


    /// Current value.
    value_type m_value;

};

/* ************************************************************************ */

#ifndef _MSC_VER
}
#endif

/* ************************************************************************ */

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
class ValueIterator
{

// Public Types
public:


    /// Iterator traits
    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = value_type*;
    using reference = value_type&;


// Public Structures
public:


    /**
     * @brief Reference proxy.
     */
    class ReferenceProxy
    {
    // Public Ctors & Dtors
    public:


        /**
         * @brief Constructor.
         *
         * @param iter
         */
        ReferenceProxy(const ValueIterator& iter) NOEXCEPT
          : m_iter(iter)
        {
            // Nothing to do
        }


    // Public Operators
    public:


        /**
         * @brief Cast to reference operator.
         */
        operator reference() const NOEXCEPT
        {
            return *m_iter;
        }


        /**
         * @brief Assignment operator.
         *
         * @param val value to assign.
         *
         * @return *this.
         */
        ReferenceProxy& operator=(const value_type& val)
        {
            *m_iter = val;
            return *this;
        }


    // Private Data Members
    private:

        /// Iterator value.
        ValueIterator m_iter;
    };



// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param value.
     */
    explicit ValueIterator(value_type value) NOEXCEPT
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
    bool operator==(const ValueIterator& rhs) const NOEXCEPT
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
    bool operator!=(const ValueIterator& rhs) const NOEXCEPT
    {
        return !operator==(rhs);
    }


    /**
     * @brief Compares two iterators.
     *
     * @param rhs Second iterator.
     *
     * @return
     */
    bool operator<(const ValueIterator& rhs) const NOEXCEPT
    {
        return m_value < rhs.m_value;
    }


    /**
     * @brief Compares two iterators.
     *
     * @param rhs Second iterator.
     *
     * @return
     */
    bool operator<=(const ValueIterator& rhs) const NOEXCEPT
    {
        return !operator>(rhs);
    }


    /**
     * @brief Compares two iterators.
     *
     * @param rhs Second iterator.
     *
     * @return
     */
    bool operator>(const ValueIterator& rhs) const NOEXCEPT
    {
        return rhs.operator<(*this);
    }


    /**
     * @brief Compares two iterators.
     *
     * @param rhs Second iterator.
     *
     * @return
     */
    bool operator>=(const ValueIterator& rhs) const NOEXCEPT
    {
        return !operator<(rhs);
    }


    /**
     * @brief Move to next n-th element.
     *
     * @param n Number of steps.
     *
     * @return *this.
     */
    ValueIterator& operator+=(difference_type n) NOEXCEPT
    {
        m_value += n;
        return *this;
    }


    /**
     * @brief Move to next n-th element.
     *
     * @param n Number of steps.
     *
     * @return Copy of *this.
     */
    ValueIterator operator+(difference_type n) const NOEXCEPT
    {
        return ValueIterator(m_value + n);
    }


    /**
     * @brief Move to previous n-th element.
     *
     * @param n Number of steps.
     *
     * @return *this.
     */
    ValueIterator& operator-=(difference_type n) NOEXCEPT
    {
        m_value -= n;
        return *this;
    }


    /**
     * @brief Move to previous n-th element.
     *
     * @param n Number of steps.
     *
     * @return Copy of *this.
     */
    ValueIterator operator-(difference_type n) const NOEXCEPT
    {
        return ValueIterator(m_value - n);
    }


    /**
     * @brief Calculate distance between iterators.
     *
     * @param rhs Second iterator.
     *
     * @return Distance.
     */
    difference_type operator-(const ValueIterator& rhs) const NOEXCEPT
    {
        return m_value - rhs.m_value;
    }


    /**
     * @brief Return reference to next n-th value.
     *
     * @param n
     *
     * @return
     */
    ReferenceProxy operator[](difference_type n)
    {
        return ReferenceProxy(ValueIterator(m_value + n));
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
    ValueIterator& operator++()
    {
        ++m_value;
        return *this;
    }


    /**
     * @brief Post-increment operator.
     */
    ValueIterator operator++(int) const
    {
        ValueIterator tmp(*this);
        operator++();
        return tmp;
    }


    /**
     * @brief Pre-decrement operator.
     *
     * @return Reference to this.
     */
    ValueIterator& operator--()
    {
        --m_value;
        return *this;
    }


    /**
     * @brief Post-decrement operator.
     */
    ValueIterator operator--(int) const
    {
        ValueIterator tmp(*this);
        operator--();
        return tmp;
    }


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

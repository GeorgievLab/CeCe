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
#include <cstddef>
#include <iterator>
#include <utility>

/* ************************************************************************ */

namespace cece {
inline namespace core {

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
        ReferenceProxy(const ValueIterator& iter) noexcept
          : m_iter(iter)
        {
            // Nothing to do
        }


    // Public Operators
    public:


        /**
         * @brief Cast to reference operator.
         */
        operator reference() const noexcept
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
    explicit ValueIterator(value_type value) noexcept
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
    bool operator==(const ValueIterator& rhs) const noexcept
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
    bool operator!=(const ValueIterator& rhs) const noexcept
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
    bool operator<(const ValueIterator& rhs) const noexcept
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
    bool operator<=(const ValueIterator& rhs) const noexcept
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
    bool operator>(const ValueIterator& rhs) const noexcept
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
    bool operator>=(const ValueIterator& rhs) const noexcept
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
    ValueIterator& operator+=(difference_type n) noexcept
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
    ValueIterator operator+(difference_type n) const noexcept
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
    ValueIterator& operator-=(difference_type n) noexcept
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
    ValueIterator operator-(difference_type n) const noexcept
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
    difference_type operator-(const ValueIterator& rhs) const noexcept
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
    reference operator*() noexcept
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

}
}

/* ************************************************************************ */

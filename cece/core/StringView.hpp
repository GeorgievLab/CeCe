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
#include <cstring>

// CeCe
#include "cece/core/String.hpp"
#include "cece/core/OutStream.hpp"
#include "cece/core/InStream.hpp"
#include "cece/core/StaticArray.hpp"
#include "cece/core/DynamicArray.hpp"

/* ************************************************************************ */

namespace cece {
inline namespace core {

/* ************************************************************************ */

/**
 * @brief View for string independent on string storage.
 */
class StringView
{

// Public Types
public:


    /// Character type.
    using CharType = char;

    /// Sequence length type.
    using LengthType = unsigned long;

    /// Sequence position type.
    using PositionType = unsigned long;


// Public Ctors & Dtors
public:


    /**
     * @brief Default constructor.
     */
    StringView() = default;


    /**
     * @brief Constructor.
     *
     * @param ptr Sequence start pointer.
     * @param len Sequence length.
     */
    StringView(const CharType* ptr, LengthType len) noexcept
        : m_ptr(ptr)
        , m_length(len)
    {
        // Nothing to do
    }


    /**
     * @brief Constructor.
     *
     * @param ptr Sequence start pointer.
     */
    StringView(const CharType* ptr) noexcept
        : m_ptr(ptr)
        , m_length(strlen(ptr))
    {
        // Nothing to do
    }


    /**
     * @brief Constructor.
     *
     * @param str String.
     */
    StringView(const String& str) noexcept
        : m_ptr(str.c_str())
        , m_length(str.length())
    {
        // Nothing to do
    }


    /**
     * @brief Constructor.
     *
     * @param str String.
     */
    StringView(const DynamicArray<CharType>& str) noexcept
        : m_ptr(str.data())
        , m_length(str.size())
    {
        // Nothing to do
    }


    /**
     * @brief Constructor.
     *
     * @param str String.
     */
    template<std::size_t N>
    StringView(const StaticArray<CharType, N>& str) noexcept
        : m_ptr(str.data())
        , m_length(str.size())
    {
        // Nothing to do
    }


// Public Operators
public:


    /**
     * @brief Get character at given position.
     *
     * @param pos Character position.
     *
     * @return
     */
    CharType operator[](PositionType pos) const noexcept
    {
        return m_ptr[pos];
    }


    /**
     * @brief Cast to String.
     */
    explicit operator String() const noexcept
    {
        return String(m_ptr, m_length);
    }


// Public Accessors
public:


    /**
     * @brief Returns sequence data.
     *
     * @return
     */
    const CharType* getData() const noexcept
    {
        return m_ptr;
    }


    /**
     * @brief Returns sequence length.
     *
     * @return
     */
    LengthType getLength() const noexcept
    {
        return m_length;
    }


// Private Data Members
private:

    /// Start of the string.
    const CharType* m_ptr = nullptr;

    /// Length of the string.
    LengthType m_length = 0;
};

/* ************************************************************************ */

/**
 * @brief Compare operator.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return
 */
inline bool operator==(const StringView& lhs, const StringView& rhs) noexcept
{
    // Data pointer and length match -> same views.
    if (lhs.getData() == rhs.getData() && lhs.getLength() == rhs.getLength())
        return true;

    // Different lengths
    if (lhs.getLength() != rhs.getLength())
        return false;

    // Compare all characters
    return !strncmp(lhs.getData(), rhs.getData(), lhs.getLength());
}

/* ************************************************************************ */

/**
 * @brief Compare operator.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return
 */
inline bool operator!=(const StringView& lhs, const StringView& rhs)
{
    return !operator==(lhs, rhs);
}

/* ************************************************************************ */

/**
 * @brief Output stream operator for StringView.
 *
 * @param os   Output stream.
 * @param view View to print.
 *
 * @return os.
 */
inline OutStream& operator<<(OutStream& os, const StringView& view) noexcept
{
    return os.write(view.getData(), view.getLength());
}

/* ************************************************************************ */

/**
 * @brief Input stream operator for StringView.
 *
 * @note StringView cannot be used as string storage.
 *
 * @param is   Input stream.
 * @param view Output view.
 *
 * @return is.
 */
inline InStream& operator<<(InStream& is, StringView& view) noexcept = delete;

/* ************************************************************************ */

}
}

/* ************************************************************************ */

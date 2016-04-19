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
#include <cstdint>

// CeCe
#include "cece/core/OutStream.hpp"
#include "cece/core/String.hpp"
#include "cece/core/StringView.hpp"
#include "cece/core/StaticArray.hpp"
#include "cece/core/Vector.hpp"

/* ************************************************************************ */

namespace cece {
inline namespace core {

/* ************************************************************************ */


/**
 * @brief Class designed for storing binary data in stream.
 */
class BinaryOutput
{

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param output Output stream.
     */
    explicit BinaryOutput(OutStream& output) noexcept
        : m_output(output)
    {
        // Nothing to do
    }


// Public Accessors
public:


    /**
     * @brief Returns output stream.
     *
     * @return
     */
    OutStream& os() const
    {
        return m_output;
    }


// Public Operations
public:


    /**
     * @brief Write value binary data.
     *
     * @tparam T Type of stored value.
     *
     * @param val.
     */
    template<typename T>
    void write(T val)
    {
        os().write(reinterpret_cast<char*>(&val), sizeof(T));
    }


    /**
     * @brief Write an array of values as binary data.
     *
     * @tparam T Type of stored array elements.
     *
     * @param val   Stored array.
     * @param count Number of elements.
     */
    template<typename T>
    void write(const T* val, unsigned count)
    {
        os().write(reinterpret_cast<const char*>(val), sizeof(T) * count);
    }


    /**
     * @brief Write boolean value.
     *
     * @param val
     */
    void write(bool val)
    {
        write<char>(val ? '\1' : '\0');
    }


    /**
     * @brief Write character value.
     *
     * @param val
     */
    void write(char val)
    {
        write<char>(val);
    }


    /**
     * @brief Write character value.
     *
     * @param val
     */
    void write(unsigned char val)
    {
        write<unsigned char>(val);
    }


    /**
     * @brief Write short value.
     *
     * @param val
     */
    void write(short val)
    {
        write<int16_t>(val);
    }


    /**
     * @brief Write unsigned short value.
     *
     * @param val
     */
    void write(unsigned short val)
    {
        write<uint16_t>(val);
    }


    /**
     * @brief Write integer value.
     *
     * @param val
     */
    void write(int val)
    {
        write<int32_t>(val);
    }


    /**
     * @brief Write unsigned integer value.
     *
     * @param val
     */
    void write(unsigned int val)
    {
        write<uint32_t>(val);
    }


    /**
     * @brief Write long value.
     *
     * @param val
     */
    void write(long val)
    {
        write<int64_t>(val);
    }


    /**
     * @brief Write unsigned long value.
     *
     * @param val
     */
    void write(unsigned long val)
    {
        write<uint64_t>(val);
    }


    /**
     * @brief Write float value.
     *
     * @param val
     */
    void write(float val)
    {
        write<float>(val);
    }


    /**
     * @brief Write double value.
     *
     * @param val
     */
    void write(double val)
    {
        write<double>(val);
    }


    /**
     * @brief Write string value.
     *
     * @param val
     */
    void write(const String& val)
    {
        write<String::value_type>(val.c_str(), val.length() + 1);
    }


    /**
     * @brief Write string view value.
     *
     * @param val
     */
    void write(const StringView& val)
    {
        write<String::value_type>(val.getData(), val.getLength() + 1);
    }


    /**
     * @brief Write array value.
     *
     * @tparam T
     * @tparam N
     *
     * @param val
     */
    template<typename T, size_t N>
    void write(const StaticArray<T, N>& val)
    {
        for (size_t i = 0; i < N; ++i)
            write(val[i]);
    }


    /**
     * @brief Write vector value.
     *
     * @tparam T
     *
     * @param val
     */
    template<typename T>
    void write(const Vector<T>& val)
    {
        for (unsigned i = 0u; i < Vector<T>::SIZE; ++i)
            write(val[i]);
    }


// Private Data Members
private:

    /// Output stream.
    OutStream& m_output;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */

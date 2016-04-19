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
#include "cece/core/InStream.hpp"
#include "cece/core/String.hpp"
#include "cece/core/StaticArray.hpp"
#include "cece/core/Vector.hpp"

/* ************************************************************************ */

namespace cece {
inline namespace core {

/* ************************************************************************ */


/**
 * @brief Class designed for loading binary data from stream.
 */
class BinaryInput
{

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param input Input stream.
     */
    explicit BinaryInput(InStream& input) noexcept
        : m_input(input)
    {
        // Nothing to do
    }


// Public Accessors
public:


    /**
     * @brief Returns input stream.
     *
     * @return
     */
    InStream& is() const
    {
        return m_input;
    }


// Public Operations
public:


    /**
     * @brief Read value binary data.
     *
     * @tparam T Type of stored value.
     *
     * @param val.
     */
    template<typename T>
    void read(T& val)
    {
        is().read(reinterpret_cast<char*>(&val), sizeof(T));
    }


    /**
     * @brief Read an array of values as binary data.
     *
     * @tparam T Type of stored array elements.
     *
     * @param val   Stored array.
     * @param count Number of elements.
     */
    template<typename T>
    void read(T* val, unsigned count)
    {
        is().read(reinterpret_cast<char*>(val), sizeof(T) * count);
    }


    /**
     * @brief Read value as different type and cast it to the required type.
     *
     * @tparam Tfrom Type that is read from stream.
     * @tparam Tto   Result type.
     *
     * @param val
     */
    template<typename Tfrom, typename Tto>
    void readCast(Tto& val)
    {
        Tfrom c{};
        read<Tfrom>(c);
        val = static_cast<Tto>(c);
    }


    /**
     * @brief Read boolean value.
     *
     * @param val
     */
    void read(bool& val)
    {
        readCast<char>(val);
    }


    /**
     * @brief Read character value.
     *
     * @param val
     */
    void read(char& val)
    {
        read<char>(val);
    }


    /**
     * @brief Read character value.
     *
     * @param val
     */
    void read(unsigned char& val)
    {
        read<unsigned char>(val);
    }


    /**
     * @brief Read short value.
     *
     * @param val
     */
    void read(short& val)
    {
        readCast<int16_t>(val);
    }


    /**
     * @brief Read unsigned short value.
     *
     * @param val
     */
    void read(unsigned short& val)
    {
        readCast<uint16_t>(val);
    }


    /**
     * @brief Read integer value.
     *
     * @param val
     */
    void read(int& val)
    {
        readCast<int32_t>(val);
    }


    /**
     * @brief Read unsigned integer value.
     *
     * @param val
     */
    void read(unsigned int& val)
    {
        readCast<uint32_t>(val);
    }


    /**
     * @brief Read long value.
     *
     * @param val
     */
    void read(long& val)
    {
        readCast<int64_t>(val);
    }


    /**
     * @brief Read unsigned long value.
     *
     * @param val
     */
    void read(unsigned long& val)
    {
        readCast<uint64_t>(val);
    }


    /**
     * @brief Read float value.
     *
     * @param val
     */
    void read(float& val)
    {
        readCast<float>(val);
    }


    /**
     * @brief Read double value.
     *
     * @param val
     */
    void read(double& val)
    {
        readCast<double>(val);
    }


    /**
     * @brief Read string value.
     *
     * @param val
     */
    void read(String& val)
    {
        val.clear();

        for (auto c = is().get(); c != '\0'; c = is().get())
            val.push_back(c);
    }


    /**
     * @brief Read array value.
     *
     * @tparam T
     * @tparam N
     *
     * @param val
     */
    template<typename T, size_t N>
    void read(StaticArray<T, N>& val)
    {
        for (size_t i = 0; i < N; ++i)
            read(val[i]);
    }


    /**
     * @brief Read vector value.
     *
     * @tparam T
     *
     * @param val
     */
    template<typename T>
    void read(Vector<T>& val)
    {
        for (unsigned i = 0u; i < Vector<T>::SIZE; ++i)
            read(val[i]);
    }


// Private Data Members
private:

    /// Input stream.
    InStream& m_input;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */

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
#include <type_traits>

/* ************************************************************************ */

namespace cece {
namespace render {

/* ************************************************************************ */

/**
 * @brief Byte type.
 */
using byte = unsigned char;

/* ************************************************************************ */

/**
 * @brief Defines data type.
 */
enum class DataType
{
    Undefined = 0,
    Int,
    Uint,
    Short,
    Ushort,
    Byte,
    Ubyte,
    Float,
    Double
};

/* ************************************************************************ */

/**
 * @brief Returns DataType constant for real type.
 */
template<typename T>
struct to_data_type : std::integral_constant<DataType, DataType::Undefined> {};

/* ************************************************************************ */

template<>
struct to_data_type<int> : std::integral_constant<DataType, DataType::Int> {};

/* ************************************************************************ */

template<>
struct to_data_type<unsigned int> : std::integral_constant<DataType, DataType::Uint> {};

/* ************************************************************************ */

template<>
struct to_data_type<short> : std::integral_constant<DataType, DataType::Short> {};

/* ************************************************************************ */

template<>
struct to_data_type<unsigned short> : std::integral_constant<DataType, DataType::Ushort> {};

/* ************************************************************************ */

template<>
struct to_data_type<char> : std::integral_constant<DataType, DataType::Byte> {};

/* ************************************************************************ */

template<>
struct to_data_type<unsigned char> : std::integral_constant<DataType, DataType::Ubyte> {};

/* ************************************************************************ */

template<>
struct to_data_type<float> : std::integral_constant<DataType, DataType::Float> {};

/* ************************************************************************ */

template<>
struct to_data_type<double> : std::integral_constant<DataType, DataType::Double> {};

/* ************************************************************************ */

/**
 * @brief Returns size of given data type.
 *
 * @param type Data type.
 * @return Size of type.
 */
inline constexpr size_t data_type_size(DataType type) noexcept
{
    // Very ugly code, but can be evaluated at compile-time
    return
        (type == DataType::Int) ? sizeof(int) :
        (type == DataType::Uint) ? sizeof(unsigned int) :
        (type == DataType::Short) ? sizeof(short) :
        (type == DataType::Ushort) ? sizeof(unsigned short) :
        (type == DataType::Byte) ? sizeof(char) :
        (type == DataType::Ubyte) ? sizeof(unsigned char) :
        (type == DataType::Float) ? sizeof(float) :
        (type == DataType::Double) ? sizeof(double) :
        0
    ;
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

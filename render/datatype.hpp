
#pragma once

/* ************************************************************************ */

// C++
#include <cstddef>
#include <type_traits>

// Simulator
#include "core/compatibility.hpp"

/* ************************************************************************ */

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
inline CONSTEXPR size_t data_type_size(DataType type) NOEXCEPT
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

/* ************************************************************************ */

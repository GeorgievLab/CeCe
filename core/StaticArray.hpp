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
#include <array>

/* ************************************************************************ */

#ifndef _MSC_VER
inline namespace core {
#endif

/* ************************************************************************ */

/**
 * @brief Static (stack allocated) array.
 *
 * @tparam T Element type.
 * @tparam N Number of elements.
 */
template<typename T, std::size_t N>
using StaticArray = std::array<T, N>;

/* ************************************************************************ */

#ifndef _MSC_VER
}
#endif

/* ************************************************************************ */

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
#include <vector>

/* ************************************************************************ */

#ifndef _MSC_VER
inline namespace core {
#endif

/* ************************************************************************ */

/**
 * @brief Dynamic array.
 *
 * @tparam T
 * @tparam Allocator
 */
template<typename T, class Allocator = std::allocator<T>>
using DynamicArray = std::vector<T, Allocator>;

/* ************************************************************************ */

#ifndef _MSC_VER
}
#endif

/* ************************************************************************ */

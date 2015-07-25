/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
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
#include <utility>

/* ************************************************************************ */

inline namespace core {

/* ************************************************************************ */

/**
 * @brief Pair type.
 */
template<typename T1, typename T2>
using Pair = std::pair<T1, T2>;

/* ************************************************************************ */

/**
 * @brief Creates a Pair object, deducing the target type from the types of arguments.
 *
 * @tparam T1
 * @tparam T2
 *
 * @param v1
 * @param v2
 *
 * @return
 */
template<typename T1, typename T2>
auto makePair(T1&& v1, T2&& v2) -> decltype(std::make_pair(v1, v2))
{
    return std::make_pair(v1, v2);
}

/* ************************************************************************ */

}

/* ************************************************************************ */

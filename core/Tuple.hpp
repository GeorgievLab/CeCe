/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */

#pragma once

/* ************************************************************************ */

// C++
#include <tuple>

/* ************************************************************************ */

inline namespace core {

/* ************************************************************************ */

/**
 * @brief Tuple type.
 *
 * @tparam Args Arguments type.
 */
template<typename... Args>
using Tuple = std::tuple<Args...>;

/* ************************************************************************ */

/**
 * @brief Access function for Tuple.
 *
 * @param t Tuple.
 *
 * @return
 */
template<unsigned I, class... Types>
constexpr auto get(Tuple<Types...>& t) -> decltype(std::get<I>(t))
{
   return std::get<I>(t);
}

/* ************************************************************************ */

/**
 * @brief Access function for Tuple.
 *
 * @param t Tuple.
 *
 * @return
 */
template<unsigned I, class... Types>
constexpr auto get(Tuple<Types...>&& t) -> decltype(std::get<I>(t))
{
   return std::get<I>(t);
}

/* ************************************************************************ */

/**
 * @brief Access function for Tuple.
 *
 * @param t Tuple.
 *
 * @return
 */
template<unsigned I, class... Types>
constexpr auto get(const Tuple<Types...>& t) -> decltype(std::get<I>(t))
{
   return std::get<I>(t);
}

/* ************************************************************************ */

}

/* ************************************************************************ */

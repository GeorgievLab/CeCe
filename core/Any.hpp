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
#include <boost/any.hpp>

/* ************************************************************************ */

inline namespace core {

/* ************************************************************************ */

/**
 * @brief Type for storing any value.
 */
using Any = boost::any;

/* ************************************************************************ */

/**
 * @brief Any cast.
 *
 * @tparam T Type of required object.
 *
 * @param any Any object.
 *
 * @return Value of required object.
 */
template<typename T>
T AnyCast(Any& any)
{
    return boost::any_cast<T>(any);
}

/* ************************************************************************ */

/**
 * @brief Any cast.
 *
 * @tparam T Type of required object.
 *
 * @param any Any object.
 *
 * @return Value of required object.
 */
template<typename T>
T AnyCast(Any&& any)
{
    return boost::any_cast<T>(any);
}

/* ************************************************************************ */

/**
 * @brief Any cast.
 *
 * @tparam T Type of required object.
 *
 * @param any Any object.
 *
 * @return Value of required object.
 */
template<typename T>
T AnyCast(const Any& any)
{
    return boost::any_cast<T>(any);
}

/* ************************************************************************ */

/**
 * @brief Any cast.
 *
 * @tparam T Type of required object.
 *
 * @param any Any object.
 *
 * @return Value of required object.
 */
template<typename T>
const T* AnyCast(const Any* any)
{
    return boost::any_cast<T>(any);
}

/* ************************************************************************ */

/**
 * @brief Any cast.
 *
 * @tparam T Type of required object.
 *
 * @param any Any object.
 *
 * @return Value of required object.
 */
template<typename T>
T* AnyCast(Any* any)
{
    return boost::any_cast<T>(any);
}

/* ************************************************************************ */

}

/* ************************************************************************ */

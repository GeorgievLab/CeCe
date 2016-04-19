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
#include <boost/any.hpp>

/* ************************************************************************ */

namespace cece {
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
}

/* ************************************************************************ */

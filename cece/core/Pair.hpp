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
#include <utility>

/* ************************************************************************ */

namespace cece {
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
}

/* ************************************************************************ */

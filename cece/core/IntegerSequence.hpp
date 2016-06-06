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
#include <type_traits>

/* ************************************************************************ */

namespace cece {
inline namespace core {

/* ************************************************************************ */

/**
 * @brief Sequence of integers.
 *
 * @tparam Integer Integer type.
 * @tparam I       A list of integers.
 *
 * @todo Replace with std::integer_sequence in C++14
 */
template<typename Integer, Integer... I>
struct IntegerSequence { };

/* ************************************************************************ */

/**
 * @brief Sequence number range generator. The generated range is [S, E - 1].
 *
 * @tparam S Starting number.
 * @tparam E Number after last generated.
 * @tparam I... Helper list for recursive generating numbers.
 *
 * @see MakeIntegerSequence
 *
 * The idea is based on link below.
 *
 * @link http://stackoverflow.com/questions/7858817/unpacking-a-tuple-to-call-a-matching-function-pointer/7858971#7858971
 */
template<typename Integer, Integer S, Integer E, Integer... I>
struct IntegerSequenceGenerator
{
    static_assert(S <= E, "S <= E !!!");

    /// Generated `IntegerSequence` type.
    using type = typename std::conditional<
        S >= E,
        // This is required for prevent instantization of the second type
        // that causes recursive instantizations
        std::conditional<true, IntegerSequence<Integer, I...>, void>,
        IntegerSequenceGenerator<Integer, S, E - 1, E - 1, I...>
    >::type::type;
};

/* ************************************************************************ */

// Just some tests
//static_assert(std::is_same<typename IntegerSequenceGenerator<int, 0, 0>::type, IntegerSequence<int>>::value, "Fail [0, 0]");
//static_assert(std::is_same<typename IntegerSequenceGenerator<int, 0, 1>::type, IntegerSequence<int, 0>>::value, "Fail [0, 1]");
//static_assert(std::is_same<typename IntegerSequenceGenerator<int, 5, 10>::type, IntegerSequence<int, 5, 6, 7, 8, 9>>::value, "Fail [5, 10]");

/* ************************************************************************ */

/**
 * @brief Alias type for generating numbers range.
 *
 * @tparam S Starting number.
 * @tparam E Number after last generated.
 */
template<typename Integer, Integer S, Integer E>
using MakeIntegerSequence = typename IntegerSequenceGenerator<Integer, S, E>::type;

/* ************************************************************************ */

}
}

/* ************************************************************************ */

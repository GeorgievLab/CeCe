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
#include <type_traits>

/* ************************************************************************ */

#ifndef _MSC_VER
inline namespace core {
#endif

/* ************************************************************************ */

/**
 * @brief Sequence of integers.
 *
 * @tparam I A list of integers.
 *
 * @todo Replace with std::integer_sequence in C++14
 */
template<int... I>
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
template<int S, int E, int... I>
struct IntegerSequenceGenerator
{
    static_assert(S <= E, "S <= E !!!");

    /// Generated `IntegerSequence` type.
    using type = typename std::conditional<
        S >= E,
        // This is required for prevent instantization of the second type
        // that causes recursive instantizations
        std::conditional<true, IntegerSequence<I...>, void>,
        IntegerSequenceGenerator<S, E - 1, E - 1, I...>
    >::type::type;
};

/* ************************************************************************ */

// Just some tests
static_assert(std::is_same<typename IntegerSequenceGenerator<0, 0>::type, IntegerSequence<>>::value, "Fail [0, 0]");
static_assert(std::is_same<typename IntegerSequenceGenerator<0, 1>::type, IntegerSequence<0>>::value, "Fail [0, 1]");
static_assert(std::is_same<typename IntegerSequenceGenerator<5, 10>::type, IntegerSequence<5, 6, 7, 8, 9>>::value, "Fail [5, 10]");

/* ************************************************************************ */

/**
 * @brief Alias type for generating numbers range.
 *
 * @tparam S Starting number.
 * @tparam E Number after last generated.
 */
template<int S, int E>
using MakeIntegerSequence = typename IntegerSequenceGenerator<S, E>::type;

/* ************************************************************************ */

#ifndef _MSC_VER
}
#endif

/* ************************************************************************ */

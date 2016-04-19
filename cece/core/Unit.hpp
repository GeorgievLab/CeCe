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
#include <cmath>
#include <limits>
#include <type_traits>

// CeCe
#include "cece/core/Real.hpp"
#include "cece/core/Zero.hpp"
#include "cece/core/String.hpp"
#include "cece/core/StringView.hpp"
#include "cece/core/StaticArray.hpp"

/* ************************************************************************ */

namespace cece {
inline namespace core {

/* ************************************************************************ */

namespace units {

/* ************************************************************************ */

/**
 * @brief Basic value.
 */
using Value = RealType;

/* ************************************************************************ */

/// Base units exponents
static constexpr int LENGTH_EXPONENT = 6;
static constexpr int TIME_EXPONENT = 0;
static constexpr int MASS_EXPONENT = 6;
static constexpr int ELECTRIC_CURRENT_EXPONENT = 0;
static constexpr int THERMODYNAMIC_TEMPERATURE_EXPONENT = 0;
static constexpr int AMOUNT_OF_SUBSTANCE_EXPONENT = 6;
static constexpr int LUMINOUS_INTENSITY_EXPONENT = 0;

/* ************************************************************************ */

/// @link https://azraelplanet.wordpress.com/2012/03/30/number-of-arguments-in-__va_args__-gcc/
#define ARG_N(_1, _2, _3, N, ...) N
#define NARG_(...) ARG_N(__VA_ARGS__)
#define NARG(...) NARG_(__VA_ARGS__, 3, 2, 1, 0)

/* ************************************************************************ */

/**
 * @brief Defines base unit.
 *
 * @param name Unit Name.
 * @param ord  Unit order.
 * @param exp  Value coefficient exponent.
 * @param ...  Characters of unit symbol.
 */
#define DEFINE_BASE_UNIT(name, exp, ord, ...) \
    struct Base ## name {\
        static constexpr int exponent = exp; \
        static constexpr int order = ord;\
        static constexpr std::size_t symbolLength = NARG(__VA_ARGS__) + 1; \
        static constexpr StaticArray<char, symbolLength> getSymbol() noexcept \
        { \
            return {{__VA_ARGS__, '\0'}};\
        } \
    }

/* ************************************************************************ */

/**
 * @brief Base SI units.
 */
DEFINE_BASE_UNIT(Length,                   LENGTH_EXPONENT,                    0, 'm');
DEFINE_BASE_UNIT(Time,                     TIME_EXPONENT,                      1, 's');
DEFINE_BASE_UNIT(Mass,                     MASS_EXPONENT,                      2, 'g');
DEFINE_BASE_UNIT(ElectricCurrent,          ELECTRIC_CURRENT_EXPONENT,          3, 'A');
DEFINE_BASE_UNIT(ThermodynamicTemperature, THERMODYNAMIC_TEMPERATURE_EXPONENT, 4, 'K');
DEFINE_BASE_UNIT(AmountOfSubstance,        AMOUNT_OF_SUBSTANCE_EXPONENT,       5, 'm', 'o', 'l');
DEFINE_BASE_UNIT(LuminousIntensity,        LUMINOUS_INTENSITY_EXPONENT,        6, 'c', 'd');

/* ************************************************************************ */

/**
 * @brief Calculate coefficient from exponent.
 *
 * @param exponent
 *
 * @return Result coefficient
 */
inline constexpr Value exponentToCoefficient(int exponent) noexcept
{
    return (exponent == 0)
        ? 1
        : (exponent > 0)
            ? 10 * exponentToCoefficient(exponent - 1)
            : 0.1 * exponentToCoefficient(exponent + 1)
    ;
}

/* ************************************************************************ */

/**
 * @brief Less structure.
 *
 * @tparam T1 First base unit type.
 * @tparam T2 Second base unit type.
 */
template<typename T1, typename T2>
struct Less
{
    /// If T1 is less than T2
    static constexpr bool value = T1::order < T2::order;
};

/* ************************************************************************ */

/**
 * @brief List of types.
 *
 * @tparam Types A list of types.
 */
template<typename... Types>
struct List
{

    /// Number of types in list.
    static constexpr std::size_t size = sizeof...(Types);

};

/* ************************************************************************ */

/**
 * @brief Struct for calculating total coefficient exponent of given list.
 *
 * @tparam Types A list of unit types.
 */
template<typename... Types>
struct Exponent;

/* ************************************************************************ */

/**
 * @brief Struct for calculating total coefficient exponent of given list.
 *
 * @tparam Types A list of unit types.
 */
template<typename... Types>
struct Exponent<List<Types...>>
{

    /**
     * @brief Add coefficient exponents.
     *
     * @param arg  The first coefficient exponent.
     * @param args Rest of the coefficient exponents.
     *
     * @return Result exponent.
     */
    template<typename Arg, typename... Args>
    static constexpr int add(Arg&& arg, Args&&... args) noexcept
    {
        return arg + add(args...);
    }


    /**
     * @brief Add coefficient exponents.
     *
     * @return 1.
     */
    static constexpr int add() noexcept
    {
        return 0;
    }


    /// Calculate types coefficient exponent.
    static constexpr int value = add(Types::exponent...);

};

/* ************************************************************************ */

/**
 * @brief Concatenate two lists.
 *
 * @tparam T Types.
 */
template<typename... T>
struct Concat;

/* ************************************************************************ */

/**
 * @brief Concatenated specialization for single list.
 *
 * @tparam Types Types of the list.
 */
template<typename... Types>
struct Concat<List<Types...>>
{
    /// Concatenated list.
    using type = List<Types...>;
};

/* ************************************************************************ */

/**
 * @brief Concatenate two lists.
 *
 * @tparam Types1 Types of the first list.
 * @tparam Types2 Types of the second list.
 * @tparam Tail   Remaining types.
 */
template<typename... Types1, typename... Types2, typename... Tail>
struct Concat<List<Types1...>, List<Types2...>, Tail...>
{
    /// Concatenated list.
    using type = typename Concat<List<Types1..., Types2...>, Tail...>::type;
};

/* ************************************************************************ */

/**
 * @brief Type counter.
 *
 * @tparam T    Type to count.
 * @tparam List List of types.
 */
template<typename T, typename... Types>
struct Counter;

/* ************************************************************************ */

/**
 * @brief Type counter.
 *
 * @tparam T Type to count.
 */
template<typename T>
struct Counter<T, List<>>
{
    /// Number of occurences.
    static constexpr std::size_t value = 0;
};

/* ************************************************************************ */

/**
 * @brief Type counter.
 *
 * @tparam T    Type to count.
 * @tparam Type The first type from types.
 * @tparam List List of types.
 */
template<typename T, typename Type, typename... Types>
struct Counter<T, List<Type, Types...>>
{
    /// Number of occurences.
    static constexpr std::size_t value =
        (std::is_same<T, Type>::value ? 1 : 0) +
        Counter<T, List<Types...>>::value
    ;
};

/* ************************************************************************ */

/**
 * @brief Type counter.
 *
 * @tparam List List of types.
 */
template<typename... Types>
struct CounterFirst;

/* ************************************************************************ */

/**
 * @brief Type counter.
 *
 * @tparam List List of types.
 */
template<>
struct CounterFirst<List<>>
{
    /// Number of occurences.
    static constexpr std::size_t value = 0;
};

/* ************************************************************************ */

/**
 * @brief Type counter.
 *
 * @tparam Type The first type from types.
 * @tparam List List of types.
 */
template<typename Type, typename... Types>
struct CounterFirst<List<Type, Types...>>
{
    /// Number of occurences.
    static constexpr std::size_t value = Counter<Type, List<Type, Types...>>::value;
};

/* ************************************************************************ */

/**
 * @brief Remove type from list.
 *
 * @tparam T    Type to remove.
 * @tparam List List of types.
 */
template<typename T, typename List>
struct Remove;

/* ************************************************************************ */

/**
 * @brief Remove type from list.
 *
 * @tparam T Type to remove.
 */
template<typename T>
struct Remove<T, List<>>
{
    // Not found
    static constexpr bool value = false;

    // Remaining types.
    using type = List<>;
};

/* ************************************************************************ */

/**
 * @brief Remove type element from list.
 *
 * @tparam T     Type to remove.
 * @tparam Type  First type.
 * @tparam Types Remaining types in the list.
 */
template<typename T, typename Type, typename... Types>
struct Remove<T, List<Type, Types...>>
{
    /// If types match.
    static constexpr bool match = std::is_same<T, Type>::value;

    // Inner remove.
    using RemoveInnerType = Remove<T, List<Types...>>;

    // If type is found.
    static constexpr bool value = match || RemoveInnerType::value;

    /// List type without the required type.
    using type = typename std::conditional<match,
        List<Types...>,
        typename Concat<List<Type>, typename RemoveInnerType::type>::type
    >::type;
};

/* ************************************************************************ */

/**
 * @brief Helper class to reduce units.
 *
 * @tparam Nom   Nominators.
 * @tparam Denom Denominators.
 */
template<typename Nom, typename Denom>
struct ReduceInner;

/* ************************************************************************ */

/**
 * @brief Helper class to reduce units.
 *
 * @tparam Denominators List types.
 */
template<typename... Denominators>
struct ReduceInner<List<>, List<Denominators...>>
{
    using nominators = List<>;
    using denominators = List<Denominators...>;
};

/* ************************************************************************ */

/**
 * @brief Helper class to simplify units.
 *
 * Type removes shared types in nominator and denominator.
 *
 * @tparam Nom          First nominator.
 * @tparam Nominators   List types.
 * @tparam Denominators List types.
 */
template<typename Nom, typename... Nominators, typename... Denominators>
struct ReduceInner<List<Nom, Nominators...>, List<Denominators...>>
{
    /// Type of removing type.
    using remove_type = Remove<Nom, List<Denominators...>>;

    // Reduce without the first nominator
    using reduce_inner = ReduceInner<List<Nominators...>, typename remove_type::type>;

    /// List of nominators
    using nominators = typename std::conditional<remove_type::value,
        typename reduce_inner::nominators,
        typename Concat<List<Nom>, typename reduce_inner::nominators>::type
    >::type;

    /// List of denominators
    using denominators = typename reduce_inner::denominators;
};

/* ************************************************************************ */

/**
 * @brief Remove all occurences of given type element from list.
 *
 * @tparam T    Type to remove.
 * @tparam List List of types.
 */
template<typename T, typename List>
struct RemoveAll;

/* ************************************************************************ */

/**
 * @brief Remove all occurences of given type element from list.
 *
 * @tparam T Type to remove.
 */
template<typename T>
struct RemoveAll<T, List<>>
{
    // Not found
    static constexpr bool value = false;

    // Number of occurences.
    static constexpr std::size_t count = 0;

    // Remaining types.
    using type = List<>;
};

/* ************************************************************************ */

/**
 * @brief Remove all occurences of given type element from list.
 *
 * @tparam T     Type to remove.
 * @tparam Type  First type.
 * @tparam Types Remaining types in the list.
 */
template<typename T, typename Type, typename... Types>
struct RemoveAll<T, List<Type, Types...>>
{
    /// If types match.
    static constexpr bool match = std::is_same<T, Type>::value;

    // Inner remove.
    using RemoveInnerType = RemoveAll<T, List<Types...>>;

    // If type is found.
    static constexpr bool value = match || RemoveInnerType::value;

    // Number of occurences.
    static constexpr std::size_t count = (match ? 1 : 0) + RemoveInnerType::count;

    /// List type without the required type.
    using type = typename std::conditional<match,
        typename RemoveInnerType::type,
        typename Concat<List<Type>, typename RemoveInnerType::type>::type
    >::type;
};

/* ************************************************************************ */

/**
 * @brief SI Unit.
 *
 * @tparam Nom   List type.
 * @tparam Denom List type.
 */
template<typename Nom, typename Denom>
class Unit
{

// Public Types
public:


    /// Value type.
    using value_type = Value;

    /// List type.
    using nominator = Nom;

    /// List type.
    using denominator = Denom;

// Public Constants
public:


    /// Total unit coefficient.
    static constexpr int exponent = Exponent<Nom>::value - Exponent<Denom>::value;

    /// Number of occurences of the first nominator.
    static constexpr std::size_t firstCountNom = CounterFirst<Nom>::value;

    /// Number of occurences of the first denominator.
    static constexpr std::size_t firstCountDenom = CounterFirst<Denom>::value;


// Public Ctors & Dtors
public:


    /**
     * @brief Default constructor.
     */
    Unit() = default;


    /**
     * @brief Constructor.
     *
     * @param value Init value.
     */
    explicit constexpr Unit(value_type value) noexcept
        : m_value(value)
    {
        // Nothing to do
    }


    /**
     * @brief Zero constructor.
     */
    constexpr Unit(Zero_t) noexcept
        : m_value(0)
    {
        // Nothing to do
    }


// Public Operators
public:


    /**
     * @brief If value is set operator.
     *
     * @return
     */
    explicit operator bool() const noexcept
    {
        return m_value != 0;
    }


    /**
     * @brief Cast to value type.
     *
     * @return
     */
    explicit operator Value() const noexcept
    {
        return m_value;
    }


    /**
     * @brief Unary plus operator.
     *
     * @return New value.
     */
    Unit operator+() const noexcept
    {
        return Unit(m_value);
    }


    /**
     * @brief Unary minus operator.
     *
     * @return New value.
     */
    Unit operator-() const noexcept
    {
        return Unit(-m_value);
    }


    /**
     * @brief Addition operator.
     *
     * @param rhs
     *
     * @return *this.
     */
    Unit& operator+=(Unit rhs) noexcept
    {
        m_value += rhs.m_value;
        return *this;
    }


    /**
     * @brief Substraction operator.
     *
     * @param rhs Right operand.
     *
     * @return *this.
     */
    Unit& operator-=(Unit rhs) noexcept
    {
        m_value -= rhs.m_value;
        return *this;
    }


    /**
     * @brief Multiplication operator.
     *
     * @param rhs Right operand.
     *
     * @return *this.
     */
    Unit& operator*=(value_type rhs) noexcept
    {
        m_value *= rhs;
        return *this;
    }


    /**
     * @brief Division operator.
     *
     * @param rhs Right operand.
     *
     * @return *this.
     */
    Unit& operator/=(value_type rhs) noexcept
    {
        m_value /= rhs;
        return *this;
    }


// Public Accessors
public:


    /**
     * @brief Returns current value.
     *
     * @return
     */
    constexpr value_type value() const noexcept
    {
        return m_value;
    }


// Private Data Members
private:

    /// Stored value.
    value_type m_value;

};

/* ************************************************************************ */

/**
 * @brief Reduce empty lists.
 */
template<typename Nominators, typename Denominators>
struct ReduceEmpty
{
    /// Result unit type.
    using type = Unit<Nominators, Denominators>;
};

/* ************************************************************************ */

/**
 * @brief Reduce empty lists.
 */
template<>
struct ReduceEmpty<List<>, List<>>
{
    /// Result unit type.
    using type = Value;
};

/* ************************************************************************ */

/**
 * @brief Value filter.
 *
 * @tparam T     Type to filter.
 * @tparam neg   Negate less.
 * @tparam Types Types.
 */
template<typename T, bool neg, typename Types>
struct Filter;

/* ************************************************************************ */

/**
 * @brief Value filter.
 *
 * @tparam T     Type to filter.
 * @tparam neg   Negate less.
 * @tparam Type  First type.
 * @tparam Types Types.
 */
template<typename T, bool neg, typename Type, typename... Types>
struct Filter<T, neg, List<Type, Types...>>
{
    // List without the first type.
    using tail = typename Filter<T, neg, List<Types...>>::type;

    using type = typename std::conditional<
        neg ^ Less<T, Type>::value,
        typename Concat<List<Type>, tail>::type,
        tail
    >::type;
};

/* ************************************************************************ */

/**
 * @brief Value filter.
 *
 * @tparam T     Type to filter.
 * @tparam neg   Negate less.
 */
template<typename T, bool neg>
struct Filter<T, neg, List<>>
{
    using type = List<>;
};

/* ************************************************************************ */

/**
 * @brief List sorting structure.
 *
 * @tparam Types Types.
 */
template<typename Types>
struct Sort;

/* ************************************************************************ */

/**
 * @brief List sorting structure.
 *
 * @tparam Type  First type.
 * @tparam Types A list of types.
 */
template<typename Type, typename... Types>
struct Sort<List<Type, Types...>>
{
    using front = typename Filter<Type, true, List<Types...>>::type;
    using tail = typename Filter<Type, false, List<Types...>>::type;

    // Sorted list.
    using type = typename Concat<front, List<Type>, tail>::type;
};

/* ************************************************************************ */

/**
 * @brief List sorting structure.
 *
 * @tparam Type Type.
 */
template<typename Type>
struct Sort<List<Type>>
{
    using type = List<Type>;
};

/* ************************************************************************ */

/**
 * @brief List sorting structure.
 */
template<>
struct Sort<List<>>
{
    using type = List<>;
};

/* ************************************************************************ */

/**
 * @brief Helper class to reduce units.
 *
 * @tparam Nom   Nominators.
 * @tparam Denom Denominators.
 */
template<typename Nom, typename Denom>
struct Reduce;

/* ************************************************************************ */

/**
 * @brief Helper class to simplify units.
 *
 * Type removes shared types in nominator and denominator.
 *
 * @tparam Nom          First nominator.
 * @tparam Nominators   List types.
 * @tparam Denominators List types.
 */
template<typename... Nominators, typename... Denominators>
struct Reduce<List<Nominators...>, List<Denominators...>>
{
    // Inner reduce
    using inner = ReduceInner<List<Nominators...>, List<Denominators...>>;

    /// Result unit type.
    using type = typename ReduceEmpty<
        typename Sort<typename inner::nominators>::type,
        typename Sort<typename inner::denominators>::type
    >::type;
};

/* ************************************************************************ */

/**
 * @brief Compare operator.
 *
 * @tparam Nominators   A list of nominators.
 * @tparam Denominators A list of denominators.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result value.
 */
template<typename... Nominators, typename... Denominators>
inline constexpr bool operator==(
    Unit<List<Nominators...>, List<Denominators...>> lhs,
    Unit<List<Nominators...>, List<Denominators...>> rhs
) noexcept
{
    return std::abs(lhs.value() - rhs.value()) < std::numeric_limits<Value>::epsilon();
    //return lhs.value() == rhs.value();
}

/* ************************************************************************ */

/**
 * @brief Compare operator.
 *
 * @tparam Nominators   A list of nominators.
 * @tparam Denominators A list of denominators.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result value.
 */
template<typename... Nominators, typename... Denominators>
inline constexpr bool operator==(
    Unit<List<Nominators...>, List<Denominators...>> lhs, Zero_t rhs
) noexcept
{
    return lhs == Unit<List<Nominators...>, List<Denominators...>>(Zero);
}

/* ************************************************************************ */

/**
 * @brief Compare operator.
 *
 * @tparam Nominators   A list of nominators.
 * @tparam Denominators A list of denominators.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result value.
 */
template<typename... Nominators, typename... Denominators>
inline constexpr bool operator==(
    Zero_t lhs, Unit<List<Nominators...>, List<Denominators...>> rhs
) noexcept
{
    return Unit<List<Nominators...>, List<Denominators...>>(Zero) == rhs;
}

/* ************************************************************************ */

/**
 * @brief Compare operator.
 *
 * @tparam Nominators   A list of nominators.
 * @tparam Denominators A list of denominators.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result value.
 */
template<typename... Nominators, typename... Denominators>
inline constexpr bool operator!=(
    Unit<List<Nominators...>, List<Denominators...>> lhs,
    Unit<List<Nominators...>, List<Denominators...>> rhs
) noexcept
{
    return !operator==(lhs, rhs);
}

/* ************************************************************************ */

/**
 * @brief Compare operator.
 *
 * @tparam Nominators   A list of nominators.
 * @tparam Denominators A list of denominators.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result value.
 */
template<typename... Nominators, typename... Denominators>
inline constexpr bool operator!=(
    Unit<List<Nominators...>, List<Denominators...>> lhs, Zero_t rhs
) noexcept
{
    return !operator==(lhs, rhs);
}

/* ************************************************************************ */

/**
 * @brief Compare operator.
 *
 * @tparam Nominators   A list of nominators.
 * @tparam Denominators A list of denominators.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result value.
 */
template<typename... Nominators, typename... Denominators>
inline constexpr bool operator!=(
    Zero_t lhs, Unit<List<Nominators...>, List<Denominators...>> rhs
) noexcept
{
    return !operator==(lhs, rhs);
}

/* ************************************************************************ */

/**
 * @brief Less operator.
 *
 * @tparam Nominators   A list of nominators.
 * @tparam Denominators A list of denominators.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result value.
 */
template<typename... Nominators, typename... Denominators>
inline constexpr bool operator<(
    Unit<List<Nominators...>, List<Denominators...>> lhs,
    Unit<List<Nominators...>, List<Denominators...>> rhs
) noexcept
{
    return lhs.value() < rhs.value();
}

/* ************************************************************************ */

/**
 * @brief Less operator.
 *
 * @tparam Nominators   A list of nominators.
 * @tparam Denominators A list of denominators.
 *
 * @param lhs Left operand.
 * @param rhs Zero.
 *
 * @return Result value.
 */
template<typename... Nominators, typename... Denominators>
inline constexpr bool operator<(
    Unit<List<Nominators...>, List<Denominators...>> lhs, Zero_t
) noexcept
{
    return lhs.value() < 0;
}

/* ************************************************************************ */

/**
 * @brief Less operator.
 *
 * @tparam Nominators   A list of nominators.
 * @tparam Denominators A list of denominators.
 *
 * @param lhs Zero.
 * @param rhs Right operand.
 *
 * @return Result value.
 */
template<typename... Nominators, typename... Denominators>
inline constexpr bool operator<(
    Zero_t, Unit<List<Nominators...>, List<Denominators...>> rhs
) noexcept
{
    return 0 < rhs.value();
}

/* ************************************************************************ */

/**
 * @brief Less equals operator.
 *
 * @tparam Nominators   A list of nominators.
 * @tparam Denominators A list of denominators.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result value.
 */
template<typename... Nominators, typename... Denominators>
inline constexpr bool operator<=(
    Unit<List<Nominators...>, List<Denominators...>> lhs,
    Unit<List<Nominators...>, List<Denominators...>> rhs
) noexcept
{
    return !operator>(lhs, rhs);
}

/* ************************************************************************ */

/**
 * @brief Less equals operator.
 *
 * @tparam Nominators   A list of nominators.
 * @tparam Denominators A list of denominators.
 *
 * @param lhs Left operand.
 * @param rhs Zero.
 *
 * @return Result value.
 */
template<typename... Nominators, typename... Denominators>
inline constexpr bool operator<=(
    Unit<List<Nominators...>, List<Denominators...>> lhs, Zero_t
) noexcept
{
    return !operator>(lhs, Zero);
}

/* ************************************************************************ */

/**
 * @brief Less equals operator.
 *
 * @tparam Nominators   A list of nominators.
 * @tparam Denominators A list of denominators.
 *
 * @param lhs Zero.
 * @param rhs Right operand.
 *
 * @return Result value.
 */
template<typename... Nominators, typename... Denominators>
inline constexpr bool operator<=(
    Zero_t, Unit<List<Nominators...>, List<Denominators...>> rhs
) noexcept
{
    return !operator>(Zero, rhs);
}

/* ************************************************************************ */

/**
 * @brief Greater operator.
 *
 * @tparam Nominators   A list of nominators.
 * @tparam Denominators A list of denominators.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result value.
 */
template<typename... Nominators, typename... Denominators>
inline constexpr bool operator>(
    Unit<List<Nominators...>, List<Denominators...>> lhs,
    Unit<List<Nominators...>, List<Denominators...>> rhs
) noexcept
{
    return operator<(rhs, lhs);
}

/* ************************************************************************ */

/**
 * @brief Greater operator.
 *
 * @tparam Nominators   A list of nominators.
 * @tparam Denominators A list of denominators.
 *
 * @param lhs Left operand.
 * @param rhs Zero.
 *
 * @return Result value.
 */
template<typename... Nominators, typename... Denominators>
inline constexpr bool operator>(
    Unit<List<Nominators...>, List<Denominators...>> lhs, Zero_t
) noexcept
{
    return operator<(Zero, lhs);
}

/* ************************************************************************ */

/**
 * @brief Greater operator.
 *
 * @tparam Nominators   A list of nominators.
 * @tparam Denominators A list of denominators.
 *
 * @param lhs Zero.
 * @param rhs Right operand.
 *
 * @return Result value.
 */
template<typename... Nominators, typename... Denominators>
inline constexpr bool operator>(
    Zero_t, Unit<List<Nominators...>, List<Denominators...>> rhs
) noexcept
{
    return operator<(rhs, Zero);
}

/* ************************************************************************ */

/**
 * @brief Greater equals operator.
 *
 * @tparam Nominators   A list of nominators.
 * @tparam Denominators A list of denominators.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result value.
 */
template<typename... Nominators, typename... Denominators>
inline constexpr bool operator>=(
    Unit<List<Nominators...>, List<Denominators...>> lhs,
    Unit<List<Nominators...>, List<Denominators...>> rhs
) noexcept
{
    return !operator<(lhs, rhs);
}

/* ************************************************************************ */

/**
 * @brief Greater equals operator.
 *
 * @tparam Nominators   A list of nominators.
 * @tparam Denominators A list of denominators.
 *
 * @param lhs Left operand.
 * @param rhs Zero.
 *
 * @return Result value.
 */
template<typename... Nominators, typename... Denominators>
inline constexpr bool operator>=(
    Unit<List<Nominators...>, List<Denominators...>> lhs, Zero_t
) noexcept
{
    return !operator<(lhs, Zero);
}

/* ************************************************************************ */

/**
 * @brief Greater equals operator.
 *
 * @tparam Nominators   A list of nominators.
 * @tparam Denominators A list of denominators.
 *
 * @param lhs Zero.
 * @param rhs Right operand.
 *
 * @return Result value.
 */
template<typename... Nominators, typename... Denominators>
inline constexpr bool operator>=(
    Zero_t, Unit<List<Nominators...>, List<Denominators...>> rhs
) noexcept
{
    return !operator<(Zero, rhs);
}

/* ************************************************************************ */

/**
 * @brief Addition operator.
 *
 * @tparam Nominators   A list of nominators.
 * @tparam Denominators A list of denominators.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result value.
 */
template<typename... Nominators, typename... Denominators>
inline constexpr
Unit<List<Nominators...>, List<Denominators...>> operator+(
    Unit<List<Nominators...>, List<Denominators...>> lhs,
    Unit<List<Nominators...>, List<Denominators...>> rhs
) noexcept
{
    return lhs += rhs;
}

/* ************************************************************************ */

/**
 * @brief Substraction operator.
 *
 * @tparam Nominators   A list of nominators.
 * @tparam Denominators A list of denominators.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result value.
 */
template<typename... Nominators, typename... Denominators>
inline constexpr
Unit<List<Nominators...>, List<Denominators...>> operator-(
    Unit<List<Nominators...>, List<Denominators...>> lhs,
    Unit<List<Nominators...>, List<Denominators...>> rhs
) noexcept
{
    return lhs -= rhs;
}

/* ************************************************************************ */

/**
 * @brief Multiplication operator.
 *
 * @tparam Nominators   A list of nominators.
 * @tparam Denominators A list of denominators.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result value.
 */
template<typename... Nominators, typename... Denominators>
inline constexpr
Unit<List<Nominators...>, List<Denominators...>> operator*(
    Unit<List<Nominators...>, List<Denominators...>> lhs,
    typename Unit<List<Nominators...>, List<Denominators...>>::value_type rhs) noexcept
{
    return lhs *= rhs;
}

/* ************************************************************************ */

/**
 * @brief Multiplication operator.
 *
 * @tparam Nominators   A list of nominators.
 * @tparam Denominators A list of denominators.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result value.
 */
template<typename... Nominators, typename... Denominators>
inline constexpr
Unit<List<Nominators...>, List<Denominators...>> operator*(
    typename Unit<List<Nominators...>, List<Denominators...>>::value_type lhs,
    Unit<List<Nominators...>, List<Denominators...>> rhs
) noexcept
{
    return rhs *= lhs;
}

/* ************************************************************************ */

/**
 * @brief Multiplication operator.
 *
 * @tparam Nominators1   A list of nominators.
 * @tparam Denominators1 A list of denominators.
 * @tparam Nominators2   A list of nominators.
 * @tparam Denominators2 A list of denominators.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result value.
 */
template<typename... Nominators1, typename... Denominators1, typename... Nominators2, typename... Denominators2>
inline constexpr
typename Reduce<List<Nominators1..., Nominators2...>, List<Denominators1..., Denominators2...>>::type operator*(
    Unit<List<Nominators1...>, List<Denominators1...>> lhs,
    Unit<List<Nominators2...>, List<Denominators2...>> rhs
) noexcept
{
    return typename Reduce<List<Nominators1..., Nominators2...>, List<Denominators1..., Denominators2...>>::type{
        lhs.value() * rhs.value()
    };
}

/* ************************************************************************ */

/**
 * @brief Dividing operator.
 *
 * @tparam Nominators   A list of nominators.
 * @tparam Denominators A list of denominators.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result value.
 */
template<typename... Nominators, typename... Denominators>
inline constexpr
Unit<List<Nominators...>, List<Denominators...>> operator/(
    Unit<List<Nominators...>, List<Denominators...>> lhs,
    typename Unit<List<Nominators...>, List<Denominators...>>::value_type rhs
) noexcept
{
    return lhs /= rhs;
}

/* ************************************************************************ */

/**
 * @brief Dividing operator.
 *
 * @tparam Nominators   A list of nominators.
 * @tparam Denominators A list of denominators.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result value.
 */
template<typename... Nominators, typename... Denominators>
inline constexpr
Unit<List<Denominators...>, List<Nominators...>> operator/(
    typename Unit<List<Nominators...>, List<Denominators...>>::value_type lhs,
    Unit<List<Nominators...>, List<Denominators...>> rhs
) noexcept
{
    return Unit<List<Denominators...>, List<Nominators...>>(lhs / rhs.value());
}

/* ************************************************************************ */

/**
 * @brief Dividing operator.
 *
 * @tparam Nominators1   A list of nominators.
 * @tparam Denominators1 A list of denominators.
 * @tparam Nominators2   A list of nominators.
 * @tparam Denominators2 A list of denominators.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result value.
 */
template<typename... Nominators1, typename... Denominators1, typename... Nominators2, typename... Denominators2>
inline constexpr
typename Reduce<List<Nominators1..., Denominators2...>, List<Denominators1..., Nominators2...>>::type operator/(
    Unit<List<Nominators1...>, List<Denominators1...>> lhs,
    Unit<List<Nominators2...>, List<Denominators2...>> rhs
) noexcept
{
    return typename Reduce<List<Nominators1..., Denominators2...>, List<Denominators1..., Nominators2...>>::type{
        lhs.value() / rhs.value()
    };
}

/* ************************************************************************ */

/**
 * @brief Remove type for Sqrt.
 */
template<typename Types>
struct SqrtRemove;

/* ************************************************************************ */

/**
 * @brief Remove type for Sqrt.
 */
template<>
struct SqrtRemove<List<>>
{
    using type = List<>;
};

/* ************************************************************************ */

/**
 * @brief Remove type for Sqrt.
 */
template<typename Type, typename... Types>
struct SqrtRemove<List<Type, Types...>>
{
    using Rem = Remove<Type, List<Types...>>;
    static_assert(Rem::value, "List is not sqrt");

    using Inner = SqrtRemove<typename Rem::type>;

    using type = typename Concat<List<Type>, typename Inner::type>::type;
};

/* ************************************************************************ */

/**
 * @brief Square root type.
 *
 * @tparam Nom
 * @tparam Denom
 */
template<typename Nom, typename Denom>
struct Sqrt
{
    using Nominators = typename SqrtRemove<Nom>::type;
    using Denominators = typename SqrtRemove<Denom>::type;

    using type = Unit<Nominators, Denominators>;
};

/* ************************************************************************ */

template<typename... Nominators, typename... Denominators>
typename Sqrt<List<Nominators...>, List<Denominators...>>::type
sqrt(const Unit<List<Nominators...>, List<Denominators...>>& value) noexcept
{
    return typename Sqrt<List<Nominators...>, List<Denominators...>>::type(
        std::sqrt(value.value())
    );
}

/* ************************************************************************ */

/**
 * @brief Division type - used for change nominator per denominator.
 *
 * @tparam Nom
 * @tparam Denom
 */
template<typename Nom, typename Denom>
struct Divide
{
    using type = decltype(Nom{} / Denom{});
};

/* ************************************************************************ */

/**
 * @brief Calculate inverse type (1 / type).
 *
 * @tparam T Unit type.
 */
template<typename Type>
struct Inverse;

/* ************************************************************************ */

/**
 * @brief Calculate inverse type (1 / type).
 *
 * @tparam Nominators
 * @tparam Denominators
 */
template<typename... Nominators, typename... Denominators>
struct Inverse<Unit<List<Nominators...>, List<Denominators...>>>
{
    using type = Unit<List<Denominators...>, List<Nominators...>>;
};

/* ************************************************************************ */

#if 0
extern template class Unit<List<BaseLength>, List<>>;
extern template class Unit<List<BaseMass>, List<>>;
extern template class Unit<List<BaseTime>, List<>>;
extern template class Unit<List<BaseLength, BaseLength>, List<>>;
extern template class Unit<List<BaseLength, BaseLength, BaseLength>, List<>>;
extern template class Unit<List<BaseLength>, List<BaseTime>>;
extern template class Unit<List<BaseLength>, List<BaseTime, BaseTime>>;
extern template class Unit<List<BaseLength, BaseMass>, List<BaseTime, BaseTime>>;
extern template class Unit<List<BaseMass>, List<BaseLength, BaseLength, BaseLength>>;
extern template class Unit<List<BaseMass>, List<BaseLength, BaseTime>>;
extern template class Unit<List<BaseLength, BaseLength>, List<BaseTime>>;
#endif

/* ************************************************************************ */

/**
 * @brief Calculate prefix coefficient exponent.
 *
 * @param c     SI prefix character.
 * @param count Number of first SI base unit after prefix.
 *
 * @return Coefficient exponent.
 */
int calcPrefixExponent(char c, unsigned int count = 1);

/* ************************************************************************ */

/**
 * @brief Calculate prefix coefficient exponent.
 *
 * @param symbol     SI unit symbol (or list of symbols).
 * @param typeSymbol Required symbol (or list of symbols).
 * @param count      Number of first SI base unit after prefix.
 *
 * @return Coefficient exponent.
 */
int calcPrefixExponent(const String& symbol, StringView typeSymbol, unsigned int count = 1);

/* ************************************************************************ */

/**
 * @brief Calculate absolute value.
 *
 * @param unit
 */
template<typename... Nominators, typename... Denominators>
inline constexpr
Unit<List<Nominators...>, List<Denominators...>>
abs(const Unit<List<Nominators...>, List<Denominators...>>& unit)
{
    return Unit<List<Nominators...>, List<Denominators...>>(
        unit.value() > Value(0)
        ? unit.value()
        : -unit.value()
    );
}

/* ************************************************************************ */

}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

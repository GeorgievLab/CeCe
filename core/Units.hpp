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
#include <cmath>
#include <limits>
#include <type_traits>

// Simulator
#include "core/compatibility.hpp"

/* ************************************************************************ */

#ifndef _MSC_VER
inline namespace core {
#endif

/* ************************************************************************ */

namespace units {

/* ************************************************************************ */

/**
 * @brief Basic value.
 */
using Value = float;

/* ************************************************************************ */

/**
 * @brief Base SI units.
 */
struct BaseLength { static CONSTEXPR_CONST int value = 0; };
struct BaseTime   { static CONSTEXPR_CONST int value = 1; };
struct BaseMass   { static CONSTEXPR_CONST int value = 2; };

/* ************************************************************************ */

static CONSTEXPR_CONST Value LENGTH_COEFFICIENT = 1e6f;
static CONSTEXPR_CONST Value TIME_COEFFICIENT = 1e0f;
static CONSTEXPR_CONST Value MASS_COEFFICIENT = 1e9f;

/* ************************************************************************ */

/**
 * @brief Less structure.
 *
 * @tparam T1 First type.
 * @tparam T2 Second type.
 */
template<typename T1, typename T2>
struct Less
{
    static CONSTEXPR_CONST bool value = T1::value < T2::value;
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
    static CONSTEXPR_CONST unsigned int size = sizeof...(Types);
};

/* ************************************************************************ */

/**
 * @brief Concat lists.
 */
template<typename... T>
struct Concat;

/* ************************************************************************ */

/**
 * @brief Concat two lists.
 *
 * @tparam Types1
 * @tparam Types2
 * @tparam Tail
 */
template<typename... Types1, typename... Types2, typename... Tail>
struct Concat<List<Types1...>, List<Types2...>, Tail...>
{
    using type = typename Concat<List<Types1..., Types2...>, Tail...>::type;
};

/* ************************************************************************ */

/**
 * @brief Stop specialization.
 *
 * @tparam Types
 */
template<typename... Types>
struct Concat<List<Types...>>
{
    using type = List<Types...>;
};

/* ************************************************************************ */

/**
 * @brief SI Unit.
 *
 * @tparam Nom   List type.
 * @tparam Denom List type.
 */
template<typename Nom, typename Denom>
struct Unit
{

// Public Types
public:

    /// Value type.
    using value_type = Value;

    /// List type.
    using nominator = Nom;

    /// List type.
    using denominator = Denom;



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
    explicit CONSTEXPR Unit(value_type value) NOEXCEPT
        : m_value(value)
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
    explicit operator bool() const NOEXCEPT
    {
        return m_value != 0;
    }


    /**
     * @brief Cast to value type.
     *
     * @return
     */
    explicit operator Value() const NOEXCEPT
    {
        return m_value;
    }


    /**
     * @brief Unary plus operator.
     *
     * @return New value.
     */
    Unit operator+() const NOEXCEPT
    {
        return Unit(m_value);
    }


    /**
     * @brief Unary minus operator.
     *
     * @return New value.
     */
    Unit operator-() const NOEXCEPT
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
    Unit& operator+=(Unit rhs) NOEXCEPT
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
    Unit& operator-=(Unit rhs) NOEXCEPT
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
    Unit& operator*=(value_type rhs) NOEXCEPT
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
    Unit& operator/=(value_type rhs) NOEXCEPT
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
    CONSTEXPR value_type value() const NOEXCEPT
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
 * @brief Remove type element from list.
 *
 * @tparam T    Type to remove.
 * @tparam List List of types.
 */
template<typename T, typename List>
struct Remove;

/* ************************************************************************ */

/**
 * @brief Remove type element from list.
 *
 * @tparam T Type to remove.
 */
template<typename T>
struct Remove<T, List<>>
{
    // Not found
    static CONSTEXPR_CONST bool value = false;

    // Empty list.
    using type = List<>;
};

/* ************************************************************************ */

/**
 * @brief Remove type element from list.
 *
 * @tparam T     Type to remove.
 * @tparam Type  First type.
 * @tparam Types List of types.
 */
template<typename T, typename Type, typename... Types>
struct Remove<T, List<Type, Types...>>
{
    /// If types match.
    static CONSTEXPR_CONST bool match = std::is_same<T, Type>::value;

    // Inner remove
    using remove_inner = Remove<T, List<Types...>>;

    // If type is found.
    static CONSTEXPR_CONST bool value = match || remove_inner::value;

    /// List type without first T
    using type = typename std::conditional<match,
        List<Types...>,
        typename Concat<List<Type>, typename remove_inner::type>::type
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
inline CONSTEXPR bool operator==(
    Unit<List<Nominators...>, List<Denominators...>> lhs,
    Unit<List<Nominators...>, List<Denominators...>> rhs
) NOEXCEPT
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
inline CONSTEXPR bool operator!=(
    Unit<List<Nominators...>, List<Denominators...>> lhs,
    Unit<List<Nominators...>, List<Denominators...>> rhs
) NOEXCEPT
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
inline CONSTEXPR bool operator<(
    Unit<List<Nominators...>, List<Denominators...>> lhs,
    Unit<List<Nominators...>, List<Denominators...>> rhs
) NOEXCEPT
{
    return lhs.value() < rhs.value();
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
inline CONSTEXPR bool operator<=(
    Unit<List<Nominators...>, List<Denominators...>> lhs,
    Unit<List<Nominators...>, List<Denominators...>> rhs
) NOEXCEPT
{
    return !operator>(lhs, rhs);
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
inline CONSTEXPR bool operator>(
    Unit<List<Nominators...>, List<Denominators...>> lhs,
    Unit<List<Nominators...>, List<Denominators...>> rhs
) NOEXCEPT
{
    return operator<(rhs, lhs);
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
inline CONSTEXPR bool operator>=(
    Unit<List<Nominators...>, List<Denominators...>> lhs,
    Unit<List<Nominators...>, List<Denominators...>> rhs
) NOEXCEPT
{
    return !operator<(lhs, rhs);
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
inline CONSTEXPR
Unit<List<Nominators...>, List<Denominators...>> operator+(
    Unit<List<Nominators...>, List<Denominators...>> lhs,
    Unit<List<Nominators...>, List<Denominators...>> rhs
) NOEXCEPT
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
inline CONSTEXPR
Unit<List<Nominators...>, List<Denominators...>> operator-(
    Unit<List<Nominators...>, List<Denominators...>> lhs,
    Unit<List<Nominators...>, List<Denominators...>> rhs
) NOEXCEPT
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
inline CONSTEXPR
Unit<List<Nominators...>, List<Denominators...>> operator*(
    Unit<List<Nominators...>, List<Denominators...>> lhs,
    typename Unit<List<Nominators...>, List<Denominators...>>::value_type rhs) NOEXCEPT
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
inline CONSTEXPR
Unit<List<Nominators...>, List<Denominators...>> operator*(
    typename Unit<List<Nominators...>, List<Denominators...>>::value_type lhs,
    Unit<List<Nominators...>, List<Denominators...>> rhs
) NOEXCEPT
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
inline CONSTEXPR
typename Reduce<List<Nominators1..., Nominators2...>, List<Denominators1..., Denominators2...>>::type operator*(
    Unit<List<Nominators1...>, List<Denominators1...>> lhs,
    Unit<List<Nominators2...>, List<Denominators2...>> rhs
) NOEXCEPT
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
inline CONSTEXPR
Unit<List<Nominators...>, List<Denominators...>> operator/(
    Unit<List<Nominators...>, List<Denominators...>> lhs,
    typename Unit<List<Nominators...>, List<Denominators...>>::value_type rhs
) NOEXCEPT
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
inline CONSTEXPR
Unit<List<Nominators...>, List<Denominators...>> operator/(
    typename Unit<List<Nominators...>, List<Denominators...>>::value_type lhs,
    Unit<List<Nominators...>, List<Denominators...>> rhs
) NOEXCEPT
{
    return rhs /= lhs;
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
inline CONSTEXPR
typename Reduce<List<Nominators1..., Denominators2...>, List<Denominators1..., Nominators2...>>::type operator/(
    Unit<List<Nominators1...>, List<Denominators1...>> lhs,
    Unit<List<Nominators2...>, List<Denominators2...>> rhs
) NOEXCEPT
{
    return typename Reduce<List<Nominators1..., Denominators2...>, List<Denominators1..., Nominators2...>>::type{
        lhs.value() / rhs.value()
    };
}

/* ************************************************************************ */

/**
 * @brief Class for representing distance (meters).
 */
using Length = Unit<List<BaseLength>, List<>>;

/* ************************************************************************ */

/**
 * @brief Class for representing mass (kilograms).
 */
using Mass = Unit<List<BaseMass>, List<>>;

/* ************************************************************************ */

/**
 * @brief Class for representing time (seconds).
 */
using Time = Unit<List<BaseTime>, List<>>;
using Duration = Time;

/* ************************************************************************ */

/**
 * @brief Class for representing area.
 */
using Area = Unit<List<BaseLength, BaseLength>, List<>>;

/* ************************************************************************ */

/**
 * @brief Class for representing volume.
 */
using Volume = Unit<List<BaseLength, BaseLength, BaseLength>, List<>>;

/* ************************************************************************ */

/**
 * @brief Class for representing velocity (micrometers per second).
 */
using Velocity = Unit<List<BaseLength>, List<BaseTime>>;

/* ************************************************************************ */

/**
 * @brief Class for representing acceleration (micrometers per second^2).
 */
using Acceleration = Unit<List<BaseLength>, List<BaseTime, BaseTime>>;

/* ************************************************************************ */

/**
 * @brief Class for representing force (Newton).
 */
using Force = Unit<List<BaseLength, BaseMass>, List<BaseTime, BaseTime>>;

/* ************************************************************************ */

/**
 * @brief Class for representing density.
 */
using Density = Unit<List<BaseMass>, List<BaseLength, BaseLength, BaseLength>>;

/* ************************************************************************ */

/**
 * @brief Class for representing viscosity.
 */
using Viscosity = Unit<List<BaseMass>, List<BaseLength, BaseTime>>;

/* ************************************************************************ */

/**
 * @brief Class for representing volume.
 */
using Angle = Value;

/* ************************************************************************ */

/**
 * @brief Class for representing probability.
 */
using Probability = Value;

/* ************************************************************************ */

/**
 * @brief Convert degrees to radians.
 *
 * @param value
 *
 * @return
 */
inline CONSTEXPR Value deg2rad(Value value) NOEXCEPT
{
    return value * 0.01745329252f;
}

/* ************************************************************************ */

/**
 * @brief Convert radians to degrees.
 *
 * @param value
 *
 * @return
 */
inline CONSTEXPR Value rad2deg(Value value) NOEXCEPT
{
    return value * 57.2957795f;
}

/* ************************************************************************ */

/**
 * @brief Meter value.
 *
 * @param value Value.
 *
 * @return Length value.
 */
inline CONSTEXPR Length m(Value value) NOEXCEPT
{
    // 1m = 1'000mm
    return Length(LENGTH_COEFFICIENT * value);
}

/* ************************************************************************ */

/**
 * @brief Millimeter value.
 *
 * @param value Value.
 *
 * @return Length value.
 */
inline CONSTEXPR Length mm(Value value) NOEXCEPT
{
    // 1mm = 1/1'000m
    return m(value * 1e-3);
}

/* ************************************************************************ */

/**
 * @brief Micrometer value.
 *
 * @param value Value.
 *
 * @return Length value.
 */
inline CONSTEXPR Length um(Value value) NOEXCEPT
{
    // 1um = 1/1'000'000m
    return m(value * 1e-6);
}

/* ************************************************************************ */

/**
 * @brief Kilograms value.
 *
 * @param value Value.
 *
 * @return Mass value.
 */
inline CONSTEXPR Mass kg(Value value) NOEXCEPT
{
    return Mass(MASS_COEFFICIENT * value);
}

/* ************************************************************************ */

/**
 * @brief Grams value.
 *
 * @param value Value.
 *
 * @return Mass value.
 */
inline CONSTEXPR Mass g(Value value) NOEXCEPT
{
    // 1g = 1/1'000 kg
    return kg(value * 1e-3);
}

/* ************************************************************************ */

/**
 * @brief Milligrams value.
 *
 * @param value Value.
 *
 * @return Mass value.
 */
inline CONSTEXPR Mass mg(Value value) NOEXCEPT
{
    // 1mg = 1/1'000 g
    return g(value * 1e-3);
}

/* ************************************************************************ */

/**
 * @brief Micrograms value.
 *
 * @param value Value.
 *
 * @return Mass value.
 */
inline CONSTEXPR Mass ug(Value value) NOEXCEPT
{
    // 1ug = 1/1'000 mg
    return mg(value * 1e-3);
}

/* ************************************************************************ */

/**
 * @brief Nanograms value.
 *
 * @param value Value.
 *
 * @return Mass value.
 */
inline CONSTEXPR Mass ng(Value value) NOEXCEPT
{
    // 1ng = 1/1'000 ug
    return ug(value * 1e-3);
}

/* ************************************************************************ */

/**
 * @brief Micrograms value.
 *
 * @param value Value.
 *
 * @return Mass value.
 */
inline CONSTEXPR Mass pg(Value value) NOEXCEPT
{
    // 1pg = 1/1'000 ng
    return ng(value * 1e-3);
}

/* ************************************************************************ */

/**
 * @brief Seconds value.
 *
 * @param value Value.
 *
 * @return Time value.
 */
inline CONSTEXPR Time s(Value value) NOEXCEPT
{
    // 1s
    return Time(TIME_COEFFICIENT * value);
}

/* ************************************************************************ */

/**
 * @brief Milliseconds value.
 *
 * @param value Value.
 *
 * @return Time value.
 */
inline CONSTEXPR Time ms(Value value) NOEXCEPT
{
    // 1s = 1'000ms
    return s(value / 1000.f);
}

/* ************************************************************************ */

/**
 * @brief Microseconds value.
 *
 * @param value Value.
 *
 * @return Time value.
 */
inline CONSTEXPR Time us(Value value) NOEXCEPT
{
    // 1ms = 1'000us
    return ms(value / 1000.f);
}

/* ************************************************************************ */

/**
 * @brief Meters^2 value.
 *
 * @param value Value.
 *
 * @return Area value.
 */
inline CONSTEXPR Area m2(Value value) NOEXCEPT
{
    return m(value) * m(1);
}

/* ************************************************************************ */

/**
 * @brief Millimeters^2 value.
 *
 * @param value Value.
 *
 * @return Area value.
 */
inline CONSTEXPR Area mm2(Value value) NOEXCEPT
{
    return mm(value) * mm(1);
}

/* ************************************************************************ */

/**
 * @brief Micrometers^2 value.
 *
 * @param value Value.
 *
 * @return Area value.
 */
inline CONSTEXPR Area um2(Value value) NOEXCEPT
{
    return um(value) * um(1);
}

/* ************************************************************************ */

/**
 * @brief Meters^3 value.
 *
 * @param value Value.
 *
 * @return Volume value.
 */
inline CONSTEXPR Volume m3(Value value) NOEXCEPT
{
    return m2(value) * m(1);
}

/* ************************************************************************ */

/**
 * @brief Millimeters^3 value.
 *
 * @param value Value.
 *
 * @return Volume value.
 */
inline CONSTEXPR Volume mm3(Value value) NOEXCEPT
{
    return mm2(value) * mm(1);
}

/* ************************************************************************ */

/**
 * @brief Micrometers^3 value.
 *
 * @param value Value.
 *
 * @return Volume value.
 */
inline CONSTEXPR Volume um3(Value value) NOEXCEPT
{
    return um2(value) * um(1);
}

/* ************************************************************************ */

/**
 * @brief Meters per second value.
 *
 * @param value Value.
 *
 * @return Velocity value.
 */
inline CONSTEXPR Velocity m_s(Value value) NOEXCEPT
{
    return m(value) / s(1);
}

/* ************************************************************************ */

/**
 * @brief Millimeters per second value.
 *
 * @param value Value.
 *
 * @return Velocity value.
 */
inline CONSTEXPR Velocity mm_s(Value value) NOEXCEPT
{
    return mm(value) / s(1);
}

/* ************************************************************************ */

/**
 * @brief Micrometers per second value.
 *
 * @param value Value.
 *
 * @return Velocity value.
 */
inline CONSTEXPR Velocity um_s(Value value) NOEXCEPT
{
    return um(value) / s(1);
}

/* ************************************************************************ */

/**
 * @brief Meters per second^2 value.
 *
 * @param value Value.
 *
 * @return Acceleration value.
 */
inline CONSTEXPR Acceleration m_s2(Value value) NOEXCEPT
{
    return m_s(value) / s(1);
}

/* ************************************************************************ */

/**
 * @brief Millimeters per second^2 value.
 *
 * @param value Value.
 *
 * @return Acceleration value.
 */
inline CONSTEXPR Acceleration mm_s2(Value value) NOEXCEPT
{
    return mm_s(value) / s(1);
}

/* ************************************************************************ */

/**
 * @brief Micrometers per second^2 value.
 *
 * @param value Value.
 *
 * @return Acceleration value.
 */
inline CONSTEXPR Acceleration um_s2(Value value) NOEXCEPT
{
    return um_s(value) / s(1);
}

/* ************************************************************************ */

/**
 * @brief Kilogram meter per second^2 value.
 *
 * @param value Value.
 *
 * @return Force value.
 */
inline CONSTEXPR Force kgm_s2(Value value) NOEXCEPT
{
    return kg(value) * m_s2(1);
}

/* ************************************************************************ */

/**
 * @brief Gram meter per second^2 value.
 *
 * @param value Value.
 *
 * @return Force value.
 */
inline CONSTEXPR Force gm_s2(Value value) NOEXCEPT
{
    return g(value) * m_s2(1);
}

/* ************************************************************************ */

/**
 * @brief Milligram meter per second^2 value.
 *
 * @param value Value.
 *
 * @return Force value.
 */
inline CONSTEXPR Force mgm_s2(Value value) NOEXCEPT
{
    return kg(value) * m_s2(1);
}

/* ************************************************************************ */

/**
 * @brief Newton value.
 *
 * @param value Value.
 *
 * @return Force value.
 */
inline CONSTEXPR Force N(Value value) NOEXCEPT
{
    return kgm_s2(value);
}

/* ************************************************************************ */

/**
 * @brief Millinewton value.
 *
 * @param value Value.
 *
 * @return Force value.
 */
inline CONSTEXPR Force mN(Value value) NOEXCEPT
{
    return gm_s2(value);
}

/* ************************************************************************ */

/**
 * @brief Micronewton value.
 *
 * @param value Value.
 *
 * @return Force value.
 */
inline CONSTEXPR Force uN(Value value) NOEXCEPT
{
    return mgm_s2(value);
}

/* ************************************************************************ */

/**
 * @brief Kilogram per meter and second value.
 *
 * @param value Value.
 *
 * @return Viscosity value.
 */
inline CONSTEXPR Viscosity kg_m_s(Value value) NOEXCEPT
{
    return kg(value) / (m(1) * s(1));
}

/* ************************************************************************ */

/**
 * @brief Kilogram per meter and second value.
 *
 * @param value Value.
 *
 * @return Viscosity value.
 */
inline CONSTEXPR Viscosity g_m_s(Value value) NOEXCEPT
{
    return g(value) / (m(1) * s(1));
}

/* ************************************************************************ */

/**
 * @brief Newton second per meter square value.
 *
 * @param value Value.
 *
 * @return Viscosity value.
 */
inline CONSTEXPR Viscosity Ns_m2(Value value) NOEXCEPT
{
    return N(value) * s(1) / m2(1);
}

/* ************************************************************************ */

/**
 * @brief Newton second per meter square value.
 *
 * @param value Value.
 *
 * @return Viscosity value.
 */
inline CONSTEXPR Viscosity Pas(Value value) NOEXCEPT
{
    return N(value) * s(1) / m2(1);
}

/* ************************************************************************ */

/**
 * @brief Newton second per meter square value.
 *
 * @param value Value.
 *
 * @return Viscosity value.
 */
inline CONSTEXPR Viscosity mPas(Value value) NOEXCEPT
{
    return mN(value) * s(1) / m2(1);
}

/* ************************************************************************ */

/**
 * @brief Radian constructor.
 *
 * @param value Value in radians.
 *
 * @return Angle.
 */
inline CONSTEXPR Angle rad(Value value) NOEXCEPT
{
    return Angle(value);
}

/* ************************************************************************ */

/**
 * @brief Degree constructor.
 *
 * @param value Value in degrees.
 *
 * @return Angle.
 */
inline CONSTEXPR Angle deg(Value value) NOEXCEPT
{
    return rad(deg2rad(value));
}

/* ************************************************************************ */

/**
 * @brief Probability constructor.
 *
 * @param value Value in precent.
 *
 * @return
 */
inline CONSTEXPR Probability precent(Value value) NOEXCEPT
{
    return value / 100.f;
}

/* ************************************************************************ */

}

/* ************************************************************************ */

#ifndef _MSC_VER
}
#endif

/* ************************************************************************ */

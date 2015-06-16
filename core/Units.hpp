/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* ************************************************************************ */

#pragma once

/* ************************************************************************ */

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
 * @brief Base SI units.
 */
struct BaseLength {};
struct BaseTime {};
struct BaseMass {};

/* ************************************************************************ */

/**
 * @brief Unit nominator.
 *
 * @tparam Types A list of nominator types.
 */
template<typename... Types>
struct Nominator
{

};

/* ************************************************************************ */

/**
 * @brief Unit denominator.
 *
 * @tparam Types A list of denominator types.
 */
template<typename... Types>
struct Denominator
{

};

/* ************************************************************************ */

/**
 * @brief SI Unit.
 *
 * @tparam Nom   Nominator type.
 * @tparam Denom Denominator type.
 */
template<typename Nom, typename Denom>
struct Unit
{

// Public Types
public:

    /// Value type.
    using value_type = float;

    /// Nominator type.
    using nominator = Nom;

    /// Denominator type.
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
    Unit<Nominator<Nominators...>, Denominator<Denominators...>> lhs,
    Unit<Nominator<Nominators...>, Denominator<Denominators...>> rhs
) NOEXCEPT
{
    return lhs.value() == rhs.value();
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
    Unit<Nominator<Nominators...>, Denominator<Denominators...>> lhs,
    Unit<Nominator<Nominators...>, Denominator<Denominators...>> rhs
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
    Unit<Nominator<Nominators...>, Denominator<Denominators...>> lhs,
    Unit<Nominator<Nominators...>, Denominator<Denominators...>> rhs
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
    Unit<Nominator<Nominators...>, Denominator<Denominators...>> lhs,
    Unit<Nominator<Nominators...>, Denominator<Denominators...>> rhs
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
    Unit<Nominator<Nominators...>, Denominator<Denominators...>> lhs,
    Unit<Nominator<Nominators...>, Denominator<Denominators...>> rhs
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
    Unit<Nominator<Nominators...>, Denominator<Denominators...>> lhs,
    Unit<Nominator<Nominators...>, Denominator<Denominators...>> rhs
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
Unit<Nominator<Nominators...>, Denominator<Denominators...>> operator+(
    Unit<Nominator<Nominators...>, Denominator<Denominators...>> lhs,
    Unit<Nominator<Nominators...>, Denominator<Denominators...>> rhs
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
Unit<Nominator<Nominators...>, Denominator<Denominators...>> operator-(
    Unit<Nominator<Nominators...>, Denominator<Denominators...>> lhs,
    Unit<Nominator<Nominators...>, Denominator<Denominators...>> rhs
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
Unit<Nominator<Nominators...>, Denominator<Denominators...>> operator*(
    Unit<Nominator<Nominators...>, Denominator<Denominators...>> lhs,
    typename Unit<Nominator<Nominators...>, Denominator<Denominators...>>::value_type rhs) NOEXCEPT
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
Unit<Nominator<Nominators...>, Denominator<Denominators...>> operator*(
    typename Unit<Nominator<Nominators...>, Denominator<Denominators...>>::value_type lhs,
    Unit<Nominator<Nominators...>, Denominator<Denominators...>> rhs
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
Unit<Nominator<Nominators1..., Nominators2...>, Denominator<Denominators1..., Denominators2...>> operator*(
    Unit<Nominator<Nominators1...>, Denominator<Denominators1...>> lhs,
    Unit<Nominator<Nominators2...>, Denominator<Denominators2...>> rhs
) NOEXCEPT
{
    return Unit<Nominator<Nominators1..., Nominators2...>, Denominator<Denominators1..., Denominators2...>>{
        rhs.value() * lhs.value()
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
Unit<Nominator<Nominators...>, Denominator<Denominators...>> operator/(
    Unit<Nominator<Nominators...>, Denominator<Denominators...>> lhs,
    typename Unit<Nominator<Nominators...>, Denominator<Denominators...>>::value_type rhs
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
Unit<Nominator<Nominators...>, Denominator<Denominators...>> operator/(
    typename Unit<Nominator<Nominators...>, Denominator<Denominators...>>::value_type lhs,
    Unit<Nominator<Nominators...>, Denominator<Denominators...>> rhs
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
Unit<Nominator<Nominators1..., Denominators2...>, Denominator<Denominators1..., Nominators2...>> operator/(
    Unit<Nominator<Nominators1...>, Denominator<Denominators1...>> lhs,
    Unit<Nominator<Nominators2...>, Denominator<Denominators2...>> rhs
) NOEXCEPT
{
    return Unit<Nominator<Nominators1..., Denominators2...>, Denominator<Denominators1..., Nominators2...>>{
        rhs.value() / lhs.value()
    };
}

/* ************************************************************************ */

/**
 * @brief Class for representing distance (meters).
 */
using Length = Unit<Nominator<BaseLength>, Denominator<>>;

/* ************************************************************************ */

/**
 * @brief Class for representing mass (kilograms).
 */
using Mass = Unit<Nominator<BaseMass>, Denominator<>>;

/* ************************************************************************ */

/**
 * @brief Class for representing time (seconds).
 */
using Time = Unit<Nominator<BaseTime>, Denominator<>>;
using Duration = Time;

/* ************************************************************************ */

/**
 * @brief Class for representing area.
 */
using Area = Unit<Nominator<BaseLength, BaseLength>, Denominator<>>;

/* ************************************************************************ */

/**
 * @brief Class for representing volume.
 */
using Volume = Unit<Nominator<BaseLength, BaseLength, BaseLength>, Denominator<>>;

/* ************************************************************************ */

/**
 * @brief Class for representing velocity (micrometers per second).
 */
using Velocity = Unit<Nominator<BaseLength>, Denominator<BaseTime>>;

/* ************************************************************************ */

/**
 * @brief Class for representing acceleration (micrometers per second^2).
 */
using Acceleration = Unit<Nominator<BaseLength>, Denominator<BaseTime, BaseTime>>;

/* ************************************************************************ */

/**
 * @brief Class for representing force (Newton).
 */
using Force = Unit<Nominator<BaseMass, BaseLength>, Denominator<BaseTime, BaseTime>>;

/* ************************************************************************ */

/**
 * @brief Class for representing density.
 */
using Density = Unit<Nominator<BaseMass>, Denominator<BaseLength, BaseLength, BaseLength>>;

/* ************************************************************************ */

/**
 * @brief Class for representing volume.
 */
using Angle = float;

/* ************************************************************************ */

/**
 * @brief Class for representing probability.
 */
using Probability = float;

/* ************************************************************************ */

/**
 * @brief Convert degrees to radians.
 *
 * @param value
 *
 * @return
 */
inline CONSTEXPR float deg2rad(float value) NOEXCEPT
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
inline CONSTEXPR float rad2deg(float value) NOEXCEPT
{
    return value * 57.2957795f;
}

/* ************************************************************************ */

/**
 * @brief Meter value.
 *
 * @param value
 */
inline CONSTEXPR Length m(float value) NOEXCEPT
{
    // 1m = 1'000'000um
    return Length(1000.f * 1000.f * value);
}

/* ************************************************************************ */

/**
 * @brief Millimeter value.
 *
 * @param value
 */
inline CONSTEXPR Length mm(float value) NOEXCEPT
{
    // 1mm = 1/1'000m
    return m(value / 1000.f);
}

/* ************************************************************************ */

/**
 * @brief Micrometer value.
 *
 * @param value
 */
inline CONSTEXPR Length um(float value) NOEXCEPT
{
    // 1um = 1/1'000'000m
    return m(value / (1000.f * 1000.f));
}

/* ************************************************************************ */

/**
 * @brief Kilograms value.
 *
 * @param value
 */
inline CONSTEXPR Mass kg(float value) NOEXCEPT
{
    // 1mg
    return Mass(value / (1000.f * 1000.f));
}

/* ************************************************************************ */

/**
 * @brief Grams value.
 *
 * @param value
 */
inline CONSTEXPR Mass g(float value) NOEXCEPT
{
    // 1kg = 1'000g
    return kg(value * 1000.f);
}

/* ************************************************************************ */

/**
 * @brief Milligrams value.
 *
 * @param value
 */
inline CONSTEXPR Mass mg(float value) NOEXCEPT
{
    // 1g = 1'000mg
    return g(value * 1000.f);
}

/* ************************************************************************ */

/**
 * @brief Micrograms value.
 *
 * @param value
 */
inline CONSTEXPR Mass ug(float value) NOEXCEPT
{
    // 1mg = 1'000ug
    return mg(value * 1000.f);
}

/* ************************************************************************ */

/**
 * @brief Seconds value.
 *
 * @param value
 */
inline CONSTEXPR Duration s(float value) NOEXCEPT
{
    // 1s
    return Duration(value);
}

/* ************************************************************************ */

/**
 * @brief Milliseconds value.
 *
 * @param value
 */
inline CONSTEXPR Duration ms(float value) NOEXCEPT
{
    // 1s = 1'000ms
    return s(value / 1000.f);
}

/* ************************************************************************ */

/**
 * @brief Microseconds value.
 *
 * @param value
 */
inline CONSTEXPR Duration us(float value) NOEXCEPT
{
    // 1ms = 1'000us
    return ms(value / 1000.f);
}

/* ************************************************************************ */

/**
 * @brief Meters^2 value.
 *
 * @param value
 */
inline CONSTEXPR Area m2(float value) NOEXCEPT
{
    return m(value) * m(1);
}

/* ************************************************************************ */

/**
 * @brief Millimeters^2 value.
 *
 * @param value
 */
inline CONSTEXPR Area mm2(float value) NOEXCEPT
{
    return mm(value) * mm(1);
}

/* ************************************************************************ */

/**
 * @brief Micrometers^2 value.
 *
 * @param value
 */
inline CONSTEXPR Area um2(float value) NOEXCEPT
{
    return um(value) * um(1);
}

/* ************************************************************************ */

/**
 * @brief Meters^3 value.
 *
 * @param value
 */
inline CONSTEXPR Volume m3(float value) NOEXCEPT
{
    return m2(value) * m(1);
}

/* ************************************************************************ */

/**
 * @brief Millimeters^3 value.
 *
 * @param value
 */
inline CONSTEXPR Volume mm3(float value) NOEXCEPT
{
    return mm2(value) * mm(1);
}

/* ************************************************************************ */

/**
 * @brief Micrometers^3 value.
 *
 * @param value
 */
inline CONSTEXPR Volume um3(float value) NOEXCEPT
{
    return um2(value) * um(1);
}

/* ************************************************************************ */

/**
 * @brief Meters per second value.
 *
 * @param value
 */
inline CONSTEXPR Velocity m_s(float value) NOEXCEPT
{
    return m(value) / s(1);
}

/* ************************************************************************ */

/**
 * @brief Millimeters per second value.
 *
 * @param value
 */
inline CONSTEXPR Velocity mm_s(float value) NOEXCEPT
{
    return mm(value) / s(1);
}

/* ************************************************************************ */

/**
 * @brief Micrometers per second value.
 *
 * @param value
 */
inline CONSTEXPR Velocity um_s(float value) NOEXCEPT
{
    return um(value) / s(1);
}

/* ************************************************************************ */

/**
 * @brief Meters per second^2 value.
 *
 * @param value
 */
inline CONSTEXPR Acceleration m_s2(float value) NOEXCEPT
{
    return m_s(value) / s(1);
}

/* ************************************************************************ */

/**
 * @brief Millimeters per second^2 value.
 *
 * @param value
 */
inline CONSTEXPR Acceleration mm_s2(float value) NOEXCEPT
{
    return mm_s(value) / s(1);
}

/* ************************************************************************ */

/**
 * @brief Micrometers per second^2 value.
 *
 * @param value
 */
inline CONSTEXPR Acceleration um_s2(float value) NOEXCEPT
{
    return um_s(value) / s(1);
}

/* ************************************************************************ */

/**
 * @brief Kilogram meter per second^2 value.
 *
 * @param value
 */
inline CONSTEXPR Force kgm_s2(float value) NOEXCEPT
{
    return kg(value) * m_s2(1);
}

/* ************************************************************************ */

/**
 * @brief Gram meter per second^2 value.
 *
 * @param value
 */
inline CONSTEXPR Force gm_s2(float value) NOEXCEPT
{
    return g(value) * m_s2(1);
}

/* ************************************************************************ */

/**
 * @brief Milligram meter per second^2 value.
 *
 * @param value
 */
inline CONSTEXPR Force mgm_s2(float value) NOEXCEPT
{
    return kg(value) * m_s2(1);
}

/* ************************************************************************ */

/**
 * @brief Newton value.
 *
 * @param value
 */
inline CONSTEXPR Force N(float value) NOEXCEPT
{
    return kgm_s2(value);
}

/* ************************************************************************ */

/**
 * @brief Millinewton value.
 *
 * @param value
 */
inline CONSTEXPR Force mN(float value) NOEXCEPT
{
    return gm_s2(value);
}

/* ************************************************************************ */

/**
 * @brief Micronewton value.
 *
 * @param value
 */
inline CONSTEXPR Force uN(float value) NOEXCEPT
{
    return mgm_s2(value);
}

/* ************************************************************************ */

/**
 * @brief Radian constructor.
 *
 * @param value Value in radians.
 *
 * @return Angle.
 */
inline CONSTEXPR Angle rad(float value) NOEXCEPT
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
inline CONSTEXPR Angle deg(float value) NOEXCEPT
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
inline CONSTEXPR Probability precent(float value) NOEXCEPT
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

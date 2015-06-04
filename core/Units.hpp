
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
 * @brief Class for representing distance (micrometers).
 */
using Length = float;

/* ************************************************************************ */

/**
 * @brief Class for representing velocity (micrometers per second).
 */
using Velocity = float;

/* ************************************************************************ */

/**
 * @brief Class for representing acceleration (micrometers per second^2).
 */
using Acceleration = float;

/* ************************************************************************ */

/**
 * @brief Class for representing force (Newton).
 */
using Force = float;

/* ************************************************************************ */

/**
 * @brief Class for representing volume.
 */
using Volume = float;

/* ************************************************************************ */

/**
 * @brief Class for representing density.
 */
using Density = float;

/* ************************************************************************ */

/**
 * @brief Class for representing volume.
 */
using Angle = float;

/* ************************************************************************ */

/**
 * @brief Class for representing duration.
 */
using Duration = float;

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
 * @brief Micrometer value.
 *
 * @param value
 */
inline CONSTEXPR Length um(float value) NOEXCEPT
{
    return Length(value);
}

/* ************************************************************************ */

/**
 * @brief Millimeter value.
 *
 * @param value
 */
inline CONSTEXPR Length mm(float value) NOEXCEPT
{
    return Length(1000 * value);
}

/* ************************************************************************ */

/**
 * @brief Centimeter value.
 *
 * @param value
 */
inline CONSTEXPR Length cm(float value) NOEXCEPT
{
    return Length(10 * 1000 * value);
}

/* ************************************************************************ */

/**
 * @brief Meter value.
 *
 * @param value
 */
inline CONSTEXPR Length m(float value) NOEXCEPT
{
    return Length(1000 * 1000 * value);
}

/* ************************************************************************ */

/**
 * @brief Micrometer^3 value.
 *
 * @param value
 */
inline CONSTEXPR Volume um3(float value) NOEXCEPT
{
    return Volume(value);
}

/* ************************************************************************ */

/**
 * @brief Radian constructor.
 */
inline CONSTEXPR Angle rad(float value)
{
    return Angle(value);
}

/* ************************************************************************ */

/**
 * @brief Degree constructor.
 */
inline CONSTEXPR Angle deg(float value)
{
    return Angle(deg2rad(value));
}

/* ************************************************************************ */

/**
 * @brief Seconds value.
 *
 * @param value
 */
inline CONSTEXPR Duration s(float value) NOEXCEPT
{
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
    return Duration(value / 1000.f);
}

/* ************************************************************************ */

/**
 * @brief Microseconds value.
 *
 * @param value
 */
inline CONSTEXPR Duration us(float value) NOEXCEPT
{
    return Duration(value / 1000000.f);
}

/* ************************************************************************ */

#ifndef _MSC_VER
/**
 * @brief Micrometers literal.
 */
inline Length operator"" _um(long double value)
{
    return um(value);
}

/* ************************************************************************ */

/**
 * @brief Micrometers literal.
 */
inline Length operator"" _um(unsigned long long int value)
{
    return um(value);
}

/* ************************************************************************ */

/**
 * @brief MilliMeters literal.
 */
inline Length operator"" _mm(unsigned long long int value)
{
    return mm(value);
}

/* ************************************************************************ */

/**
 * @brief CentiMeters literal.
 */
inline Length operator"" _cm(unsigned long long int value)
{
    return cm(value);
}

/* ************************************************************************ */

/**
 * @brief Meters literal.
 */
inline Length operator"" _m(unsigned long long int value)
{
    return m(value);
}

/* ************************************************************************ */

/**
 * @brief Micrometers^3 literal.
 */
inline Volume operator"" _um3(long double value)
{
    return um3(value);
}

/* ************************************************************************ */

/**
 * @brief Micrometers^3 literal.
 */
inline Volume operator"" _um3(unsigned long long int value)
{
    return um3(value);
}

/* ************************************************************************ */

/**
 * @brief Radian literal.
 */
inline Angle operator"" _rad(long double value)
{
    return rad(value);
}

/* ************************************************************************ */

/**
 * @brief Radian literal.
 */
inline Angle operator"" _rad(unsigned long long int value)
{
    return rad(value);
}

/* ************************************************************************ */

/**
 * @brief Radian literal.
 */
inline Angle operator"" _deg(long double value)
{
    return deg(value);
}

/* ************************************************************************ */

/**
 * @brief Radian literal.
 */
inline Angle operator"" _deg(unsigned long long int value)
{
    return deg(value);
}

/* ************************************************************************ */

/**
 * @brief Seconds literal.
 */
inline Duration operator"" _s(long double value)
{
    return Duration(value);
}

/* ************************************************************************ */

/**
 * @brief Seconds literal.
 */
inline Duration operator"" _s(unsigned long long int value)
{
    return Duration(value);
}

/* ************************************************************************ */

/**
 * @brief Seconds literal.
 */
inline Duration operator"" _ms(long double value)
{
    return Duration(value * 0.001);
}

/* ************************************************************************ */

/**
 * @brief Seconds literal.
 */
inline Duration operator"" _ms(unsigned long long int value)
{
    return Duration(value * 0.001);
}
#endif

/* ************************************************************************ */

}

/* ************************************************************************ */

#ifndef _MSC_VER
}
#endif

/* ************************************************************************ */

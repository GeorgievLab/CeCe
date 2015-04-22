
#pragma once

/* ************************************************************************ */

inline namespace core {
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
 * @brief Micrometer value.
 *
 * @param value
 */
constexpr Length um(float value) noexcept
{
    return Length(value);
}

/* ************************************************************************ */

/**
 * @brief Millimeter value.
 *
 * @param value
 */
constexpr Length mm(float value) noexcept
{
    return Length(1000 * value);
}

/* ************************************************************************ */

/**
 * @brief Centimeter value.
 *
 * @param value
 */
constexpr Length cm(float value) noexcept
{
    return Length(10 * 1000 * value);
}

/* ************************************************************************ */

/**
 * @brief Meter value.
 *
 * @param value
 */
constexpr Length m(float value) noexcept
{
    return Length(1000 * 1000 * value);
}

/* ************************************************************************ */

/**
 * @brief Micrometer^3 value.
 *
 * @param value
 */
constexpr Volume um3(float value) noexcept
{
    return Volume(value);
}

/* ************************************************************************ */

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
    return Angle(value);
}

/* ************************************************************************ */

/**
 * @brief Radian literal.
 */
inline Angle operator"" _rad(unsigned long long int value)
{
    return Angle(value);
}

/* ************************************************************************ */

/**
 * @brief Radian literal.
 */
inline Angle operator"" _deg(long double value)
{
    return Angle(value * 0.01745329252);
}

/* ************************************************************************ */

/**
 * @brief Radian literal.
 */
inline Angle operator"" _deg(unsigned long long int value)
{
    return Angle(value * 0.01745329252);
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

/* ************************************************************************ */

}
}

/* ************************************************************************ */

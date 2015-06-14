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
enum class SI
{
    Length,
    Time,
    Mass,
    ElectricCurrent,
    ThermodynamicTemperature,
    AmountOfSubstance,
    LuminousIntensity
};

/* ************************************************************************ */

/**
 * @brief Class for storing unit value.
 *
 * @tparam Derived Derived class.
 */
template<typename Derived>
class Value
{

// Public Ctors & Dtors
public:


    /**
     * @brief Default constructor.
     */
    Value() = default;


    /**
     * @brief Constructor.
     *
     * @param value Init value.
     */
    explicit CONSTEXPR Value(float value) NOEXCEPT
        : m_value(value)
    {
        // Nothing to do
    }


// Public Operators
public:


    /**
     * @brief Addition operator.
     *
     * @param rhs
     *
     * @return *this.
     */
    Derived& operator+=(Derived rhs) NOEXCEPT
    {
        m_value += rhs.m_value;
        return *this;
    }


    /**
     * @brief Substract operator.
     *
     * @param rhs
     *
     * @return *this.
     */
    Derived& operator-=(Derived rhs) NOEXCEPT
    {
        m_value -= rhs.m_value;
        return *this;
    }


    /**
     * @brief Multiply operator.
     *
     * @param rhs
     *
     * @return *this.
     */
    Derived& operator*=(float rhs) NOEXCEPT
    {
        m_value *= rhs;
        return *this;
    }


    /**
     * @brief Divide operator.
     *
     * @param rhs
     *
     * @return *this.
     */
    Derived& operator/=(float rhs) NOEXCEPT
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
    CONSTEXPR float value() const NOEXCEPT
    {
        return m_value;
    }


// Private Data Members
private:

    /// Stored value.
    float m_value;

};

/* ************************************************************************ */

/**
 * @brief Base SI unit.
 *
 * @tparam BaseSI
 */
template<SI BaseSI>
class Base : public Value<Base<BaseSI>>
{

// Public Ctors & Dtors
public:


    /**
     * @brief Constructors.
     */
    using Value<Base>::Value;

};

/* ************************************************************************ */

/**
 * @brief Derived SI unit.
 *
 * @tparam T1
 * @tparam T2
 */
template<typename T1, typename T2>
class Derived : public Value<Derived<T1, T2>>
{

// Public Ctors & Dtors
public:


    /**
     * @brief Constructors.
     */
    using Value<Derived>::Value;

};

/* ************************************************************************ */

/**
 * @brief Divided value helper.
 *
 * @tparam T
 */
template<typename T>
class Divide : public Value<Divide<T>>
{

// Public Ctors & Dtors
public:


    /**
     * @brief Constructors.
     */
    using Value<Divide>::Value;

};

/* ************************************************************************ */

/**
 * @brief Addition operator.
 *
 * @tparam BaseSI Basic SI unit type.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result value.
 */
template<SI BaseSI>
inline CONSTEXPR Base<BaseSI> operator+(Base<BaseSI> lhs, Base<BaseSI> rhs) NOEXCEPT
{
    return lhs += rhs;
}

/* ************************************************************************ */

/**
 * @brief Substract operator.
 *
 * @tparam BaseSI Basic SI unit type.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result value.
 */
template<SI BaseSI>
inline CONSTEXPR Base<BaseSI> operator-(Base<BaseSI> lhs, Base<BaseSI> rhs) NOEXCEPT
{
    return lhs -= rhs;
}

/* ************************************************************************ */

/**
 * @brief Multiply operator.
 *
 * @tparam BaseSI Basic SI unit type.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result value.
 */
template<SI BaseSI>
inline CONSTEXPR Base<BaseSI> operator*(Base<BaseSI> lhs, float rhs) NOEXCEPT
{
    return lhs *= rhs;
}

/* ************************************************************************ */

/**
 * @brief Multiply operator.
 *
 * @tparam BaseSI Basic SI unit type.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result value.
 */
template<SI BaseSI>
inline CONSTEXPR Base<BaseSI> operator*(float lhs, Base<BaseSI> rhs) NOEXCEPT
{
    return rhs *= lhs;
}

/* ************************************************************************ */

/**
 * @brief Multiply operator.
 *
 * @tparam BaseSI1 Basic SI unit type.
 * @tparam BaseSI2 Basic SI unit type.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result value.
 */
template<SI BaseSI1, SI BaseSI2>
inline CONSTEXPR Derived<Base<BaseSI1>, Base<BaseSI2>> operator*(Base<BaseSI1> lhs, Base<BaseSI2> rhs) NOEXCEPT
{
    return Derived<Base<BaseSI1>, Base<BaseSI2>>{rhs.value() * lhs.value()};
}

/* ************************************************************************ */

/**
 * @brief Multiply operator.
 *
 * @tparam BaseSI Basic SI unit type.
 * @tparam T1
 * @tparam T2
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result value.
 */
template<SI BaseSI, typename T1, typename T2>
inline CONSTEXPR Derived<Base<BaseSI>, Derived<T1, T2>> operator*(Base<BaseSI> lhs, Derived<T1, T2> rhs) NOEXCEPT
{
    return Derived<Base<BaseSI>, Derived<T1, T2>>{rhs.value() * lhs.value()};
}

/* ************************************************************************ */

/**
 * @brief Multiply operator.
 *
 * @tparam T1
 * @tparam T2
 * @tparam BaseSI Basic SI unit type.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result value.
 */
template<typename T1, typename T2, SI BaseSI>
inline CONSTEXPR Derived<Derived<T1, T2>, Base<BaseSI>> operator*(Derived<T1, T2> lhs, Base<BaseSI> rhs) NOEXCEPT
{
    return Derived<Derived<T1, T2>, Base<BaseSI>>{rhs.value() * lhs.value()};
}

/* ************************************************************************ */

/**
 * @brief Multiply operator.
 *
 * @tparam T11
 * @tparam T12
 * @tparam T21
 * @tparam T22
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result value.
 */
template<typename T11, typename T12, typename T21, typename T22>
inline CONSTEXPR Derived<Derived<T11, T12>, Derived<T21, T22>> operator*(Derived<T11, T12> lhs, Derived<T21, T22> rhs) NOEXCEPT
{
    return Derived<Derived<T11, T12>, Derived<T21, T22>>{rhs.value() * lhs.value()};
}

/* ************************************************************************ */

/**
 * @brief Dividing operator.
 *
 * @tparam BaseSI Basic SI unit type.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result value.
 */
template<SI BaseSI>
inline CONSTEXPR Base<BaseSI> operator/(Base<BaseSI> lhs, float rhs) NOEXCEPT
{
    return lhs /= rhs;
}

/* ************************************************************************ */

/**
 * @brief Dividing operator.
 *
 * @tparam BaseSI Basic SI unit type.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result value.
 */
template<SI BaseSI>
inline CONSTEXPR Base<BaseSI> operator/(float lhs, Base<BaseSI> rhs) NOEXCEPT
{
    return rhs /= lhs;
}

/* ************************************************************************ */

/**
 * @brief Dividing operator.
 *
 * @tparam BaseSI1 Basic SI unit type.
 * @tparam BaseSI2 Basic SI unit type.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result value.
 */
template<SI BaseSI1, SI BaseSI2>
inline CONSTEXPR Derived<Base<BaseSI1>, Divide<Base<BaseSI2>>> operator/(Base<BaseSI1> lhs, Base<BaseSI2> rhs) NOEXCEPT
{
    return Derived<Base<BaseSI1>, Divide<Base<BaseSI2>>>{rhs.value() / lhs.value()};
}

/* ************************************************************************ */

/**
 * @brief Dividing operator.
 *
 * @tparam BaseSI Basic SI unit type.
 * @tparam T1
 * @tparam T2
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result value.
 */
template<SI BaseSI, typename T1, typename T2>
inline CONSTEXPR Derived<Base<BaseSI>, Divide<Derived<T1, T2>>> operator/(Base<BaseSI> lhs, Derived<T1, T2> rhs) NOEXCEPT
{
    return Derived<Base<BaseSI>, Divide<Derived<T1, T2>>>{rhs.value() / lhs.value()};
}

/* ************************************************************************ */

/**
 * @brief Dividing operator.
 *
 * @tparam T1
 * @tparam T2
 * @tparam BaseSI Basic SI unit type.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result value.
 */
template<typename T1, typename T2, SI BaseSI>
inline CONSTEXPR Derived<Derived<T1, T2>, Divide<Base<BaseSI>>> operator/(Derived<T1, T2> lhs, Base<BaseSI> rhs) NOEXCEPT
{
    return Derived<Derived<T1, T2>, Divide<Base<BaseSI>>>{rhs.value() / lhs.value()};
}

/* ************************************************************************ */

/**
 * @brief Dividing operator.
 *
 * @tparam T11
 * @tparam T12
 * @tparam T21
 * @tparam T22
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result value.
 */
template<typename T11, typename T12, typename T21, typename T22>
inline CONSTEXPR Derived<Derived<T11, T12>, Divide<Derived<T21, T22>>> operator/(Derived<T11, T12> lhs, Derived<T21, T22> rhs) NOEXCEPT
{
    return Derived<Derived<T11, T12>, Divide<Derived<T21, T22>>>{rhs.value() / lhs.value()};
}

/* ************************************************************************ */

/**
 * @brief Class for representing distance (meters).
 */
using Length = float; //Base<SI::Length>;

/* ************************************************************************ */

/**
 * @brief Class for representing mass (kilograms).
 */
using Mass = float; // Base<SI::Mass>;

/* ************************************************************************ */

/**
 * @brief Class for representing time (seconds).
 */
using Time = float; // Base<SI::Time>;
using Duration = Time;

/* ************************************************************************ */

/**
 * @brief Class for representing area.
 */
using Area = float; // Derived<Length, Length>;

/* ************************************************************************ */

/**
 * @brief Class for representing volume.
 */
using Volume = float; // Derived<Area, Length>;

/* ************************************************************************ */

/**
 * @brief Class for representing velocity (micrometers per second).
 */
using Velocity = float; // Derived<Length, Divide<Time>>;

/* ************************************************************************ */

/**
 * @brief Class for representing acceleration (micrometers per second^2).
 */
using Acceleration = float; // Derived<Velocity, Divide<Time>>;

/* ************************************************************************ */

/**
 * @brief Class for representing force (Newton).
 */
using Force = float;

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
inline CONSTEXPR Area mm3(float value) NOEXCEPT
{
    return mm2(value) * mm(1);
}

/* ************************************************************************ */

/**
 * @brief Micrometers^3 value.
 *
 * @param value
 */
inline CONSTEXPR Area um3(float value) NOEXCEPT
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
    return m(value) / (s(1) * s(1));
}

/* ************************************************************************ */

/**
 * @brief Millimeters per second^2 value.
 *
 * @param value
 */
inline CONSTEXPR Velocity mm_s2(float value) NOEXCEPT
{
    return mm(value) / (s(1) * s(1));
}

/* ************************************************************************ */

/**
 * @brief Micrometers per second^2 value.
 *
 * @param value
 */
inline CONSTEXPR Velocity um_s2(float value) NOEXCEPT
{
    return um(value) / (s(1) * s(1));
}

/* ************************************************************************ */

/**
 * @brief Kilogram meter per second^2 value.
 *
 * @param value
 */
inline CONSTEXPR Force kgm_s2(float value) NOEXCEPT
{
    return kg(value) * m(1) / (s(1) * s(1));
}

/* ************************************************************************ */

/**
 * @brief Gram meter per second^2 value.
 *
 * @param value
 */
inline CONSTEXPR Force gm_s2(float value) NOEXCEPT
{
    return g(value) * m(1) / (s(1) * s(1));
}

/* ************************************************************************ */

/**
 * @brief Milligram meter per second^2 value.
 *
 * @param value
 */
inline CONSTEXPR Force mgm_s2(float value) NOEXCEPT
{
    return kg(value) * m(1) / (s(1) * s(1));
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

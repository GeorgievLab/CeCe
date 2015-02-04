
#pragma once

/* ************************************************************************ */

// C++
#include <type_traits>

/* ************************************************************************ */

/**
 * @brief Base class for units.
 */
template<class Rep>
class Unit
{
public:


    /**
     * @brief Default constructor.
     */
    constexpr Unit() = default;


    /**
     * @brief Copy constructor.
     */
    Unit(const Unit&) = default;


    /**
     * @brief Value constructor.
     *
     * @param v
     */
    constexpr explicit Unit(const Rep& v)
        : m_value(v)
    {
        // Nothing to do
    }


// Public Operators
public:


    /**
     * @brief Plus unary operator.
     *
     * @return *this.
     */
    constexpr Unit operator+() const noexcept
    {
        return *this;
    }


    /**
     * @brief Minus unary operator.
     */
    constexpr Unit operator-() const noexcept
    {
        return Unit(-value());
    }


    /**
     * @brief Prefix increment operator.
     */
    Unit& operator++() noexcept
    {
        ++m_value;
        return *this;
    }


    /**
     * @brief Postfix increment operator.
     */
    Unit operator++(int) noexcept
    {
        return Unit(m_value++);
    }


    /**
     * @brief Prefix decrement operator.
     */
    Unit& operator--() noexcept
    {
        --m_value;
        return *this;
    }


    /**
     * @brief Postfix decrement operator.
     */
    Unit operator--(int) noexcept
    {
        return Unit(m_value--);
    }


    /**
     * @brief Add operator.
     */
    Unit& operator+=(const Unit& d) noexcept
    {
        m_value += d.m_value;
        return *this;
    }


    /**
     * @brief Substract operator.
     */
    Unit& operator-=(const Unit& d) noexcept
    {
        m_value += d.m_value;
        return *this;
    }


    /**
     * @brief Multiplication operator.
     */
    Unit& operator*=(const Rep& rhs) noexcept
    {
        m_value *= rhs;
        return *this;
    }


    /**
     * @brief Divide operator.
     */
    Unit& operator/=(const Rep& rhs) noexcept
    {
        m_value /= rhs;
        return *this;
    }


    /**
     * @brief Modulo operator.
     */
    Unit& operator%=(const Rep& rhs) noexcept
    {
        m_value %= rhs;
        return *this;
    }


    /**
     * @brief Modulo operator.
     */
    Unit& operator%=(const Unit& rhs) noexcept
    {
        m_value %= rhs.m_value;
        return *this;
    }


// Public Accessors
public:


    /**
     * @brief Returns current value.
     *
     * @return
     */
    constexpr Rep value() const noexcept
    {
        return m_value;
    }


// Private Data Members
private:

    /// Stored value.
    Rep m_value{};

};

/* ************************************************************************ */

template<class Rep1, class Rep2>
inline constexpr
Unit<typename std::common_type<Rep1, Rep2>::type> operator+(const Unit<Rep1>& lhs, const Unit<Rep2>& rhs)
{
    return Unit<typename std::common_type<Rep1, Rep2>::type>{lhs.value() + rhs.value()};
}

/* ************************************************************************ */

template<class Rep1, class Rep2>
inline constexpr
Unit<typename std::common_type<Rep1, Rep2>::type> operator-(const Unit<Rep1>& lhs, const Unit<Rep2>& rhs)
{
    return Unit<typename std::common_type<Rep1, Rep2>::type>{lhs.value() - rhs.value()};
}

/* ************************************************************************ */

template<class Rep1, class Rep2>
inline constexpr
Unit<typename std::common_type<Rep1, Rep2>::type> operator*(const Unit<Rep1>& d, const Rep2& s)
{
    return Unit<typename std::common_type<Rep1, Rep2>::type>{d.value() * s};
}

/* ************************************************************************ */

template<class Rep1, class Rep2>
inline constexpr
Unit<typename std::common_type<Rep1, Rep2>::type> operator*(const Rep2& s, const Unit<Rep1>& d)
{
    return Unit<typename std::common_type<Rep1, Rep2>::type>{s * d.value()};
}

/* ************************************************************************ */

template<class Rep1, class Rep2>
inline constexpr
Unit<typename std::common_type<Rep1, Rep2>::type> operator/(const Unit<Rep1>& d, const Rep2& s)
{
    return Unit<typename std::common_type<Rep1, Rep2>::type>{d.value() / s};
}

/* ************************************************************************ */

template<class Rep1, class Rep2>
inline constexpr
Unit<typename std::common_type<Rep1, Rep2>::type> operator/(const Rep2& s, const Unit<Rep1>& d)
{
    return Unit<typename std::common_type<Rep1, Rep2>::type>{s / d.value()};
}

/* ************************************************************************ */

/**
 * @brief Class for representing distance (micrometers).
 */
using Length = Unit<float>;

/* ************************************************************************ */

/**
 * @brief Class for representing volume.
 */
using Volume = Unit<float>;

/* ************************************************************************ */

/**
 * @brief Class for representing volume.
 */
using Angle = Unit<float>;

/* ************************************************************************ */

/**
 * @brief Micrometers literal.
 */
inline Length operator"" _um(long double value)
{
    return Length(value);
}

/* ************************************************************************ */

/**
 * @brief Micrometers literal.
 */
inline Length operator"" _um(unsigned long long int value)
{
    return Length(value);
}

/* ************************************************************************ */

/**
 * @brief MilliMeters literal.
 */
inline Length operator"" _mm(unsigned long long int value)
{
    return Length(1000 * value);
}

/* ************************************************************************ */

/**
 * @brief CentiMeters literal.
 */
inline Length operator"" _cm(unsigned long long int value)
{
    return Length(10 * 1000 * value);
}

/* ************************************************************************ */

/**
 * @brief Meters literal.
 */
inline Length operator"" _m(unsigned long long int value)
{
    return Length(1000 * 1000 * value);
}

/* ************************************************************************ */

/**
 * @brief Micrometers^3 literal.
 */
inline Volume operator"" _um3(long double value)
{
    return Volume(value);
}

/* ************************************************************************ */

/**
 * @brief Micrometers^3 literal.
 */
inline Volume operator"" _um3(unsigned long long int value)
{
    return Volume(value);
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

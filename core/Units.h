
#ifndef _LIBRARY_UNITS_H_
#define _LIBRARY_UNITS_H_

/* ************************************************************************ */

// C++
#include <ratio>

/* ************************************************************************ */

/**
 * @brief Base class for units.
 */
template<class Rep, class Ratio>
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
    template<class Rep2>
    constexpr explicit Unit(const Rep2& v)
        : m_value(v)
    {
        // Nothing to do
    }


    /**
     * @brief Construct from different ratio.
     *
     * @param v
     */
    template<class Rep2, class Ratio2>
    constexpr Unit(const Unit<Rep2, Ratio2>& v)
        : m_value(v.value() * Ratio2::num / Ratio2::den)
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

template<class Rep1, class Ratio1, class Rep2, class Ratio2>
inline
typename std::common_type<Rep1, Rep2>::type
constexpr operator+(const Unit<Rep1, Ratio1>& lhs, const Unit<Rep2, Ratio2>& rhs)
{
    // TODO implement
    return typename std::common_type<Rep1, Rep2>::type{};
}

/* ************************************************************************ */

template<class Rep1, class Ratio1, class Rep2, class Ratio2>
inline
typename std::common_type<Rep1, Rep2>::type
constexpr operator-(const Unit<Rep1, Ratio1>& lhs, const Unit<Rep2, Ratio2>& rhs)
{
    // TODO implement
    return typename std::common_type<Rep1, Rep2>::type{};
}

/* ************************************************************************ */

template<class Rep1, class Period1, class Rep2>
inline
Unit<typename std::common_type<Rep1, Rep2>::type, Period1>
constexpr operator*(const Unit<Rep1, Period1>& d, const Rep2& s)
{
    return Unit<typename std::common_type<Rep1, Rep2>::type, Period1>{d.value() * s};
}

/* ************************************************************************ */

template<class Rep1, class Rep2, class Period2>
inline
Unit<typename std::common_type<Rep1, Rep2>::type, Period2>
constexpr operator*(const Rep1& s, const Unit<Rep2, Period2>& d)
{
    return Unit<typename std::common_type<Rep1, Rep2>::type, Period2>{s * d.value()};
}

/* ************************************************************************ */

template< class Rep1, class Period1, class Rep2>
inline
Unit<typename std::common_type<Rep1, Rep2>::type, Period1>
constexpr operator/(const Unit<Rep1, Period1>& d, const Rep2& s)
{
    return Unit<typename std::common_type<Rep1, Rep2>::type, Period1>{d.value() / s};
}

/* ************************************************************************ */

/**
 * @brief Class for representing distance (micrometers).
 */
template<class Rep = float, class Ratio = std::ratio<1> >
class Distance : public Unit<Rep, Ratio>
{
    using Unit<Rep, Ratio>::Unit;
};

/* ************************************************************************ */

/**
 * @brief Class for representing volume.
 */
template<class Rep = float, class Ratio = std::ratio<1> >
class Volume : public Unit<Rep, Ratio>
{
    using Unit<Rep, Ratio>::Unit;
};

/* ************************************************************************ */

using MicroMeters = Distance<>;

/* ************************************************************************ */

using MicroMeters3 = Volume<>;

/* ************************************************************************ */

/**
 * @brief Micrometers literal.
 */
inline MicroMeters operator"" _um(long double value)
{
    return MicroMeters(value);
}

/* ************************************************************************ */

/**
 * @brief Micrometers literal.
 */
inline MicroMeters operator"" _um(unsigned long long int value)
{
    return MicroMeters(value);
}

/* ************************************************************************ */

/**
 * @brief Micrometers^3 literal.
 */
inline MicroMeters3 operator"" _um3(long double value)
{
    return MicroMeters3(value);
}

/* ************************************************************************ */

/**
 * @brief Micrometers^3 literal.
 */
inline MicroMeters3 operator"" _um3(unsigned long long int value)
{
    return MicroMeters3(value);
}

/* ************************************************************************ */

#endif // _LIBRARY_UNITS_H_

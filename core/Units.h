
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


private:

    /// Stored value.
    Rep m_value{};

};

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

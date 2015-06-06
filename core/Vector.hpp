
#pragma once

/* ************************************************************************ */

// C++
#include <cmath>
#include <tuple>
#include <type_traits>

// Simulator
#include "core/Units.hpp"

// Simulator
#include "core/compatibility.hpp"

/* ************************************************************************ */

#ifndef _MSC_VER
inline namespace core {
#endif

/* ************************************************************************ */

/**
 * @brief 2D vector.
 */
template<typename T>
class Vector
{

// Public Ctors
public:


    /**
     * @brief Default constructor.
     */
    CONSTEXPR Vector() = default;


    /**
     * @brief Constructor.
     *
     * @param val Value for all vector coordinates.
     */
#if __APPLE__ && __MACH__
    /// NOTE: On Mac OS X explicit keyword causes problem with free operators deduction.
    CONSTEXPR Vector(T val) NOEXCEPT
#else
    explicit CONSTEXPR Vector(T val) NOEXCEPT
#endif
        : m_x(val), m_y(val)
    {
        // Nothing to do
    }


    /**
     * @brief Constructor.
     *
     * @param x
     * @param y
     */
    CONSTEXPR Vector(T x, T y) NOEXCEPT
        : m_x(x), m_y(y)
    {
        // Nothing to do
    }


    /**
     * @brief Copy constructor.
     *
     * @param v Source vector.
     */
    CONSTEXPR Vector(const Vector& v) = default;


    /**
     * @brief Copy constructor.
     *
     * @param v Source vector.
     */
    template<typename T2>
    CONSTEXPR Vector(const Vector<T2>& v) NOEXCEPT
        : m_x(static_cast<T>(v.getX())), m_y(static_cast<T>(v.getY()))
    {
        // Nothing to do
    }


    /**
     * @brief Move constructor.
     *
     * @param v Source vector.
     */
#ifndef _MSC_VER
    CONSTEXPR Vector(Vector&& v) = default;
#endif


// Public Operators
public:


    /**
     * @brief Assignment operator.
     *
     * @param v
     *
     * @return
     */
    Vector& operator=(const Vector& v) = default;


    /**
     * @brief Move assignment operator.
     *
     * @param v
     *
     * @return
     */
#ifndef _MSC_VER
    Vector& operator=(Vector&& v) = default;
#endif


    /**
     * @brief Inverse operator.
     *
     * @return
     */
    CONSTEXPR Vector operator-() const NOEXCEPT
    {
        return Vector{-getX(), -getY()};
    }


    /**
     * @brief Add operator.
     *
     * @param v
     *
     * @return
     */
    CONSTEXPR Vector operator+(const Vector& v) const NOEXCEPT
    {
        return Vector{getX() + v.getX(), getY() + v.getY()};
    }


    /**
     * @brief Add operator.
     *
     * @param v
     *
     * @return
     */
    Vector& operator+=(const Vector& v) NOEXCEPT
    {
        m_x += v.getX();
        m_y += v.getY();
        return *this;
    }


    /**
     * @brief Substract operator.
     *
     * @param v
     *
     * @return
     */
    CONSTEXPR Vector operator-(const Vector& v) const NOEXCEPT
    {
        return Vector{getX() - v.getX(), getY() - v.getY()};
    }


    /**
     * @brief Substract operator.
     *
     * @param v
     *
     * @return
     */
    Vector& operator-=(const Vector& v) NOEXCEPT
    {
        m_x -= v.m_x;
        m_y -= v.m_y;
        return *this;
    }


    /**
     * @brief Multiple operator.
     *
     * @param v
     *
     * @return
     */
    CONSTEXPR Vector operator*(const Vector& val) const NOEXCEPT
    {
        return Vector{getX() * val.getX(), getY() * val.getY()};
    }


    /**
     * @brief Multiple operator.
     *
     * @param v
     *
     * @return
     */
    Vector& operator*=(const Vector& val) NOEXCEPT
    {
        m_x *= val.getX();
        m_y *= val.getY();
        return *this;
    }


    /**
     * @brief Divide operator.
     *
     * @param v
     *
     * @return
     */
    CONSTEXPR Vector operator/(const Vector& val) const NOEXCEPT
    {
        return Vector{getX() / val.getX(), getY() / val.getY()};
    }


    /**
     * @brief Divide operator.
     *
     * @param v
     *
     * @return
     */
    Vector& operator/=(const Vector& val) NOEXCEPT
    {
        m_x /= val.getX();
        m_y /= val.getY();
        return *this;
    }


// Public Accessors
public:


    /**
     * @brief Returns X coordinate.
     *
     * @return
     */
    T& x() NOEXCEPT
    {
        return m_x;
    }


    /**
     * @brief Returns X coordinate.
     *
     * @return
     */
    T getX() const NOEXCEPT
    {
        return m_x;
    }


    /**
     * @brief Returns Y coordinate.
     *
     * @return
     */
    T& y() NOEXCEPT
    {
        return m_y;
    }


    /**
     * @brief Returns Y coordinate.
     *
     * @return
     */
    T getY() const NOEXCEPT
    {
        return m_y;
    }


    /**
     * @brief Returns width.
     *
     * @return
     */
    T& width() NOEXCEPT
    {
        return m_x;
    }


    /**
     * @brief Returns width.
     *
     * @return
     */
    T getWidth() const NOEXCEPT
    {
        return m_x;
    }


    /**
     * @brief Returns height.
     *
     * @return
     */
    T& height() NOEXCEPT
    {
        return m_y;
    }


    /**
     * @brief Returns height.
     *
     * @return
     */
    T getHeight() const NOEXCEPT
    {
        return m_y;
    }


    /**
     * @brief Check if given value is in given range.
     *
     * @param value Given value.
     * @param low   Minimum value (>=).
     * @param high  Maximum value (<).
     *
     * @return
     */
    static bool inRange(T value, T low, T high) NOEXCEPT
    {
        return value >= low && value < high;
    }


    /**
     * @brief Check if current vector is in given range.
     *
     * @param low  Minimum coordinates (>=).
     * @param high Maximum coordinates (<).
     *
     * @return
     */
    bool inRange(const Vector& low, const Vector& high) const NOEXCEPT
    {
        return (
            inRange(getX(), low.getX(), high.getX()) &&
            inRange(getY(), low.getY(), high.getY())
        );
    }


// Public Mutators
public:


    /**
     * @brief Set X coordinate.
     *
     * @param x
     */
    void setX(T x) NOEXCEPT
    {
        m_x = x;
    }


    /**
     * @brief Set Y coordinate.
     *
     * @param y
     */
    void setY(T y) NOEXCEPT
    {
        m_y = y;
    }


// Public Operations
public:


    /**
     * @brief Calculate vector length.
     *
     * @return
     */
    T getLength() const NOEXCEPT
    {
        return std::sqrt(getLengthSquared());
    }


    /**
     * @brief Calculate vector length - squared.
     *
     * @return
     */
    T getLengthSquared() const NOEXCEPT
    {
        return dot(*this);
    }


    /**
     * @brief Calculate dot of two vectors.
     *
     * @param rhs Second vector.
     *
     * @return
     */
    T dot(const Vector& rhs) const NOEXCEPT
    {
        return getX() * rhs.getX() + getY() * rhs.getY();
    }


    /**
     * @brief Rotate current vector and return rotated version.
     *
     * @param angle
     *
     * @return
     */
    Vector rotated(units::Angle angle) const NOEXCEPT
    {
        return Vector(
            getX() * cos(angle) - getY() * sin(angle),
            getX() * sin(angle) + getY() * cos(angle)
        );
    }


// Private Data Members
private:


    /// X coordinate.
    T m_x;

    /// Y coordinate.
    T m_y;

};

/* ************************************************************************ */

//extern template class Vector<float>;

/* ************************************************************************ */

//extern template class Vector<unsigned>;

/* ************************************************************************ */

/**
 * @brief Add two vectors.
 *
 * @param lhs
 * @param rhs
 *
 * @return New vector
 */
template<typename T1, typename T2, typename CT = typename std::common_type<T1, T2>::type>
inline Vector<CT> operator+(const Vector<T1>& lhs, const Vector<T2>& rhs) NOEXCEPT
{
    return Vector<CT>(lhs).operator+(Vector<CT>(rhs));
}

/* ************************************************************************ */

/**
 * @brief Add single value to vector.
 *
 * @param lhs
 * @param rhs
 *
 * @return New vector.
 */
template<typename T1, typename T2, typename CT = typename std::common_type<T1, T2>::type>
inline Vector<CT> operator+(const Vector<T1>& lhs, T2 rhs) NOEXCEPT
{
    return operator+(lhs, Vector<T1>(rhs));
}

/* ************************************************************************ */

/**
 * @brief Add single value to vector.
 *
 * @param lhs
 * @param rhs
 *
 * @return New vector
 */
template<typename T1, typename T2, typename CT = typename std::common_type<T1, T2>::type>
inline Vector<CT> operator+(T1 lhs, const Vector<T2>& rhs) NOEXCEPT
{
    return operator+(Vector<T1>(lhs), rhs);
}

/* ************************************************************************ */

/**
 * @brief Substract two vectors.
 *
 * @param lhs
 * @param rhs
 *
 * @return New vector
 */
template<typename T1, typename T2, typename CT = typename std::common_type<T1, T2>::type>
inline Vector<CT> operator-(const Vector<T1>& lhs, const Vector<T2>& rhs) NOEXCEPT
{
    return Vector<CT>(lhs).operator-(Vector<CT>(rhs));
}

/* ************************************************************************ */

/**
 * @brief Substract single value from vector.
 *
 * @param lhs
 * @param rhs
 *
 * @return New vector
 */
template<typename T1, typename T2, typename CT = typename std::common_type<T1, T2>::type>
inline Vector<CT> operator-(const Vector<T1>& lhs, T2 rhs) NOEXCEPT
{
    return operator-<T1, T2>(lhs, Vector<T2>(rhs));
}

/* ************************************************************************ */

/**
 * @brief Substract single value from vector.
 *
 * @param lhs
 * @param rhs
 *
 * @return New vector
 */
template<typename T1, typename T2, typename CT = typename std::common_type<T1, T2>::type>
inline Vector<CT> operator-(T1 lhs, const Vector<T2>& rhs) NOEXCEPT
{
    return operator-(Vector<T1>(lhs), rhs);
}

/* ************************************************************************ */

/**
 * @brief Multiple vectors.
 *
 * @param lhs
 * @param rhs
 *
 * @return New vector
 */
template<typename T1, typename T2, typename CT = typename std::common_type<T1, T2>::type>
inline Vector<CT> operator*(const Vector<T1>& lhs, const Vector<T2>& rhs) NOEXCEPT
{
    return Vector<CT>(lhs).operator*(Vector<CT>(rhs));
}

/* ************************************************************************ */

/**
 * @brief Multiple single value from vector.
 *
 * @param lhs
 * @param rhs
 *
 * @return New vector
 */
template<typename T1, typename T2, typename CT = typename std::common_type<T1, T2>::type>
inline Vector<CT> operator*(const Vector<T1>& lhs, T2 rhs) NOEXCEPT
{
    return operator*(lhs, Vector<T2>(rhs));
}

/* ************************************************************************ */

/**
 * @brief Multiple single value from vector.
 *
 * @param lhs
 * @param rhs
 *
 * @return New vector
 */
template<typename T1, typename T2, typename CT = typename std::common_type<T1, T2>::type>
inline Vector<CT> operator*(T1 lhs, const Vector<T2>& rhs) NOEXCEPT
{
    return operator*(Vector<T1>(lhs), rhs);
}

/* ************************************************************************ */

/**
 * @brief Divide vectors.
 *
 * @param lhs
 * @param rhs
 *
 * @return New vector
 */
template<typename T1, typename T2, typename CT = typename std::common_type<T1, T2>::type>
inline Vector<CT> operator/(const Vector<T1>& lhs, const Vector<T2>& rhs) NOEXCEPT
{
    return Vector<CT>(lhs).operator/(Vector<CT>(rhs));
}

/* ************************************************************************ */

/**
 * @brief Divide vector by one value.
 *
 * @param lhs
 * @param rhs
 *
 * @return New vector
 */
template<typename T1, typename T2, typename CT = typename std::common_type<T1, T2>::type>
inline Vector<CT> operator/(const Vector<T1>& lhs, T2 rhs) NOEXCEPT
{
    return operator/(lhs, Vector<T2>(rhs));
}

/* ************************************************************************ */

/**
 * @brief Divide single value by vector.
 *
 * @param lhs
 * @param rhs
 *
 * @return New vector.
 */
template<typename T1, typename T2, typename CT = typename std::common_type<T1, T2>::type>
inline Vector<CT> operator/(T1 lhs, const Vector<T2>& rhs) NOEXCEPT
{
    return operator/(Vector<T1>(lhs), rhs);
}

/* ************************************************************************ */

/**
 * @brief Compare vectors.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return
 */
template<typename T1, typename T2>
inline bool operator==(const Vector<T1>& lhs, const Vector<T2>& rhs) NOEXCEPT
{
    auto x1 = lhs.getX();
    auto y1 = lhs.getY();
    auto x2 = rhs.getX();
    auto y2 = rhs.getY();

    return std::tie(x1, y1) == std::tie(x2, y2);
}

/* ************************************************************************ */

/**
 * @brief Compare vectors.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return
 */
template<typename T1, typename T2>
inline bool operator!=(const Vector<T1>& lhs, const Vector<T2>& rhs) NOEXCEPT
{
    return !operator==(lhs, rhs);
}

/* ************************************************************************ */

/**
 * @brief Compare vectors.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return
 */
template<typename T1, typename T2>
inline bool operator<(const Vector<T1>& lhs, const Vector<T2>& rhs) NOEXCEPT
{
    using T = typename std::common_type<T1, T2>::type;

    T x1 = lhs.getX();
    T y1 = lhs.getY();
    T x2 = rhs.getX();
    T y2 = rhs.getY();

    return std::tie(x1, y1) < std::tie(x2, y2);
}

/* ************************************************************************ */

/**
 * @brief Compare vectors.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return
 */
template<typename T1, typename T2>
inline bool operator<=(const Vector<T1>& lhs, const Vector<T2>& rhs) NOEXCEPT
{
    return !operator>(lhs, rhs);
}

/* ************************************************************************ */

/**
 * @brief Compare vectors.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return
 */
template<typename T1, typename T2>
inline bool operator>(const Vector<T1>& lhs, const Vector<T2>& rhs) NOEXCEPT
{
    // Reversed operands
    return operator<(rhs, lhs);
}

/* ************************************************************************ */

/**
 * @brief Compare vectors.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return
 */
template<typename T1, typename T2>
inline bool operator>=(const Vector<T1>& lhs, const Vector<T2>& rhs) NOEXCEPT
{
    return !operator<(lhs, rhs);
}

/* ************************************************************************ */

#ifndef _MSC_VER
}
#endif

/* ************************************************************************ */

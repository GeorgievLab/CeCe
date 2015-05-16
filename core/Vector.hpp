
#pragma once

/* ************************************************************************ */

// C++
#include <cmath>
#include <tuple>
#include <type_traits>

// Simulator
#include "core/Units.hpp"

/* ************************************************************************ */

inline namespace core {

/* ************************************************************************ */

/**
 * @brief 2D vector.
 */
template<typename T>
class Vector
{


// Public Types
public:


    /**
     * @brief Common type helper.
     */
    template<typename T2>
    using CT = typename std::common_type<T, T2>::type;


// Public Ctors
public:


    /**
     * @brief Default constructor.
     */
    constexpr Vector() = default;


    /**
     * @brief Constructor.
     *
     * @param val Value for all vector coordinates.
     */
    explicit constexpr Vector(T val) noexcept
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
    constexpr Vector(T x, T y) noexcept
        : m_x(x), m_y(y)
    {
        // Nothing to do
    }


    /**
     * @brief Constructor.
     *
     * @param v Source vector.
     */
    template<typename T2>
    constexpr Vector(const Vector<T2>& v) noexcept
        : m_x(v.getX()), m_y(v.getY())
    {
        // Nothing to do
    }


// Public Operators
public:


    /**
     * @brief Assignment operator.
     *
     * @param v
     *
     * @return
     */
    template<typename T2>
    Vector& operator=(const Vector<T2>& v) noexcept
    {
        m_x = v.getX();
        m_y = v.getY();
        return *this;
    }


    /**
     * @brief Inverse operator.
     *
     * @return
     */
    constexpr Vector operator-() const noexcept
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
    template<typename T2>
    constexpr Vector<CT<T2>> operator+(const Vector<T2>& v) const noexcept
    {
        return Vector<CT<T2>>{getX() + v.getX(), getY() + v.getY()};
    }


    /**
     * @brief Add operator.
     *
     * @param v
     *
     * @return
     */
    template<typename T2>
    Vector& operator+=(const Vector<T2>& v) noexcept
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
    template<typename T2>
    constexpr Vector<CT<T2>> operator-(const Vector<T2>& v) const noexcept
    {
        return Vector<CT<T2>>{getX() - v.getX(), getY() - v.getY()};
    }


    /**
     * @brief Substract operator.
     *
     * @param v
     *
     * @return
     */
    template<typename T2>
    Vector& operator-=(const Vector<T2>& v) noexcept
    {
        m_x -= v.getX();
        m_y -= v.getY();
        return *this;
    }


    /**
     * @brief Multiple operator.
     *
     * @param v
     *
     * @return
     */
    template<typename T2>
    constexpr Vector<CT<T2>> operator*(const Vector<T2>& val) const noexcept
    {
        return Vector<CT<T2>>{getX() * val.getX(), getY() * val.getY()};
    }


    /**
     * @brief Multiple operator.
     *
     * @param v
     *
     * @return
     */
    template<typename T2>
    Vector& operator*=(const Vector<T2>& val) noexcept
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
    template<typename T2>
    constexpr Vector<CT<T2>> operator/(const Vector<T2>& val) const noexcept
    {
        return Vector<CT<T2>>{getX() / val.getX(), getY() / val.getY()};
    }


    /**
     * @brief Divide operator.
     *
     * @param v
     *
     * @return
     */
    template<typename T2>
    Vector& operator/=(const Vector<T2>& val) noexcept
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
    T& x() noexcept
    {
        return m_x;
    }


    /**
     * @brief Returns X coordinate.
     *
     * @return
     */
    T getX() const noexcept
    {
        return m_x;
    }


    /**
     * @brief Returns Y coordinate.
     *
     * @return
     */
    T& y() noexcept
    {
        return m_y;
    }


    /**
     * @brief Returns Y coordinate.
     *
     * @return
     */
    T getY() const noexcept
    {
        return m_y;
    }


    /**
     * @brief Returns width.
     *
     * @return
     */
    T& width() noexcept
    {
        return m_x;
    }


    /**
     * @brief Returns width.
     *
     * @return
     */
    T getWidth() const noexcept
    {
        return m_x;
    }


    /**
     * @brief Returns height.
     *
     * @return
     */
    T& height() noexcept
    {
        return m_y;
    }


    /**
     * @brief Returns height.
     *
     * @return
     */
    T getHeight() const noexcept
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
    template<typename T1, typename T2>
    static bool inRange(T value, T1 low, T2 high) noexcept
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
    template<typename T1, typename T2>
    bool inRange(const Vector<T1>& low, const Vector<T2>& high) const noexcept
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
    void setX(T x) noexcept
    {
        m_x = x;
    }


    /**
     * @brief Set Y coordinate.
     *
     * @param y
     */
    void setY(T y) noexcept
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
    T getLength() const noexcept
    {
        return std::sqrt(getLengthSquared());
    }


    /**
     * @brief Calculate vector length - squared.
     *
     * @return
     */
    T getLengthSquared() const noexcept
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
    template<typename T2>
    CT<T2> dot(const Vector<T2>& rhs) const noexcept
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
    Vector rotated(units::Angle angle) const noexcept
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

/**
 * @brief Add single value to vector.
 *
 * @param lhs
 * @param rhs
 *
 * @return New vector.
 */
template<typename T>
inline Vector<T> operator+(const Vector<T>& lhs, T rhs) noexcept
{
    return lhs + Vector<T>{rhs};
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
template<typename T>
inline Vector<T> operator+(T lhs, const Vector<T>& rhs) noexcept
{
    return Vector<T>{lhs} + rhs;
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
template<typename T1, typename T2>
inline Vector<typename std::common_type<T1, T2>::type> operator+(const Vector<T1>& lhs, T2 rhs) noexcept
{
    return lhs + Vector<T2>{rhs};
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
template<typename T1, typename T2>
inline Vector<typename std::common_type<T1, T2>::type> operator+(T1 lhs, const Vector<T2>& rhs) noexcept
{
    return Vector<T1>{lhs} + rhs;
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
template<typename T>
inline Vector<T> operator-(const Vector<T>& lhs, T rhs) noexcept
{
    return lhs - Vector<T>{rhs};
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
template<typename T>
inline Vector<T> operator-(T lhs, const Vector<T>& rhs) noexcept
{
    return Vector<T>{lhs} - rhs;
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
template<typename T1, typename T2>
inline Vector<typename std::common_type<T1, T2>::type> operator-(const Vector<T1>& lhs, T2 rhs) noexcept
{
    return lhs - Vector<T2>{rhs};
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
template<typename T1, typename T2>
inline Vector<typename std::common_type<T1, T2>::type> operator-(T1 lhs, const Vector<T2>& rhs) noexcept
{
    return Vector<T1>{lhs} - rhs;
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
template<typename T>
inline Vector<T> operator*(const Vector<T>& lhs, T rhs) noexcept
{
    return lhs * Vector<T>{rhs};
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
template<typename T>
inline Vector<T> operator*(T lhs, const Vector<T>& rhs) noexcept
{
    return Vector<T>{lhs} * rhs;
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
template<typename T1, typename T2>
inline Vector<typename std::common_type<T1, T2>::type> operator*(const Vector<T1>& lhs, T2 rhs) noexcept
{
    return lhs * Vector<T2>{rhs};
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
template<typename T1, typename T2>
inline Vector<typename std::common_type<T1, T2>::type> operator*(T1 lhs, const Vector<T2>& rhs) noexcept
{
    return Vector<T1>{lhs} * rhs;
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
template<typename T>
inline Vector<T> operator/(const Vector<T>& lhs, T rhs) noexcept
{
    return lhs / Vector<T>{rhs};
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
template<typename T>
inline Vector<T> operator/(T lhs, const Vector<T>& rhs) noexcept
{
    return Vector<T>{lhs} / rhs;
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
template<typename T1, typename T2>
inline Vector<typename std::common_type<T1, T2>::type> operator/(const Vector<T1>& lhs, T2 rhs) noexcept
{
    return lhs / Vector<T2>{rhs};
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
template<typename T1, typename T2>
inline Vector<typename std::common_type<T1, T2>::type> operator/(T1 lhs, const Vector<T2>& rhs) noexcept
{
    return Vector<T1>{lhs} / rhs;
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
static bool operator==(const Vector<T1>& lhs, const Vector<T2>& rhs) noexcept
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
static bool operator!=(const Vector<T1>& lhs, const Vector<T2>& rhs) noexcept
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
static bool operator<(const Vector<T1>& lhs, const Vector<T2>& rhs) noexcept
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
static bool operator<=(const Vector<T1>& lhs, const Vector<T2>& rhs) noexcept
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
static bool operator>(const Vector<T1>& lhs, const Vector<T2>& rhs) noexcept
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
static bool operator>=(const Vector<T1>& lhs, const Vector<T2>& rhs) noexcept
{
    return !operator<(lhs, rhs);
}

/* ************************************************************************ */

extern template class Vector<float>;

/* ************************************************************************ */

//extern template class Vector<unsigned>;

/* ************************************************************************ */

}

/* ************************************************************************ */

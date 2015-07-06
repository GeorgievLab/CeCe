/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */

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

// Public Types
public:


    /// Vector value type.
    using value_type = T;


    /// Zero value initializer type.
    struct DLL_EXPORT Zero_t { constexpr Zero_t() {} };


// Public Constants
public:


    /**
     * @brief Placeholder for zero vector.
     */
    static constexpr Zero_t Zero{};


// Public Ctors
public:


    /**
     * @brief Default constructor.
     */
    CONSTEXPR Vector()
        : m_x(), m_y()
    {
        // Nothing to do
    }


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
     * @brief Zero value constructor.
     */
    CONSTEXPR Vector(Zero_t) NOEXCEPT
        : m_x(), m_y()
    {
        // Nothing to do
    }


    /**
     * @brief Copy constructor.
     *
     * @param v Source vector.
     */
    template<typename T2>
    explicit CONSTEXPR Vector(const Vector<T2>& v) NOEXCEPT
        : m_x(static_cast<T>(v.getX())), m_y(static_cast<T>(v.getY()))
    {
        // Nothing to do
    }


// Public Operators
public:


    /**
     * @brief Unary plus operator.
     *
     * @return
     */
    CONSTEXPR Vector operator+() const NOEXCEPT
    {
        return *this;
    }


    /**
     * @brief Unary minus operator.
     *
     * @return
     */
    template<typename TI = T, typename std::enable_if<std::is_unsigned<TI>::value>::type* = 0>
    CONSTEXPR Vector operator-() const NOEXCEPT
    {
        return Vector{-getX(), -getY()};
    }


    /**
     * @brief Addition operator.
     *
     * @tparam T1 Type of right operand.
     *
     * @param rhs Right operand.
     *
     * @return *this.
     */
    template<typename T1, typename std::enable_if<std::is_same<decltype(T{} + T1{}), T>::value>::type* = nullptr>
    Vector& operator+=(const Vector<T1>& rhs) NOEXCEPT
    {
        m_x += rhs.getX();
        m_y += rhs.getY();
        return *this;
    }


    /**
     * @brief Subtraction operator.
     *
     * @tparam T1 Type of value in Vector operand.
     *
     * @param rhs Right operand.
     *
     * @return *this.
     */
    template<typename T1, typename std::enable_if<std::is_same<decltype(T{} - T1{}), T>::value>::type* = nullptr>
    Vector& operator-=(const Vector<T1>& rhs) NOEXCEPT
    {
        m_x -= rhs.getX();
        m_y -= rhs.getY();
        return *this;
    }


    /**
     * @brief Multiplication operator.
     *
     * @tparam T1 Type of right operand.
     *
     * @param rhs Right operand.
     *
     * @return *this.
     */
    template<typename T1, typename std::enable_if<std::is_same<decltype(T{} * T1{}), T>::value>::type* = nullptr>
    Vector& operator*=(T1 rhs) NOEXCEPT
    {
        m_x *= rhs;
        m_y *= rhs;
        return *this;
    }


    /**
     * @brief Multiplication operator.
     *
     * @tparam T1 Type of value in Vector operand.
     *
     * @param rhs Right operand.
     *
     * @return *this.
     */
    template<typename T1, typename std::enable_if<std::is_same<decltype(T{} * T1{}), T>::value>::type* = nullptr>
    Vector& operator*=(const Vector<T1>& rhs) NOEXCEPT
    {
        m_x *= rhs.getX();
        m_y *= rhs.getY();
        return *this;
    }


    /**
     * @brief Division operator.
     *
     * @tparam T1 Type of right operand.
     *
     * @param rhs Right operand.
     *
     * @return *this.
     */
    template<typename T1, typename std::enable_if<std::is_same<decltype(T{} / T1{}), T>::value>::type* = nullptr>
    Vector& operator/=(T1 rhs) NOEXCEPT
    {
        m_x /= rhs;
        m_y /= rhs;
        return *this;
    }


    /**
     * @brief Division operator.
     *
     * @tparam T1 Type of value in Vector operand.
     *
     * @param rhs Right operand.
     *
     * @return *this.
     */
    template<typename T1, typename std::enable_if<std::is_same<decltype(T{} / T1{}), T>::value>::type* = nullptr>
    Vector& operator/=(const Vector<T1>& rhs) NOEXCEPT
    {
        m_x /= rhs.getX();
        m_y /= rhs.getY();
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
        using std::sqrt;
        return static_cast<T>(sqrt(getLengthSquared()));
    }


    /**
     * @brief Calculate vector length - squared.
     *
     * @return
     */
    decltype(T{} * T{}) getLengthSquared() const NOEXCEPT
    {
        return dot(*this);
    }


    /**
     * @brief Calculate dot of two vectors.
     *
     * @param rhs Second vector.
     *
     * @return Dot product.
     */
    CONSTEXPR decltype(T{} * T{}) dot(const Vector& rhs) const NOEXCEPT
    {
        return getX() * rhs.getX() + getY() * rhs.getY();
    }


    /**
     * @brief Inverse current vector (1 / *this).
     *
     * @return
     */
    Vector inversed() const NOEXCEPT
    {
        return Vector(T(1) / getX(), T(1) / getY());
    }


    /**
     * @brief Inverse current vector (1 / *this).
     *
     * @return
     */
    template<typename T2>
    Vector<T2> inversed() const NOEXCEPT
    {
        return Vector<T2>(T2(1) / getX(), T2(1) / getY());
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
            static_cast<T>(getX() * cos(angle) - getY() * sin(angle)),
            static_cast<T>(getX() * sin(angle) + getY() * cos(angle))
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
 * @brief Vector for integer size.
 */
using Size = Vector<unsigned int>;

/* ************************************************************************ */

/**
 * @brief Vector for coordinates.
 */
using Coordinate = Vector<unsigned int>;

/* ************************************************************************ */

// TODO: GCC have problem with =default constructor in linking stage.
#ifndef __GNUC__
extern template class Vector<float>;
#endif

/* ************************************************************************ */

#ifndef __GNUC__
extern template class Vector<unsigned int>;
#endif

/* ************************************************************************ */

#ifndef __GNUC__
extern template class Vector<int>;
#endif

/* ************************************************************************ */

/**
 * @brief Addition operator.
 *
 * @tparam T1 Type of value in first Vector.
 * @tparam T2 Type of value in second Vector.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result vector.
 */
template<typename T1, typename T2>
inline Vector<decltype(T1{} + T2{})> operator+(const Vector<T1>& lhs, const Vector<T2>& rhs) NOEXCEPT
{
    return {lhs.getX() + rhs.getX(), lhs.getY() + rhs.getY()};
}

/* ************************************************************************ */

/**
 * @brief Addition operator.
 *
 * @tparam T1 Type of value in first Vector.
 * @tparam T2 Type of second operand.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result vector.
 */
template<typename T1, typename T2>
inline Vector<decltype(T1{} + T2{})> operator+(const Vector<T1>& lhs, T2 rhs) NOEXCEPT
{
    return {lhs.getX() + rhs, lhs.getY() + rhs};
}

/* ************************************************************************ */

/**
 * @brief Addition operator.
 *
 * @tparam T1 Type of first operand.
 * @tparam T2 Type of value in second Vector.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result vector.
 */
template<typename T1, typename T2>
inline Vector<decltype(T1{} + T2{})> operator+(T1 lhs, const Vector<T2>& rhs) NOEXCEPT
{
    return {lhs + rhs.getX(), lhs + rhs.getY()};
}

/* ************************************************************************ */

/**
 * @brief Substract operator.
 *
 * @tparam T1 Type of value in first Vector.
 * @tparam T2 Type of value in second Vector.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result vector.
 */
template<typename T1, typename T2>
inline Vector<decltype(T1{} - T2{})> operator-(const Vector<T1>& lhs, const Vector<T2>& rhs) NOEXCEPT
{
    return {lhs.getX() - rhs.getX(), lhs.getY() - rhs.getY()};
}

/* ************************************************************************ */

/**
 * @brief Substract operator.
 *
 * @tparam T1 Type of value in first Vector.
 * @tparam T2 Type of second operand.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result vector.
 */
template<typename T1, typename T2>
inline Vector<decltype(T1{} - T2{})> operator-(const Vector<T1>& lhs, T2 rhs) NOEXCEPT
{
    return {lhs.getX() - rhs, lhs.getY() - rhs};
}

/* ************************************************************************ */

/**
 * @brief Substract operator.
 *
 * @tparam T1 Type of first operand.
 * @tparam T2 Type of value in second Vector.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result vector.
 */
template<typename T1, typename T2>
inline Vector<decltype(T1{} - T2{})> operator-(T1 lhs, const Vector<T2>& rhs) NOEXCEPT
{
    return {lhs - rhs.getX(), lhs - rhs.getY()};
}

/* ************************************************************************ */

/**
 * @brief Multiplication operator.
 *
 * @tparam T1 Type of value in first Vector.
 * @tparam T2 Type of value in second Vector.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result vector.
 */
template<typename T1, typename T2>
inline Vector<decltype(T1{} * T2{})> operator*(const Vector<T1>& lhs, const Vector<T2>& rhs) NOEXCEPT
{
    return {lhs.getX() * rhs.getX(), lhs.getY() * rhs.getY()};
}

/* ************************************************************************ */

/**
 * @brief Multiplication operator.
 *
 * @tparam T1 Type of value in first Vector.
 * @tparam T2 Type of second operand.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result vector.
 */
template<typename T1, typename T2>
inline Vector<decltype(T1{} * T2{})> operator*(const Vector<T1>& lhs, T2 rhs) NOEXCEPT
{
    return {lhs.getX() * rhs, lhs.getY() * rhs};
}

/* ************************************************************************ */

/**
 * @brief Multiplication operator.
 *
 * @tparam T1 Type of first operand.
 * @tparam T2 Type of value in second Vector.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result vector.
 */
template<typename T1, typename T2>
inline Vector<decltype(T1{} * T2{})> operator*(T1 lhs, const Vector<T2>& rhs) NOEXCEPT
{
    return {lhs * rhs.getX(), lhs * rhs.getY()};
}

/* ************************************************************************ */

/**
 * @brief Division operator.
 *
 * @tparam T1 Type of value in first Vector.
 * @tparam T2 Type of value in second Vector.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result vector.
 */
template<typename T1, typename T2>
inline Vector<decltype(T1{} / T2{})> operator/(const Vector<T1>& lhs, const Vector<T2>& rhs) NOEXCEPT
{
    return {lhs.getX() / rhs.getX(), lhs.getY() / rhs.getY()};
}

/* ************************************************************************ */

/**
 * @brief Division operator.
 *
 * @tparam T1 Type of value in first Vector.
 * @tparam T2 Type of second operand.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result vector.
 */
template<typename T1, typename T2>
inline Vector<decltype(T1{} / T2{})> operator/(const Vector<T1>& lhs, T2 rhs) NOEXCEPT
{
    return {lhs.getX() / rhs, lhs.getY() / rhs};
}

/* ************************************************************************ */

/**
 * @brief Division operator.
 *
 * @tparam T1 Type of first operand.
 * @tparam T2 Type of value in second Vector.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result vector.
 */
template<typename T1, typename T2>
inline Vector<decltype(T1{} / T2{})> operator/(T1 lhs, const Vector<T2>& rhs) NOEXCEPT
{
    return {lhs / rhs.getX(), lhs / rhs.getY()};
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
    using T = decltype(T1{} < T2{});

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

/**
 * @brief Calculate dot product of two vectors.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Dot product.
 */
template<typename T1, typename T2>
inline CONSTEXPR decltype(T1{} * T2{}) dot(const Vector<T1>& lhs, const Vector<T2>& rhs) NOEXCEPT
{
    return lhs.getX() * rhs.getX() + lhs.getY() * rhs.getY();
}

/* ************************************************************************ */

#ifndef _MSC_VER
}
#endif

/* ************************************************************************ */

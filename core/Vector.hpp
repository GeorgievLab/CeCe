/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* ************************************************************************ */

#pragma once

/* ************************************************************************ */

// C++
#include <cmath>
#include <cassert>
#include <tuple>
#include <type_traits>

// Simulator
#include "core/Zero.hpp"
#include "core/Units.hpp"
#include "core/InStream.hpp"
#include "core/OutStream.hpp"

/* ************************************************************************ */

inline namespace core {

/* ************************************************************************ */

/**
 * @brief Two dimensional vector.
 */
template<typename T>
class Vector
{

// Public Types
public:


    /// Vector value type.
    using value_type = T;


// Public Ctors
public:


    /**
     * @brief Default constructor.
     */
    constexpr Vector()
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
    constexpr Vector(T val) noexcept
#else
    explicit constexpr Vector(T val) noexcept
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
    constexpr Vector(T x, T y) noexcept
        : m_x(x), m_y(y)
    {
        // Nothing to do
    }


    /**
     * @brief Zero value constructor.
     */
    constexpr Vector(Zero_t) noexcept
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
    explicit constexpr Vector(const Vector<T2>& v) noexcept
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
    constexpr Vector operator+() const noexcept
    {
        return *this;
    }


    /**
     * @brief Unary minus operator.
     *
     * @return
     */
    template<typename TI = T, typename std::enable_if<std::is_unsigned<TI>::value>::type* = nullptr>
    constexpr Vector operator-() const noexcept
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
    Vector& operator+=(const Vector<T1>& rhs) noexcept
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
    Vector& operator-=(const Vector<T1>& rhs) noexcept
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
    Vector& operator*=(T1 rhs) noexcept
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
    Vector& operator*=(const Vector<T1>& rhs) noexcept
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
    Vector& operator/=(T1 rhs) noexcept
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
    Vector& operator/=(const Vector<T1>& rhs) noexcept
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
    static constexpr bool inRange(T value, T low, T high) noexcept
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
    constexpr bool inRange(const Vector& low, const Vector& high) const noexcept
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
    template<typename T1 = decltype(T{} * T{}), typename std::enable_if<std::is_scalar<T1>::value>::type* = nullptr>
    T getLength() const noexcept
    {
        using std::sqrt;
        return static_cast<T>(sqrt(getLengthSquared()));
    }


    /**
     * @brief Calculate vector length - squared.
     *
     * @return
     */
    decltype(T{} * T{}) getLengthSquared() const noexcept
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
    constexpr decltype(T{} * T{}) dot(const Vector& rhs) const noexcept
    {
        return getX() * rhs.getX() + getY() * rhs.getY();
    }


    /**
     * @brief Inverse current vector (1 / *this).
     *
     * @return
     */
    template<typename T2>
    Vector<T2> inversed() const noexcept
    {
        return Vector<T2>(T2(1) / getX(), T2(1) / getY());
    }


    /**
     * @brief Rotate current vector and return rotated version.
     *
     * @param angle Rotation angle.
     *
     * @return
     */
    Vector rotated(units::Angle angle) const noexcept
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

// NOTE: GCC have problem with =default constructor in linking stage.
#if !defined(__GNUC__)
extern template class Vector<float>;
extern template class Vector<unsigned int>;
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
inline Vector<decltype(T1{} + T2{})> operator+(const Vector<T1>& lhs, const Vector<T2>& rhs) noexcept
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
inline Vector<decltype(T1{} + T2{})> operator+(const Vector<T1>& lhs, T2 rhs) noexcept
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
inline Vector<decltype(T1{} + T2{})> operator+(T1 lhs, const Vector<T2>& rhs) noexcept
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
inline Vector<decltype(T1{} - T2{})> operator-(const Vector<T1>& lhs, const Vector<T2>& rhs) noexcept
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
inline Vector<decltype(T1{} - T2{})> operator-(const Vector<T1>& lhs, T2 rhs) noexcept
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
inline Vector<decltype(T1{} - T2{})> operator-(T1 lhs, const Vector<T2>& rhs) noexcept
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
inline Vector<decltype(T1{} * T2{})> operator*(const Vector<T1>& lhs, const Vector<T2>& rhs) noexcept
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
inline Vector<decltype(T1{} * T2{})> operator*(const Vector<T1>& lhs, T2 rhs) noexcept
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
inline Vector<decltype(T1{} * T2{})> operator*(T1 lhs, const Vector<T2>& rhs) noexcept
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
inline Vector<decltype(T1{} / T2{})> operator/(const Vector<T1>& lhs, const Vector<T2>& rhs) noexcept
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
inline Vector<decltype(T1{} / T2{})> operator/(const Vector<T1>& lhs, T2 rhs) noexcept
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
inline Vector<decltype(T1{} / T2{})> operator/(T1 lhs, const Vector<T2>& rhs) noexcept
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
inline bool operator==(const Vector<T1>& lhs, const Vector<T2>& rhs) noexcept
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
template<typename T1>
inline bool operator==(const Vector<T1>& lhs, Zero_t rhs) noexcept
{
    return lhs == Vector<T1>(Zero);
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
template<typename T1>
inline bool operator==(Zero_t lhs, const Vector<T1>& rhs) noexcept
{
    return Vector<T1>(Zero) == rhs;
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
inline bool operator!=(const Vector<T1>& lhs, const Vector<T2>& rhs) noexcept
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
template<typename T1>
inline bool operator!=(const Vector<T1>& lhs, Zero_t rhs) noexcept
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
template<typename T1>
inline bool operator!=(Zero_t lhs, const Vector<T1>& rhs) noexcept
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
inline bool operator<(const Vector<T1>& lhs, const Vector<T2>& rhs) noexcept
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
template<typename T1>
inline bool operator<(const Vector<T1>& lhs, Zero_t rhs) noexcept
{
    return lhs < Vector<T1>{Zero};
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
template<typename T1>
inline bool operator<(Zero_t lhs, const Vector<T1>& rhs) noexcept
{
    return Vector<T1>{Zero} < rhs;
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
inline bool operator<=(const Vector<T1>& lhs, const Vector<T2>& rhs) noexcept
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
template<typename T1>
inline bool operator<=(const Vector<T1>& lhs, Zero_t rhs) noexcept
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
template<typename T1>
inline bool operator<=(Zero_t lhs, const Vector<T1>& rhs) noexcept
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
inline bool operator>(const Vector<T1>& lhs, const Vector<T2>& rhs) noexcept
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
template<typename T1>
inline bool operator>(const Vector<T1>& lhs, Zero_t rhs) noexcept
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
template<typename T1>
inline bool operator>(Zero_t lhs, const Vector<T1>& rhs) noexcept
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
inline bool operator>=(const Vector<T1>& lhs, const Vector<T2>& rhs) noexcept
{
    return !operator<(lhs, rhs);
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
template<typename T1>
inline bool operator>=(const Vector<T1>& lhs, Zero_t rhs) noexcept
{
    return !operator<(lhs, rhs);
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
template<typename T1>
inline bool operator>=(Zero_t lhs, const Vector<T1>& rhs) noexcept
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
inline constexpr decltype(T1{} * T2{}) dot(const Vector<T1>& lhs, const Vector<T2>& rhs) noexcept
{
    return lhs.getX() * rhs.getX() + lhs.getY() * rhs.getY();
}

/* ************************************************************************ */

/**
 * @brief Input stream operator.
 *
 * @param is     Input stream.
 * @param vector Result value.
 *
 * @return is.
 */
template<typename T>
InStream& operator>>(InStream& is, Vector<T>& vector)
{
    T v1;
    T v2;
    is >> v1;

    if (is >> v2)
    {
        vector = Vector<T>{v1, v2};
    }
    else
    {
        vector = Vector<T>{v1};
        is.clear();
        is.setstate(InStream::eofbit);
    }

    return is;
}

/* ************************************************************************ */

/**
 * @brief Output stream operator.
 *
 * @param os     Output stream.
 * @param vector Input value.
 *
 * @return os.
 */
template<typename T>
OutStream& operator<<(OutStream& os, const Vector<T>& vector) noexcept
{
    os << vector.getX() << " " << vector.getY();
    return os;
}

/* ************************************************************************ */

}

/* ************************************************************************ */

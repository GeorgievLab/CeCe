/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/*                                                                          */
/* This file is part of CeCe.                                               */
/*                                                                          */
/* CeCe is free software: you can redistribute it and/or modify             */
/* it under the terms of the GNU General Public License as published by     */
/* the Free Software Foundation, either version 3 of the License, or        */
/* (at your option) any later version.                                      */
/*                                                                          */
/* CeCe is distributed in the hope that it will be useful,                  */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of           */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            */
/* GNU General Public License for more details.                             */
/*                                                                          */
/* You should have received a copy of the GNU General Public License        */
/* along with CeCe.  If not, see <http://www.gnu.org/licenses/>.            */
/*                                                                          */
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
#include "core/StaticArray.hpp"
#include "core/InStream.hpp"
#include "core/OutStream.hpp"

/* ************************************************************************ */

inline namespace core {

/* ************************************************************************ */

/**
 * @brief N dimensional vector.
 *
 * @tparam T Element type.
 * @tparam N Number of elements.
 */
template<typename T, unsigned N = 2>
class Vector
{

// Public Types
public:


    /// Vector value type.
    using value_type = T;


// Public Contants
public:


    /// Number of elements
    static constexpr unsigned SIZE = N;


// Public Ctors
public:


    /**
     * @brief Default constructor.
     */
    Vector() noexcept
        : m_data{}
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
        : m_data{x, y}
    {
        // Nothing to do
    }


    /**
     * @brief Zero value constructor.
     */
    Vector(Zero_t) noexcept
    {
        for (unsigned i = 0; i < N; ++i)
            m_data[i] = T{};
    }


    /**
     * @brief Copy constructor.
     *
     * @param v Source vector.
     */
    template<typename T2>
    explicit Vector(const Vector<T2, N>& v) noexcept
    {
        for (unsigned i = 0; i < N; ++i)
            m_data[i] = static_cast<T>(v[i]);
    }


// Public Operators
public:


    /**
     * @brief Unary plus operator.
     *
     * @return
     */
    Vector operator+() const noexcept
    {
        return *this;
    }


    /**
     * @brief Unary minus operator.
     *
     * @return
     */
    template<typename TI = T, typename std::enable_if<std::is_unsigned<TI>::value>::type* = nullptr>
    Vector operator-() const noexcept
    {
        Vector res;

        for (unsigned i = 0; i < N; ++i)
            res[i] = -m_data[i];

        return res;
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
    Vector& operator+=(const Vector<T1, N>& rhs) noexcept
    {
        for (unsigned i = 0; i < N; ++i)
            m_data[i] += rhs[i];

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
    Vector& operator-=(const Vector<T1, N>& rhs) noexcept
    {
        for (unsigned i = 0; i < N; ++i)
            m_data[i] -= rhs[i];

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
        for (unsigned i = 0; i < N; ++i)
            m_data[i] *= rhs;

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
    Vector& operator*=(const Vector<T1, N>& rhs) noexcept
    {
        for (unsigned i = 0; i < N; ++i)
            m_data[i] *= rhs[i];

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
        for (unsigned i = 0; i < N; ++i)
            m_data[i] /= rhs;

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
    Vector& operator/=(const Vector<T1, N>& rhs) noexcept
    {
        for (unsigned i = 0; i < N; ++i)
            m_data[i] /= rhs[i];

        return *this;
    }


    /**
     * @brief Access operator.
     *
     * @param pos
     *
     * @return
     */
    T& operator[](unsigned pos) noexcept
    {
        return m_data[pos];
    }


    /**
     * @brief Access operator.
     *
     * @param pos
     *
     * @return
     */
    const T& operator[](unsigned pos) const noexcept
    {
        return m_data[pos];
    }


// Public Accessors
public:


    /**
     * @brief Returns vector size.
     *
     * @return
     */
    unsigned getSize() const noexcept
    {
        return SIZE;
    }


    /**
     * @brief Returns X coordinate.
     *
     * @return
     */
    T& x() noexcept
    {
        return m_data[0];
    }


    /**
     * @brief Returns X coordinate.
     *
     * @return
     */
    T getX() const noexcept
    {
        return m_data[0];
    }


    /**
     * @brief Returns Y coordinate.
     *
     * @return
     */
    T& y() noexcept
    {
        return m_data[1];
    }


    /**
     * @brief Returns Y coordinate.
     *
     * @return
     */
    T getY() const noexcept
    {
        return m_data[1];
    }


    /**
     * @brief Returns width.
     *
     * @return
     */
    T& width() noexcept
    {
        return m_data[0];
    }


    /**
     * @brief Returns width.
     *
     * @return
     */
    T getWidth() const noexcept
    {
        return m_data[0];
    }


    /**
     * @brief Returns height.
     *
     * @return
     */
    T& height() noexcept
    {
        return m_data[1];
    }


    /**
     * @brief Returns height.
     *
     * @return
     */
    T getHeight() const noexcept
    {
        return m_data[1];
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
        m_data[0] = x;
    }


    /**
     * @brief Set Y coordinate.
     *
     * @param y
     */
    void setY(T y) noexcept
    {
        m_data[1] = y;
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
    decltype(T{} * T{}) dot(const Vector& rhs) const noexcept
    {
        decltype(T{} * T{}) res{};

        for (unsigned i = 0; i < N; ++i)
            res += m_data[i] * rhs[i];

        return res;
    }


    /**
     * @brief Inverse current vector (1 / *this).
     *
     * @return
     */
    template<typename T2>
    Vector<T2> inversed() const noexcept
    {
        Vector<T2> res;

        for (unsigned i = 0; i < N; ++i)
            res[i] = T2(1) / m_data[i];

        return res;
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


// Public Operations
public:


    /**
     * @brief Create from single value.
     *
     * @param val
     *
     * @return
     */
    static Vector createSingle(T val) noexcept
    {
        Vector res;

        for (unsigned i = 0; i < N; ++i)
            res[i] = val;

        return res;
    }


// Private Data Members
private:


    /// Vector data.
    StaticArray<T, N> m_data;
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
 * @tparam N  Vector size.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result vector.
 */
template<typename T1, typename T2, unsigned N>
inline Vector<decltype(T1{} + T2{}), N> operator+(const Vector<T1, N>& lhs, const Vector<T2, N>& rhs) noexcept
{
    Vector<decltype(T1{} + T2{}), N> res;

    for (unsigned i = 0; i < N; ++i)
        res[i] = lhs[i] + rhs[i];

    return res;
}

/* ************************************************************************ */

/**
 * @brief Addition operator.
 *
 * @tparam T1 Type of value in first Vector.
 * @tparam T2 Type of second operand.
 * @tparam N  Vector size.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result vector.
 */
template<typename T1, typename T2, unsigned N>
inline Vector<decltype(T1{} + T2{}), N> operator+(const Vector<T1, N>& lhs, T2 rhs) noexcept
{
    Vector<decltype(T1{} + T2{}), N> res;

    for (unsigned i = 0; i < N; ++i)
        res[i] = lhs[i] + rhs;

    return res;
}

/* ************************************************************************ */

/**
 * @brief Addition operator.
 *
 * @tparam T1 Type of first operand.
 * @tparam T2 Type of value in second Vector.
 * @tparam N  Vector size.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result vector.
 */
template<typename T1, typename T2, unsigned N>
inline Vector<decltype(T1{} + T2{}), N> operator+(T1 lhs, const Vector<T2, N>& rhs) noexcept
{
    Vector<decltype(T1{} + T2{}), N> res;

    for (unsigned i = 0; i < N; ++i)
        res[i] = lhs + rhs[i];

    return res;
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
template<typename T1, typename T2, unsigned N>
inline Vector<decltype(T1{} - T2{}), N> operator-(const Vector<T1, N>& lhs, const Vector<T2, N>& rhs) noexcept
{
    Vector<decltype(T1{} - T2{}), N> res;

    for (unsigned i = 0; i < N; ++i)
        res[i] = lhs[i] - rhs[i];

    return res;
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
template<typename T1, typename T2, unsigned N>
inline Vector<decltype(T1{} - T2{}), N> operator-(const Vector<T1, N>& lhs, T2 rhs) noexcept
{
    Vector<decltype(T1{} - T2{}), N> res;

    for (unsigned i = 0; i < N; ++i)
        res[i] = lhs[i] - rhs;

    return res;
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
template<typename T1, typename T2, unsigned N>
inline Vector<decltype(T1{} - T2{}), N> operator-(T1 lhs, const Vector<T2, N>& rhs) noexcept
{
    Vector<decltype(T1{} - T2{}), N> res;

    for (unsigned i = 0; i < N; ++i)
        res[i] = lhs - rhs[i];

    return res;
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
template<typename T1, typename T2, unsigned N>
inline Vector<decltype(T1{} * T2{}), N> operator*(const Vector<T1, N>& lhs, const Vector<T2, N>& rhs) noexcept
{
    Vector<decltype(T1{} * T2{}), N> res;

    for (unsigned i = 0; i < N; ++i)
        res[i] = lhs[i] * rhs[i];

    return res;
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
template<typename T1, typename T2, unsigned N>
inline Vector<decltype(T1{} * T2{}), N> operator*(const Vector<T1, N>& lhs, T2 rhs) noexcept
{
    Vector<decltype(T1{} * T2{}), N> res;

    for (unsigned i = 0; i < N; ++i)
        res[i] = lhs[i] * rhs;

    return res;
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
template<typename T1, typename T2, unsigned N>
inline Vector<decltype(T1{} * T2{}), N> operator*(T1 lhs, const Vector<T2, N>& rhs) noexcept
{
    Vector<decltype(T1{} * T2{}), N> res;

    for (unsigned i = 0; i < N; ++i)
        res[i] = lhs * rhs[i];

    return res;
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
template<typename T1, typename T2, unsigned N>
inline Vector<decltype(T1{} / T2{}), N> operator/(const Vector<T1, N>& lhs, const Vector<T2, N>& rhs) noexcept
{
    Vector<decltype(T1{} / T2{}), N> res;

    for (unsigned i = 0; i < N; ++i)
        res[i] = lhs[i] / rhs[i];

    return res;
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
template<typename T1, typename T2, unsigned N>
inline Vector<decltype(T1{} / T2{}), N> operator/(const Vector<T1, N>& lhs, T2 rhs) noexcept
{
    Vector<decltype(T1{} / T2{}), N> res;

    for (unsigned i = 0; i < N; ++i)
        res[i] = lhs[i] / rhs;

    return res;
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
template<typename T1, typename T2, unsigned N>
inline Vector<decltype(T1{} / T2{}), N> operator/(T1 lhs, const Vector<T2, N>& rhs) noexcept
{
    Vector<decltype(T1{} / T2{}), N> res;

    for (unsigned i = 0; i < N; ++i)
        res[i] = lhs / rhs[i];

    return res;
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
template<typename T1, typename T2, unsigned N>
inline bool operator==(const Vector<T1, N>& lhs, const Vector<T2, N>& rhs) noexcept
{
    bool res = true;

    for (unsigned i = 0; i < N; ++i)
        res = res && lhs[i] == rhs[i];

    return res;
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
template<typename T1, unsigned N>
inline bool operator==(const Vector<T1, N>& lhs, Zero_t rhs) noexcept
{
    return lhs == Vector<T1, N>(Zero);
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
template<typename T1, unsigned N>
inline bool operator==(Zero_t lhs, const Vector<T1, N>& rhs) noexcept
{
    return Vector<T1, N>(Zero) == rhs;
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
template<typename T1, typename T2, unsigned N>
inline bool operator!=(const Vector<T1, N>& lhs, const Vector<T2, N>& rhs) noexcept
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
template<typename T1, unsigned N>
inline bool operator!=(const Vector<T1, N>& lhs, Zero_t rhs) noexcept
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
template<typename T1, unsigned N>
inline bool operator!=(Zero_t lhs, const Vector<T1, N>& rhs) noexcept
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
template<typename T1, typename T2, unsigned N>
inline bool operator<(const Vector<T1, N>& lhs, const Vector<T2, N>& rhs) noexcept
{
    using T = decltype(T1{} + T2{});

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
template<typename T1, unsigned N>
inline bool operator<(const Vector<T1, N>& lhs, Zero_t rhs) noexcept
{
    return lhs < Vector<T1, N>{Zero};
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
template<typename T1, unsigned N>
inline bool operator<(Zero_t lhs, const Vector<T1, N>& rhs) noexcept
{
    return Vector<T1, N>{Zero} < rhs;
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
template<typename T1, typename T2, unsigned N>
inline bool operator<=(const Vector<T1, N>& lhs, const Vector<T2, N>& rhs) noexcept
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
template<typename T1, unsigned N>
inline bool operator<=(const Vector<T1, N>& lhs, Zero_t rhs) noexcept
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
template<typename T1, unsigned N>
inline bool operator<=(Zero_t lhs, const Vector<T1, N>& rhs) noexcept
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
template<typename T1, typename T2, unsigned N>
inline bool operator>(const Vector<T1, N>& lhs, const Vector<T2, N>& rhs) noexcept
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
template<typename T1, unsigned N>
inline bool operator>(const Vector<T1, N>& lhs, Zero_t rhs) noexcept
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
template<typename T1, unsigned N>
inline bool operator>(Zero_t lhs, const Vector<T1, N>& rhs) noexcept
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
template<typename T1, typename T2, unsigned N>
inline bool operator>=(const Vector<T1, N>& lhs, const Vector<T2, N>& rhs) noexcept
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
template<typename T1, unsigned N>
inline bool operator>=(const Vector<T1, N>& lhs, Zero_t rhs) noexcept
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
template<typename T1, unsigned N>
inline bool operator>=(Zero_t lhs, const Vector<T1, N>& rhs) noexcept
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
template<typename T1, typename T2, unsigned N>
inline decltype(T1{} * T2{}) dot(const Vector<T1, N>& lhs, const Vector<T2, N>& rhs) noexcept
{
    decltype(T1{} * T2{}) res{};

    for (unsigned i = 0; i < N; ++i)
        res += lhs[i] * rhs[i];

    return res;
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
template<typename T, unsigned N>
InStream& operator>>(InStream& is, Vector<T, N>& vector)
{
    unsigned i = 0;

    for (; i < N; ++i)
    {
        if (!(is >> vector[i]))
            break;
    }

    // Copy missing values
    // TODO: have this feature?
    for (unsigned j = i; j < N; ++j)
        vector[j] = vector[i - 1];

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
template<typename T, unsigned N>
OutStream& operator<<(OutStream& os, const Vector<T, N>& vector) noexcept
{
    for (unsigned i = 0; i < N; ++i)
    {
        if (i != 0)
            os << " ";

        os << vector[i];
    }

    return os;
}

/* ************************************************************************ */

}

/* ************************************************************************ */

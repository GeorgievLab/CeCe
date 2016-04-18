/* ************************************************************************ */
/* Georgiev Lab (c) 2015-2016                                               */
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

// CeCe config
#include "cece/config.hpp"

/* ************************************************************************ */

// C++
#include <cmath>
#include <type_traits>

// CeCe
#include "cece/core/Assert.hpp"
#include "cece/core/Zero.hpp"
#include "cece/core/Units.hpp"
#include "cece/core/StaticArray.hpp"
#include "cece/core/InStream.hpp"
#include "cece/core/OutStream.hpp"

/* ************************************************************************ */

namespace cece {
inline namespace core {

/* ************************************************************************ */

/**
 * @brief N dimensional vector.
 *
 * @tparam T Element type.
 * @tparam N Number of elements.
 */
template<typename T, unsigned N>
class BasicVector
{

// Public Types
public:


    /// BasicVector value type.
    using ValueType = T;


// Public Contants
public:


    /// Number of elements
    static constexpr unsigned SIZE = N;


// Public Ctors
public:


    /**
     * @brief Default constructor.
     */
    BasicVector() noexcept
        : m_data{}
    {
        // Nothing to do
    }


    /**
     * @brief Constructor.
     *
     * @param data
     */
    explicit BasicVector(StaticArray<T, N> data) noexcept
        : m_data(data)
    {
        // Nothing to do
    }


    /**
     * @brief Zero value constructor.
     */
    BasicVector(Zero_t) noexcept
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
    explicit BasicVector(const BasicVector<T2, N>& v) noexcept
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
    BasicVector operator+() const noexcept
    {
        return *this;
    }


    /**
     * @brief Unary minus operator.
     *
     * @return
     */
    BasicVector operator-() const noexcept
    {
        BasicVector res;

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
    BasicVector& operator+=(const BasicVector<T1, N>& rhs) noexcept
    {
        for (unsigned i = 0; i < N; ++i)
            m_data[i] += rhs[i];

        return *this;
    }


    /**
     * @brief Subtraction operator.
     *
     * @tparam T1 Type of value in BasicVector operand.
     *
     * @param rhs Right operand.
     *
     * @return *this.
     */
    template<typename T1, typename std::enable_if<std::is_same<decltype(T{} - T1{}), T>::value>::type* = nullptr>
    BasicVector& operator-=(const BasicVector<T1, N>& rhs) noexcept
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
    BasicVector& operator*=(T1 rhs) noexcept
    {
        for (unsigned i = 0; i < N; ++i)
            m_data[i] *= rhs;

        return *this;
    }


    /**
     * @brief Multiplication operator.
     *
     * @tparam T1 Type of value in BasicVector operand.
     *
     * @param rhs Right operand.
     *
     * @return *this.
     */
    template<typename T1, typename std::enable_if<std::is_same<decltype(T{} * T1{}), T>::value>::type* = nullptr>
    BasicVector& operator*=(const BasicVector<T1, N>& rhs) noexcept
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
    BasicVector& operator/=(T1 rhs) noexcept
    {
        for (unsigned i = 0; i < N; ++i)
            m_data[i] /= rhs;

        return *this;
    }


    /**
     * @brief Division operator.
     *
     * @tparam T1 Type of value in BasicVector operand.
     *
     * @param rhs Right operand.
     *
     * @return *this.
     */
    template<typename T1, typename std::enable_if<std::is_same<decltype(T{} / T1{}), T>::value>::type* = nullptr>
    BasicVector& operator/=(const BasicVector<T1, N>& rhs) noexcept
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
    bool inRange(const BasicVector& low, const BasicVector& high) const noexcept
    {
        bool res = true;

        for (unsigned i = 0; i < N; ++i)
            res = res && inRange(m_data[i], low[i], high[i]);

        return res;
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
    decltype(T{} * T{}) dot(const BasicVector& rhs) const noexcept
    {
        decltype(T{} * T{}) res{};

        for (unsigned i = 0; i < N; ++i)
            res += m_data[i] * rhs[i];

        return res;
    }


    /**
     * @brief Calculate vectors squared distance.
     *
     * @param rhs Second vector.
     *
     * @return Distance.
     */
    decltype(std::declval<T>() * std::declval<T>()) distanceSquared(const BasicVector& rhs) const noexcept
    {
        return (*this - rhs).getLengthSquared();
    }


    /**
     * @brief Calculate vectors distance.
     *
     * @param rhs Second vector.
     *
     * @return Distance.
     */
    T distance(const BasicVector& rhs) const noexcept
    {
        return (*this - rhs).getLength();
    }


    /**
     * @brief Inverse current vector (1 / *this).
     *
     * @return
     */
    template<typename T2>
    BasicVector<T2, SIZE> inversed() const noexcept
    {
        BasicVector<T2, SIZE> res;

        for (unsigned i = 0; i < N; ++i)
            res[i] = T2(1) / m_data[i];

        return res;
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
    static BasicVector createSingle(T val) noexcept
    {
        BasicVector res;

        for (unsigned i = 0; i < N; ++i)
            res[i] = val;

        return res;
    }


// Private Data Members
private:


    /// BasicVector data.
    StaticArray<T, N> m_data;
};

/* ************************************************************************ */

/**
 * @brief Two dimensional vector.
 */
template<typename T>
struct BasicVector<T, 2>
{

// Public Types
public:


    /// BasicVector value type.
    using ValueType = T;


// Public Contants
public:


    /// Number of elements
    static constexpr unsigned SIZE = 2;


// Public Ctors
public:


    /**
     * @brief Default constructor.
     */
    BasicVector() = default;


    /**
     * @brief Constructor.
     *
     * @param x
     * @param y
     */
    constexpr BasicVector(T x, T y) noexcept
        : m_x(x)
        , m_y(y)
    {
        // Nothing to do
    }


    /**
     * @brief Zero value constructor.
     */
    constexpr BasicVector(Zero_t) noexcept
        : m_x{}
        , m_y{}
    {
        // Nothing to do
    }


    /**
     * @brief Copy constructor.
     *
     * @param rhs Source vector.
     */
    template<typename T2>
    explicit BasicVector(const BasicVector<T2, SIZE>& rhs) noexcept
        : m_x(static_cast<T>(rhs.getX()))
        , m_y(static_cast<T>(rhs.getY()))
    {
        // Nothing to do
    }


// Public Operators
public:


    /**
     * @brief Access operator.
     *
     * @param pos
     *
     * @return
     */
    T& operator[](unsigned pos) noexcept
    {
        Assert(pos < SIZE);
        return (&m_x)[pos];
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
        Assert(pos < SIZE);
        return (&m_x)[pos];
    }


    /**
     * @brief Unary plus operator.
     *
     * @return
     */
    constexpr BasicVector operator+() const noexcept
    {
        return *this;
    }


    /**
     * @brief Unary minus operator.
     *
     * @return
     */
    constexpr BasicVector operator-() const noexcept
    {
        return BasicVector{-getX(), -getY()};
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
    template<typename T1>
    BasicVector& operator+=(const BasicVector<T1, SIZE>& rhs) noexcept
    {
        m_x += rhs.getX();
        m_y += rhs.getY();
        return *this;
    }


    /**
     * @brief Subtraction operator.
     *
     * @tparam T1 Type of value in BasicVector operand.
     *
     * @param rhs Right operand.
     *
     * @return *this.
     */
    template<typename T1>
    BasicVector& operator-=(const BasicVector<T1, SIZE>& rhs) noexcept
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
    template<typename T1>
    BasicVector& operator*=(T1 rhs) noexcept
    {
        m_x *= rhs;
        m_y *= rhs;
        return *this;
    }


    /**
     * @brief Multiplication operator.
     *
     * @tparam T1 Type of value in BasicVector operand.
     *
     * @param rhs Right operand.
     *
     * @return *this.
     */
    template<typename T1>
    BasicVector& operator*=(const BasicVector<T1, SIZE>& rhs) noexcept
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
    template<typename T1>
    BasicVector& operator/=(T1 rhs) noexcept
    {
        m_x /= rhs;
        m_y /= rhs;
        return *this;
    }


    /**
     * @brief Division operator.
     *
     * @tparam T1 Type of value in BasicVector operand.
     *
     * @param rhs Right operand.
     *
     * @return *this.
     */
    template<typename T1>
    BasicVector& operator/=(const BasicVector<T1, SIZE>& rhs) noexcept
    {
        m_x /= rhs.getX();
        m_y /= rhs.getY();
        return *this;
    }


// Public Accessors
public:


    /**
     * @brief Returns vector size.
     *
     * @return
     */
    constexpr unsigned getSize() const noexcept
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
        return m_x;
    }


    /**
     * @brief Returns X coordinate.
     *
     * @return
     */
    constexpr T getX() const noexcept
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
    constexpr T getY() const noexcept
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
    constexpr T getWidth() const noexcept
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
    constexpr T getHeight() const noexcept
    {
        return m_y;
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
    constexpr bool inRange(const BasicVector& low, const BasicVector& high) const noexcept
    {
        return (
            inRange(getX(), low.getX(), high.getX()) &&
            inRange(getY(), low.getY(), high.getY())
        );
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
        using std::sqrt;
        return static_cast<T>(sqrt(getLengthSquared()));
    }


    /**
     * @brief Calculate vector length - squared.
     *
     * @return
     */
    constexpr decltype(T{} * T{}) getLengthSquared() const noexcept
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
    constexpr decltype(T{} * T{}) dot(const BasicVector& rhs) const noexcept
    {
        return {getX() * rhs.getX() + getY() * rhs.getY()};
    }


    /**
     * @brief Calculate vectors squared distance.
     *
     * @param rhs Second vector.
     *
     * @return Distance.
     */
    decltype(std::declval<T>() * std::declval<T>()) distanceSquared(const BasicVector& rhs) const noexcept
    {
        return (*this - rhs).getLengthSquared();
    }


    /**
     * @brief Calculate vectors distance.
     *
     * @param rhs Second vector.
     *
     * @return Distance.
     */
    T distance(const BasicVector& rhs) const noexcept
    {
        return (*this - rhs).getLength();
    }


    /**
     * @brief Inverse current vector (1 / *this).
     *
     * @return
     */
    template<typename T2>
    constexpr BasicVector<T2, SIZE> inversed() const noexcept
    {
        return BasicVector<T2, SIZE>{T2(1) / getX(), T2(1) / getY()};
    }


    /**
     * @brief Rotate current vector and return rotated version.
     *
     * @param angle Rotation angle.
     *
     * @return
     */
    BasicVector rotated(units::Angle angle) const noexcept
    {
        return BasicVector(
            static_cast<T>(getX() * cos(angle.value()) - getY() * sin(angle.value())),
            static_cast<T>(getX() * sin(angle.value()) + getY() * cos(angle.value()))
        );
    }


    /**
     * @brief Create from single value.
     *
     * @param val
     *
     * @return
     */
    static constexpr BasicVector createSingle(T val) noexcept
    {
        return BasicVector{val, val};
    }


// Private Data Members
private:

    /// X coordinate.
    T m_x{};

    /// Y coordinate.
    T m_y{};

};

/* ************************************************************************ */

/**
 * @brief Three dimensional vector.
 */
template<typename T>
struct BasicVector<T, 3>
{

// Public Types
public:


    /// BasicVector value type.
    using ValueType = T;


// Public Contants
public:


    /// Number of elements
    static constexpr unsigned SIZE = 3;


// Public Ctors
public:


    /**
     * @brief Default constructor.
     */
    BasicVector() = default;


    /**
     * @brief Constructor.
     *
     * @param x
     * @param y
     * @param z
     */
    constexpr BasicVector(T x, T y, T z) noexcept
        : m_x(x)
        , m_y(y)
        , m_z(z)
    {
        // Nothing to do
    }


    /**
     * @brief Zero value constructor.
     */
    constexpr BasicVector(Zero_t) noexcept
        : m_x{}
        , m_y{}
        , m_z{}
    {
        // Nothing to do
    }


    /**
     * @brief Copy constructor.
     *
     * @param rhs Source vector.
     */
    template<typename T2>
    explicit BasicVector(const BasicVector<T2, SIZE>& rhs) noexcept
        : m_x(static_cast<T>(rhs.getX()))
        , m_y(static_cast<T>(rhs.getY()))
        , m_z(static_cast<T>(rhs.getZ()))
    {
        // Nothing to do
    }


// Public Operators
public:


    /**
     * @brief Access operator.
     *
     * @param pos
     *
     * @return
     */
    T& operator[](unsigned pos) noexcept
    {
        Assert(pos < SIZE);
        return (&m_x)[pos];
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
        Assert(pos < SIZE);
        return (&m_x)[pos];
    }


    /**
     * @brief Unary plus operator.
     *
     * @return
     */
    constexpr BasicVector operator+() const noexcept
    {
        return *this;
    }


    /**
     * @brief Unary minus operator.
     *
     * @return
     */
    constexpr BasicVector operator-() const noexcept
    {
        return BasicVector{-getX(), -getY(), -getZ()};
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
    BasicVector& operator+=(const BasicVector<T1, SIZE>& rhs) noexcept
    {
        m_x += rhs.getX();
        m_y += rhs.getY();
        m_z += rhs.getZ();
        return *this;
    }


    /**
     * @brief Subtraction operator.
     *
     * @tparam T1 Type of value in BasicVector operand.
     *
     * @param rhs Right operand.
     *
     * @return *this.
     */
    template<typename T1, typename std::enable_if<std::is_same<decltype(T{} - T1{}), T>::value>::type* = nullptr>
    BasicVector& operator-=(const BasicVector<T1, SIZE>& rhs) noexcept
    {
        m_x -= rhs.getX();
        m_y -= rhs.getY();
        m_z -= rhs.getZ();
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
    BasicVector& operator*=(T1 rhs) noexcept
    {
        m_x *= rhs;
        m_y *= rhs;
        m_z *= rhs;
        return *this;
    }


    /**
     * @brief Multiplication operator.
     *
     * @tparam T1 Type of value in BasicVector operand.
     *
     * @param rhs Right operand.
     *
     * @return *this.
     */
    template<typename T1, typename std::enable_if<std::is_same<decltype(T{} * T1{}), T>::value>::type* = nullptr>
    BasicVector& operator*=(const BasicVector<T1, SIZE>& rhs) noexcept
    {
        m_x *= rhs.getX();
        m_y *= rhs.getY();
        m_z *= rhs.getZ();
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
    BasicVector& operator/=(T1 rhs) noexcept
    {
        m_x /= rhs;
        m_y /= rhs;
        m_z /= rhs;
        return *this;
    }


    /**
     * @brief Division operator.
     *
     * @tparam T1 Type of value in BasicVector operand.
     *
     * @param rhs Right operand.
     *
     * @return *this.
     */
    template<typename T1, typename std::enable_if<std::is_same<decltype(T{} / T1{}), T>::value>::type* = nullptr>
    BasicVector& operator/=(const BasicVector<T1, SIZE>& rhs) noexcept
    {
        m_x /= rhs.getX();
        m_y /= rhs.getY();
        m_z /= rhs.getZ();
        return *this;
    }


// Public Accessors
public:


    /**
     * @brief Returns vector size.
     *
     * @return
     */
    constexpr unsigned getSize() const noexcept
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
        return m_x;
    }


    /**
     * @brief Returns X coordinate.
     *
     * @return
     */
    constexpr T getX() const noexcept
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
    constexpr T getY() const noexcept
    {
        return m_y;
    }


    /**
     * @brief Returns Z coordinate.
     *
     * @return
     */
    T& z() noexcept
    {
        return m_z;
    }


    /**
     * @brief Returns Z coordinate.
     *
     * @return
     */
    constexpr T getZ() const noexcept
    {
        return m_z;
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
    constexpr T getWidth() const noexcept
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
    constexpr T getHeight() const noexcept
    {
        return m_y;
    }


    /**
     * @brief Returns depth.
     *
     * @return
     */
    T& depth() noexcept
    {
        return m_z;
    }


    /**
     * @brief Returns depth.
     *
     * @return
     */
    constexpr T getDepth() const noexcept
    {
        return m_z;
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
    constexpr bool inRange(const BasicVector& low, const BasicVector& high) const noexcept
    {
        return (
            inRange(getX(), low.getX(), high.getX()) &&
            inRange(getY(), low.getY(), high.getY()) &&
            inRange(getZ(), low.getZ(), high.getZ())
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


    /**
     * @brief Set Z coordinate.
     *
     * @param z
     */
    void setZ(T z) noexcept
    {
        m_z = z;
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
    constexpr decltype(T{} * T{}) getLengthSquared() const noexcept
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
    constexpr decltype(T{} * T{}) dot(const BasicVector& rhs) const noexcept
    {
        return {
            getX() * rhs.getX() +
            getY() * rhs.getY() +
            getZ() * rhs.getZ()
        };
    }


    /**
     * @brief Calculate vectors squared distance.
     *
     * @param rhs Second vector.
     *
     * @return Distance.
     */
    decltype(std::declval<T>() * std::declval<T>()) distanceSquared(const BasicVector& rhs) const noexcept
    {
        return (*this - rhs).getLengthSquared();
    }


    /**
     * @brief Calculate vectors distance.
     *
     * @param rhs Second vector.
     *
     * @return Distance.
     */
    T distance(const BasicVector& rhs) const noexcept
    {
        return (*this - rhs).getLength();
    }


    /**
     * @brief Inverse current vector (1 / *this).
     *
     * @return
     */
    template<typename T2>
    constexpr BasicVector<T2, SIZE> inversed() const noexcept
    {
        return BasicVector<T2, SIZE>{
            T2(1) / getX(),
            T2(1) / getY(),
            T2(1) / getZ()
        };
    }


    /**
     * @brief Create from single value.
     *
     * @param val
     *
     * @return
     */
    static constexpr BasicVector createSingle(T val) noexcept
    {
        return {val, val, val};
    }


// Private Data Members
private:

    /// X coordinate.
    T m_x{};

    /// Y coordinate.
    T m_y{};

    /// Z coordinate.
    T m_z{};

};

/* ************************************************************************ */

/**
 * @brief Basic vector.
 */
template<typename T>
using Vector = BasicVector<T, config::DIMENSION>;

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

/**
 * @brief Vector of int.
 */
using VectorInt = Vector<int>;

/* ************************************************************************ */

/**
 * @brief Vector of unsigned int.
 */
using VectorUint = Vector<unsigned int>;

/* ************************************************************************ */

/**
 * @brief Vector of float.
 */
using VectorFloat = Vector<float>;

/* ************************************************************************ */

// NOTE: GCC have problem with =default constructor in linking stage.
#if !defined(__GNUC__)
extern template class BasicVector<float>;
extern template class BasicVector<unsigned int>;
extern template class BasicVector<int>;
#endif

/* ************************************************************************ */

/**
 * @brief Addition operator.
 *
 * @tparam T1 Type of value in first BasicVector.
 * @tparam T2 Type of value in second BasicVector.
 * @tparam N  BasicVector size.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result vector.
 */
template<typename T1, typename T2, unsigned N>
inline BasicVector<decltype(T1{} + T2{}), N> operator+(const BasicVector<T1, N>& lhs, const BasicVector<T2, N>& rhs) noexcept
{
    BasicVector<decltype(T1{} + T2{}), N> res;

    for (unsigned i = 0; i < N; ++i)
        res[i] = lhs[i] + rhs[i];

    return res;
}

/* ************************************************************************ */

/**
 * @brief Addition operator.
 *
 * @tparam T1 Type of value in first BasicVector.
 * @tparam T2 Type of value in second BasicVector.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result vector.
 */
template<typename T1, typename T2>
inline BasicVector<decltype(T1{} + T2{}), 2> operator+(const BasicVector<T1, 2>& lhs, const BasicVector<T2, 2>& rhs) noexcept
{
    return BasicVector<decltype(T1{} + T2{}), 2>{
        lhs.getX() + rhs.getX(),
        lhs.getY() + rhs.getY()
    };
}

/* ************************************************************************ */

/**
 * @brief Addition operator.
 *
 * @tparam T1 Type of value in first BasicVector.
 * @tparam T2 Type of second operand.
 * @tparam N  BasicVector size.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result vector.
 */
template<typename T1, typename T2, unsigned N>
inline BasicVector<decltype(T1{} + T2{}), N> operator+(const BasicVector<T1, N>& lhs, T2 rhs) noexcept
{
    BasicVector<decltype(T1{} + T2{}), N> res;

    for (unsigned i = 0; i < N; ++i)
        res[i] = lhs[i] + rhs;

    return res;
}

/* ************************************************************************ */

/**
 * @brief Addition operator.
 *
 * @tparam T1 Type of value in first BasicVector.
 * @tparam T2 Type of second operand.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result vector.
 */
template<typename T1, typename T2>
inline BasicVector<decltype(T1{} + T2{}), 2> operator+(const BasicVector<T1, 2>& lhs, T2 rhs) noexcept
{
    return BasicVector<decltype(T1{} + T2{}), 2>{
        lhs.getX() + rhs,
        lhs.getY() + rhs
    };
}

/* ************************************************************************ */

/**
 * @brief Addition operator.
 *
 * @tparam T1 Type of first operand.
 * @tparam T2 Type of value in second BasicVector.
 * @tparam N  BasicVector size.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result vector.
 */
template<typename T1, typename T2, unsigned N>
inline BasicVector<decltype(T1{} + T2{}), N> operator+(T1 lhs, const BasicVector<T2, N>& rhs) noexcept
{
    BasicVector<decltype(T1{} + T2{}), N> res;

    for (unsigned i = 0; i < N; ++i)
        res[i] = lhs + rhs[i];

    return res;
}

/* ************************************************************************ */

/**
 * @brief Addition operator.
 *
 * @tparam T1 Type of first operand.
 * @tparam T2 Type of value in second BasicVector.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result vector.
 */
template<typename T1, typename T2>
inline BasicVector<decltype(T1{} + T2{}), 2> operator+(T1 lhs, const BasicVector<T2, 2>& rhs) noexcept
{
    return BasicVector<decltype(T1{} + T2{}), 2>{
        lhs + rhs.getX(),
        lhs + rhs.getY()
    };
}

/* ************************************************************************ */

/**
 * @brief Substract operator.
 *
 * @tparam T1 Type of value in first BasicVector.
 * @tparam T2 Type of value in second BasicVector.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result vector.
 */
template<typename T1, typename T2, unsigned N>
inline BasicVector<decltype(T1{} - T2{}), N> operator-(const BasicVector<T1, N>& lhs, const BasicVector<T2, N>& rhs) noexcept
{
    BasicVector<decltype(T1{} - T2{}), N> res;

    for (unsigned i = 0; i < N; ++i)
        res[i] = lhs[i] - rhs[i];

    return res;
}

/* ************************************************************************ */

/**
 * @brief Substract operator.
 *
 * @tparam T1 Type of value in first BasicVector.
 * @tparam T2 Type of value in second BasicVector.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result vector.
 */
template<typename T1, typename T2>
inline BasicVector<decltype(T1{} - T2{}), 2> operator-(const BasicVector<T1, 2>& lhs, const BasicVector<T2, 2>& rhs) noexcept
{
    return BasicVector<decltype(T1{} - T2{}), 2>{
        lhs.getX() - rhs.getX(),
        lhs.getY() - rhs.getY()
    };
}

/* ************************************************************************ */

/**
 * @brief Substract operator.
 *
 * @tparam T1 Type of value in first BasicVector.
 * @tparam T2 Type of second operand.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result vector.
 */
template<typename T1, typename T2, unsigned N>
inline BasicVector<decltype(T1{} - T2{}), N> operator-(const BasicVector<T1, N>& lhs, T2 rhs) noexcept
{
    BasicVector<decltype(T1{} - T2{}), N> res;

    for (unsigned i = 0; i < N; ++i)
        res[i] = lhs[i] - rhs;

    return res;
}

/* ************************************************************************ */

/**
 * @brief Substract operator.
 *
 * @tparam T1 Type of value in first BasicVector.
 * @tparam T2 Type of second operand.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result vector.
 */
template<typename T1, typename T2>
inline BasicVector<decltype(T1{} - T2{}), 2> operator-(const BasicVector<T1, 2>& lhs, T2 rhs) noexcept
{
    return BasicVector<decltype(T1{} - T2{}), 2>{
        lhs.getX() - rhs,
        lhs.getY() - rhs
    };
}

/* ************************************************************************ */

/**
 * @brief Substract operator.
 *
 * @tparam T1 Type of first operand.
 * @tparam T2 Type of value in second BasicVector.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result vector.
 */
template<typename T1, typename T2, unsigned N>
inline BasicVector<decltype(T1{} - T2{}), N> operator-(T1 lhs, const BasicVector<T2, N>& rhs) noexcept
{
    BasicVector<decltype(T1{} - T2{}), N> res;

    for (unsigned i = 0; i < N; ++i)
        res[i] = lhs - rhs[i];

    return res;
}

/* ************************************************************************ */

/**
 * @brief Substract operator.
 *
 * @tparam T1 Type of first operand.
 * @tparam T2 Type of value in second BasicVector.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result vector.
 */
template<typename T1, typename T2>
inline BasicVector<decltype(T1{} - T2{}), 2> operator-(T1 lhs, const BasicVector<T2, 2>& rhs) noexcept
{
    return BasicVector<decltype(T1{} - T2{}), 2>{
        lhs - rhs.getX(),
        lhs - rhs.getY()
    };
}

/* ************************************************************************ */

/**
 * @brief Multiplication operator.
 *
 * @tparam T1 Type of value in first BasicVector.
 * @tparam T2 Type of value in second BasicVector.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result vector.
 */
template<typename T1, typename T2, unsigned N>
inline BasicVector<decltype(T1{} * T2{}), N> operator*(const BasicVector<T1, N>& lhs, const BasicVector<T2, N>& rhs) noexcept
{
    BasicVector<decltype(T1{} * T2{}), N> res;

    for (unsigned i = 0; i < N; ++i)
        res[i] = lhs[i] * rhs[i];

    return res;
}

/* ************************************************************************ */

/**
 * @brief Multiplication operator.
 *
 * @tparam T1 Type of value in first BasicVector.
 * @tparam T2 Type of value in second BasicVector.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result vector.
 */
template<typename T1, typename T2>
inline BasicVector<decltype(T1{} * T2{}), 2> operator*(const BasicVector<T1, 2>& lhs, const BasicVector<T2, 2>& rhs) noexcept
{
    return BasicVector<decltype(T1{} * T2{}), 2>{
        lhs.getX() * rhs.getX(),
        lhs.getY() * rhs.getY()
    };
}

/* ************************************************************************ */

/**
 * @brief Multiplication operator.
 *
 * @tparam T1 Type of value in first BasicVector.
 * @tparam T2 Type of second operand.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result vector.
 */
template<typename T1, typename T2, unsigned N>
inline BasicVector<decltype(T1{} * T2{}), N> operator*(const BasicVector<T1, N>& lhs, T2 rhs) noexcept
{
    BasicVector<decltype(T1{} * T2{}), N> res;

    for (unsigned i = 0; i < N; ++i)
        res[i] = lhs[i] * rhs;

    return res;
}

/* ************************************************************************ */

/**
 * @brief Multiplication operator.
 *
 * @tparam T1 Type of value in first BasicVector.
 * @tparam T2 Type of second operand.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result vector.
 */
template<typename T1, typename T2>
inline BasicVector<decltype(T1{} * T2{}), 2> operator*(const BasicVector<T1, 2>& lhs, T2 rhs) noexcept
{
    return BasicVector<decltype(T1{} * T2{}), 2>{
        lhs.getX() * rhs,
        lhs.getY() * rhs
    };
}

/* ************************************************************************ */

/**
 * @brief Multiplication operator.
 *
 * @tparam T1 Type of first operand.
 * @tparam T2 Type of value in second BasicVector.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result vector.
 */
template<typename T1, typename T2, unsigned N>
inline BasicVector<decltype(T1{} * T2{}), N> operator*(T1 lhs, const BasicVector<T2, N>& rhs) noexcept
{
    BasicVector<decltype(T1{} * T2{}), N> res;

    for (unsigned i = 0; i < N; ++i)
        res[i] = lhs * rhs[i];

    return res;
}

/* ************************************************************************ */

/**
 * @brief Multiplication operator.
 *
 * @tparam T1 Type of first operand.
 * @tparam T2 Type of value in second BasicVector.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result vector.
 */
template<typename T1, typename T2>
inline BasicVector<decltype(T1{} * T2{}), 2> operator*(T1 lhs, const BasicVector<T2, 2>& rhs) noexcept
{
    return BasicVector<decltype(T1{} * T2{}), 2>{
        lhs * rhs.getX(),
        lhs * rhs.getY()
    };
}

/* ************************************************************************ */

/**
 * @brief Division operator.
 *
 * @tparam T1 Type of value in first BasicVector.
 * @tparam T2 Type of value in second BasicVector.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result vector.
 */
template<typename T1, typename T2, unsigned N>
inline BasicVector<decltype(T1{} / T2{}), N> operator/(const BasicVector<T1, N>& lhs, const BasicVector<T2, N>& rhs) noexcept
{
    BasicVector<decltype(T1{} / T2{}), N> res;

    for (unsigned i = 0; i < N; ++i)
        res[i] = lhs[i] / rhs[i];

    return res;
}

/* ************************************************************************ */

/**
 * @brief Division operator.
 *
 * @tparam T1 Type of value in first BasicVector.
 * @tparam T2 Type of value in second BasicVector.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result vector.
 */
template<typename T1, typename T2>
inline BasicVector<decltype(T1{} / T2{}), 2> operator/(const BasicVector<T1, 2>& lhs, const BasicVector<T2, 2>& rhs) noexcept
{
    return BasicVector<decltype(T1{} / T2{}), 2>{
        lhs.getX() / rhs.getX(),
        lhs.getY() / rhs.getY()
    };
}

/* ************************************************************************ */

/**
 * @brief Division operator.
 *
 * @tparam T1 Type of value in first BasicVector.
 * @tparam T2 Type of second operand.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result vector.
 */
template<typename T1, typename T2, unsigned N>
inline BasicVector<decltype(T1{} / T2{}), N> operator/(const BasicVector<T1, N>& lhs, T2 rhs) noexcept
{
    BasicVector<decltype(T1{} / T2{}), N> res;

    for (unsigned i = 0; i < N; ++i)
        res[i] = lhs[i] / rhs;

    return res;
}

/* ************************************************************************ */

/**
 * @brief Division operator.
 *
 * @tparam T1 Type of value in first BasicVector.
 * @tparam T2 Type of second operand.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result vector.
 */
template<typename T1, typename T2>
inline BasicVector<decltype(T1{} / T2{}), 2> operator/(const BasicVector<T1, 2>& lhs, T2 rhs) noexcept
{
    return BasicVector<decltype(T1{} / T2{}), 2>{
        lhs.getX() / rhs,
        lhs.getY() / rhs
    };
}

/* ************************************************************************ */

/**
 * @brief Division operator.
 *
 * @tparam T1 Type of first operand.
 * @tparam T2 Type of value in second BasicVector.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result vector.
 */
template<typename T1, typename T2, unsigned N>
inline BasicVector<decltype(T1{} / T2{}), N> operator/(T1 lhs, const BasicVector<T2, N>& rhs) noexcept
{
    BasicVector<decltype(T1{} / T2{}), N> res;

    for (unsigned i = 0; i < N; ++i)
        res[i] = lhs / rhs[i];

    return res;
}

/* ************************************************************************ */

/**
 * @brief Division operator.
 *
 * @tparam T1 Type of first operand.
 * @tparam T2 Type of value in second BasicVector.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result vector.
 */
template<typename T1, typename T2>
inline BasicVector<decltype(T1{} / T2{}), 2> operator/(T1 lhs, const BasicVector<T2, 2>& rhs) noexcept
{
    return BasicVector<decltype(T1{} / T2{}), 2>{
        lhs / rhs.getX(),
        lhs / rhs.getY()
    };
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
inline bool operator==(const BasicVector<T1, N>& lhs, const BasicVector<T2, N>& rhs) noexcept
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
template<typename T1, typename T2>
inline bool operator==(const BasicVector<T1, 2>& lhs, const BasicVector<T2, 2>& rhs) noexcept
{
    return
        lhs.getX() == rhs.getX() &&
        lhs.getY() == rhs.getY()
    ;
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
inline bool operator==(const BasicVector<T1, N>& lhs, Zero_t rhs) noexcept
{
    return lhs == BasicVector<T1, N>(Zero);
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
inline bool operator==(Zero_t lhs, const BasicVector<T1, N>& rhs) noexcept
{
    return BasicVector<T1, N>(Zero) == rhs;
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
inline bool operator!=(const BasicVector<T1, N>& lhs, const BasicVector<T2, N>& rhs) noexcept
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
inline bool operator!=(const BasicVector<T1, N>& lhs, Zero_t rhs) noexcept
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
inline bool operator!=(Zero_t lhs, const BasicVector<T1, N>& rhs) noexcept
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
inline bool operator<(const BasicVector<T1, N>& lhs, const BasicVector<T2, N>& rhs) noexcept
{
    bool res = true;

    for (unsigned i = 0; i < N; ++i)
        res = res && ((lhs[i] < rhs[i]) || !(rhs[i] < lhs[i]));

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
inline bool operator<(const BasicVector<T1, N>& lhs, Zero_t rhs) noexcept
{
    return lhs < BasicVector<T1, N>{Zero};
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
inline bool operator<(Zero_t lhs, const BasicVector<T1, N>& rhs) noexcept
{
    return BasicVector<T1, N>{Zero} < rhs;
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
inline bool operator<=(const BasicVector<T1, N>& lhs, const BasicVector<T2, N>& rhs) noexcept
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
inline bool operator<=(const BasicVector<T1, N>& lhs, Zero_t rhs) noexcept
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
inline bool operator<=(Zero_t lhs, const BasicVector<T1, N>& rhs) noexcept
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
inline bool operator>(const BasicVector<T1, N>& lhs, const BasicVector<T2, N>& rhs) noexcept
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
inline bool operator>(const BasicVector<T1, N>& lhs, Zero_t rhs) noexcept
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
inline bool operator>(Zero_t lhs, const BasicVector<T1, N>& rhs) noexcept
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
inline bool operator>=(const BasicVector<T1, N>& lhs, const BasicVector<T2, N>& rhs) noexcept
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
inline bool operator>=(const BasicVector<T1, N>& lhs, Zero_t rhs) noexcept
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
inline bool operator>=(Zero_t lhs, const BasicVector<T1, N>& rhs) noexcept
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
inline decltype(T1{} * T2{}) dot(const BasicVector<T1, N>& lhs, const BasicVector<T2, N>& rhs) noexcept
{
    decltype(T1{} * T2{}) res{};

    for (unsigned i = 0; i < N; ++i)
        res += lhs[i] * rhs[i];

    return res;
}

/* ************************************************************************ */

/**
 * @brief Calculate cross product of two vectors.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Cross product.
 */
template<typename T1, typename T2>
inline
decltype(std::declval<T1>() * std::declval<T2>())
cross(const BasicVector<T1, 2>& lhs, const BasicVector<T2, 2>& rhs) noexcept
{
    return {lhs.getX() * rhs.getY() - lhs.getY() * rhs.getX()};
}

/* ************************************************************************ */

/**
 * @brief Calculate cross product of two vectors.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Cross product.
 */
template<typename T1, typename T2>
inline
BasicVector<decltype(std::declval<T1>() * std::declval<T2>()), 2>
cross(const BasicVector<T1, 2>& lhs, const T2& rhs) noexcept
{
    return {rhs * lhs.getY(), -rhs * lhs.getX()};
}

/* ************************************************************************ */

/**
 * @brief Calculate cross product of two vectors.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Cross product.
 */
template<typename T1, typename T2>
inline
BasicVector<decltype(std::declval<T1>() * std::declval<T2>()), 2>
cross(const T1& lhs, const BasicVector<T2, 2>& rhs) noexcept
{
    return {-lhs * rhs.getY(), lhs * rhs.getX()};
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
InStream& operator>>(InStream& is, BasicVector<T, N>& vector)
{
    unsigned i = 0;

    for (; i < N; ++i)
    {
        if (!(is >> std::skipws >> vector[i]))
            break;
    }

    if (i == 0)
        return is;

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
OutStream& operator<<(OutStream& os, const BasicVector<T, N>& vector) noexcept
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
}

/* ************************************************************************ */

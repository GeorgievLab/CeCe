
#pragma once

/* ************************************************************************ */

// C++
#include <cmath>
#include <type_traits>

// Simulator
#include "core/Units.hpp"

/* ************************************************************************ */

/**
 * @brief 2D vector.
 */
template<typename T>
struct Vector
{

// Public Types
public:


    /**
     * @brief Common type helper.
     */
    template<typename T2>
    using CT = typename std::common_type<T, T2>::type;


// Public Data Members
public:


    union
    {
        T x;
        T width;
    };

    union
    {
        T y;
        T height;
    };


// Public Ctors
public:


    /**
     * @brief Default constructor.
     */
    constexpr Vector() = default;


    /**
     * @brief Constructor.
     *
     * @param x
     * @param y
     */
    constexpr Vector(T x, T y) noexcept
        : x(x), y(y)
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
        : x(v.x), y(v.y)
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
    Vector& operator=(const Vector& v) noexcept
    {
        x = v.x;
        y = v.y;
        return *this;
    }


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
        x = v.x;
        y = v.y;
        return *this;
    }


    /**
     * @brief Add operator.
     *
     * @param v
     *
     * @return
     */
    constexpr Vector operator+(const Vector& v) const noexcept
    {
        return Vector{x + v.x, y + v.y};
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
        return Vector<CT<T2>>{x + v.x, y + v.y};
    }


    /**
     * @brief Add operator.
     *
     * @param v
     *
     * @return
     */
    Vector& operator+=(const Vector& v) noexcept
    {
        x += v.x;
        y += v.y;
        return *this;
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
        x += v.x;
        y += v.y;
        return *this;
    }


    /**
     * @brief Add operator.
     *
     * @param v
     *
     * @return
     */
    constexpr Vector operator+(const T& val) const noexcept
    {
        return Vector{x + val, y + val};
    }


    /**
     * @brief Add operator.
     *
     * @param v
     *
     * @return
     */
    template<typename T2>
    constexpr Vector<CT<T2>> operator+(const T2& val) const noexcept
    {
        return Vector<CT<T2>>{x + val, y + val};
    }


    /**
     * @brief Add operator.
     *
     * @param v
     *
     * @return
     */
    Vector& operator+=(const T& val) noexcept
    {
        x += val;
        y += val;
        return *this;
    }


    /**
     * @brief Add operator.
     *
     * @param v
     *
     * @return
     */
    template<typename T2>
    Vector& operator+=(const T2& val) noexcept
    {
        x += val;
        y += val;
        return *this;
    }


    /**
     * @brief Substract operator.
     *
     * @param v
     *
     * @return
     */
    constexpr Vector operator-(const Vector& v) const noexcept
    {
        return Vector{x - v.x, y - v.y};
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
        return Vector<CT<T2>>{x - v.x, y - v.y};
    }


    /**
     * @brief Substract operator.
     *
     * @param v
     *
     * @return
     */
    Vector& operator-=(const Vector& v) noexcept
    {
        x -= v.x;
        y -= v.y;
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
    Vector& operator-=(const Vector<T2>& v) noexcept
    {
        x -= v.x;
        y -= v.y;
        return *this;
    }


    /**
     * @brief Substract operator.
     *
     * @param v
     *
     * @return
     */
    constexpr Vector operator-(const T& val) const noexcept
    {
        return Vector{x - val, y - val};
    }


    /**
     * @brief Substract operator.
     *
     * @param v
     *
     * @return
     */
    template<typename T2>
    constexpr Vector<CT<T2>> operator-(const T2& val) const noexcept
    {
        return Vector<CT<T2>>{x - val, y - val};
    }


    /**
     * @brief Substract operator.
     *
     * @param v
     *
     * @return
     */
    Vector& operator-=(const T& val) noexcept
    {
        x -= val;
        y -= val;
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
    Vector& operator-=(const T2& val) noexcept
    {
        x -= val;
        y -= val;
        return *this;
    }


    /**
     * @brief Multiple operator.
     *
     * @param v
     *
     * @return
     */
    constexpr Vector operator*(T val) const noexcept
    {
        return Vector{x * val, y * val};
    }


    /**
     * @brief Multiple operator.
     *
     * @param v
     *
     * @return
     */
    template<typename T2>
    constexpr Vector<CT<T2>> operator*(T2 val) const noexcept
    {
        return Vector<CT<T2>>{x * val, y * val};
    }


    /**
     * @brief Multiple operator.
     *
     * @param v
     *
     * @return
     */
    Vector& operator*=(T val) noexcept
    {
        x *= val;
        y *= val;
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
    Vector& operator*=(T2 val) noexcept
    {
        x *= val;
        y *= val;
        return *this;
    }


    /**
     * @brief Multiple operator.
     *
     * @param v
     *
     * @return
     */
    constexpr Vector operator*(const Vector& val) const noexcept
    {
        return Vector{x * val.x, y * val.y};
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
        return Vector<CT<T2>>{x * val.x, y * val.y};
    }


    /**
     * @brief Multiple operator.
     *
     * @param v
     *
     * @return
     */
    Vector& operator*=(const Vector& val) noexcept
    {
        x *= val.x;
        y *= val.y;
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
    Vector& operator*=(const Vector<T2>& val) noexcept
    {
        x *= val.x;
        y *= val.y;
        return *this;
    }


    /**
     * @brief Divide operator.
     *
     * @param v
     *
     * @return
     */
    constexpr Vector operator/(T val) const noexcept
    {
        return Vector{x / val, y / val};
    }


    /**
     * @brief Divide operator.
     *
     * @param v
     *
     * @return
     */
    template<typename T2>
    constexpr Vector<CT<T2>> operator/(T2 val) const noexcept
    {
        return Vector<CT<T2>>{x / val, y / val};
    }


    /**
     * @brief Divide operator.
     *
     * @param v
     *
     * @return
     */
    Vector& operator/=(T val) noexcept
    {
        x /= val;
        y /= val;
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
    Vector& operator/=(T2 val) noexcept
    {
        x /= val;
        y /= val;
        return *this;
    }


    /**
     * @brief Divide operator.
     *
     * @param v
     *
     * @return
     */
    constexpr Vector operator/(const Vector& val) const noexcept
    {
        return Vector{x / val.x, y / val.y};
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
        return Vector<CT<T2>>{x / val.x, y / val.y};
    }


    /**
     * @brief Divide operator.
     *
     * @param v
     *
     * @return
     */
    Vector& operator/=(const Vector& val) noexcept
    {
        x /= val.x;
        y /= val.y;
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
    Vector& operator/=(const Vector<T2>& val) noexcept
    {
        x /= val.x;
        y /= val.y;
        return *this;
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
        return x * x + y * y;
    }


    /**
     * @brief Rotate current vector.
     *
     * @param angle
     *
     * @return
     */
    Vector rotate(units::Angle angle) const noexcept
    {
        return Vector(
            x * cos(angle) - y * sin(angle),
            x * sin(angle) + y * cos(angle)
        );
    }

};

/* ************************************************************************ */


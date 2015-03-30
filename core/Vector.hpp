
#pragma once

/* ************************************************************************ */

// C++
#include <cmath>

// Simulator
#include "core/Units.hpp"

/* ************************************************************************ */

/**
 * @brief 2D vector.
 */
template<typename T>
struct Vector
{

// Public Data Members
public:


    T x;
    T y;


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
    Vector& operator+=(const T& val) noexcept
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
    Vector& operator-=(const T& val) noexcept
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
    constexpr Vector operator*(Vector val) const noexcept
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
    Vector& operator*=(Vector val) noexcept
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
    constexpr Vector operator/(Vector val) const noexcept
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
    Vector& operator/=(Vector val) noexcept
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


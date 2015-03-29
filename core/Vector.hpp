
#pragma once

/* ************************************************************************ */

// C++
#include <cmath>

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

};

/* ************************************************************************ */


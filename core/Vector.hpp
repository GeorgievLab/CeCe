
#pragma once

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
     * @brief Add operator.
     *
     * @param v
     *
     * @return
     */
    Vector operator+(const Vector& v) const noexcept
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
     * @brief Substract operator.
     *
     * @param v
     *
     * @return
     */
    Vector operator-(const Vector& v) const noexcept
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
     * @brief Multiple operator.
     *
     * @param v
     *
     * @return
     */
    Vector operator*(T val) const noexcept
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
     * @brief Divide operator.
     *
     * @param v
     *
     * @return
     */
    Vector operator/(T val) const noexcept
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


// Public Operations
public:

};

/* ************************************************************************ */


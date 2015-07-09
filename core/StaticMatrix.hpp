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
#include <cstring>

// Simulator
#include "core/Units.hpp"
#include "core/Vector.hpp"

/* ************************************************************************ */

#ifndef _MSC_VER
inline namespace core {
#endif

/* ************************************************************************ */

/**
 * @brief 2D matrix of fixed size.
 *
 * @tparam T      Type of stored value.
 * @tparam Width  Matrix width.
 * @tparam Height Matrix height.
 */
template<typename T, unsigned Width, unsigned Height = Width>
struct StaticMatrix
{

// Public Constants
public:


    /**
     * @brief Matrix width.
     */
    static CONSTEXPR_CONST auto WIDTH = Width;


    /**
     * @brief Matrix height.
     */
    static CONSTEXPR_CONST auto HEIGHT = Height;


// Public Ctors
public:


    /**
     * @brief Default constructor.
     */
    CONSTEXPR StaticMatrix() = default;


    /**
     * @brief Constructor.
     *
     * @param src Initial data
     */
    StaticMatrix(const T (&src)[Width][Height]) NOEXCEPT
    {
        for (decltype(Width) i = 0; i < WIDTH; ++i)
            for (decltype(Height) j = 0; j < HEIGHT; ++j)
                m_data[i][j] = src[i][j];
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
    StaticMatrix& operator=(const StaticMatrix& v) NOEXCEPT
    {
        std::memcpy(m_data, v.m_data, sizeof(m_data));
        return *this;
    }


    /**
     * @brief Access operator.
     *
     * @param i
     *
     * @return
     */
    T* operator[](decltype(Height) i) NOEXCEPT
    {
        return m_data[i];
    }


    /**
     * @brief Access operator.
     *
     * @param i
     *
     * @return
     */
    const T* operator[](decltype(Height) i) const NOEXCEPT
    {
        return m_data[i];
    }


    /**
     * @brief Access operator.
     *
     * @param coord Coordinates.
     *
     * @return
     */
    T& operator[](Vector<decltype(Height)> coord) NOEXCEPT
    {
        return m_data[coord.getY()][coord.getX()];
    }


    /**
     * @brief Access operator.
     *
     * @param coord Coordinates.
     *
     * @return
     */
    const T& operator[](Vector<decltype(Height)> coord) const NOEXCEPT
    {
        return m_data[coord.getY()][coord.getX()];
    }


    /**
     * @brief Addition operator.
     *
     * @param rhs Right operand.
     *
     * @return
     */
    StaticMatrix& operator+=(const StaticMatrix& rhs) NOEXCEPT
    {
        for (decltype(Width) i = 0; i < WIDTH; ++i)
            for (decltype(Height) j = 0; j < HEIGHT; ++j)
                m_data[i][j] += rhs.m_data[i][j];

        return *this;
    }


    /**
     * @brief Substraction operator.
     *
     * @param rhs Right operand.
     *
     * @return
     */
    StaticMatrix& operator-=(const StaticMatrix& rhs) NOEXCEPT
    {
        for (decltype(Width) i = 0; i < WIDTH; ++i)
            for (decltype(Height) j = 0; j < HEIGHT; ++j)
                m_data[i][j] -= rhs.m_data[i][j];

        return *this;
    }


    /**
     * @brief Multiplication operator.
     *
     * @param rhs Right operand.
     *
     * @return
     */
    StaticMatrix& operator*=(T rhs) NOEXCEPT
    {
        for (decltype(Width) i = 0; i < WIDTH; ++i)
            for (decltype(Height) j = 0; j < HEIGHT; ++j)
                m_data[i][j] *= rhs;

        return *this;
    }


    /**
     * @brief Division operator.
     *
     * @param rhs Right operand.
     *
     * @return
     */
    StaticMatrix& operator/=(T rhs) NOEXCEPT
    {
        for (decltype(Width) i = 0; i < WIDTH; ++i)
            for (decltype(Height) j = 0; j < HEIGHT; ++j)
                m_data[i][j] /= rhs;

        return *this;
    }


// Public Operations
public:


    /**
     * @brief Sum all values.
     *
     * @return
     */
    T sum() const NOEXCEPT
    {
        T res{};

        for (decltype(Width) i = 0; i < WIDTH; ++i)
            for (decltype(Height) j = 0; j < HEIGHT; ++j)
                res += m_data[i][j];

        return res;
    }


    /**
     * @brief Normalize matrix values.
     *
     * @return
     */
    StaticMatrix& normalize() NOEXCEPT
    {
        *this /= sum();
        return *this;
    }


    /**
     * @brief Create normalized matrix.
     *
     * @return
     */
    StaticMatrix<decltype(T{} / T{}), Width, Height> normalized() NOEXCEPT
    {
        return *this / sum();
    }


    /**
     * @brief Compute matrix values.
     *
     * @return
     */
    template<typename F>
    static StaticMatrix generate(F fn) NOEXCEPT
    {
        StaticMatrix res;

        for (decltype(Width) i = 0; i < WIDTH; ++i)
            for (decltype(Height) j = 0; j < HEIGHT; ++j)
                res.m_data[i][j] = fn(i, j);

        return res;
    }


    /**
     * @brief Create distance matrix.
     *
     * @return
     */
    static StaticMatrix<Vector<T>, Width, Height> makeDistances() NOEXCEPT
    {
        static_assert(Width % 2 == 1, "Width must be odd");
        static_assert(Height % 2 == 1, "Height must be odd");
        static CONSTEXPR_CONST T HALF_X = Width / T(2);
        static CONSTEXPR_CONST T HALF_Y = Height / T(2);

        StaticMatrix<Vector<T>, Width, Height> res;

        for (decltype(Width) i = 0; i < WIDTH; ++i)
            for (decltype(Height) j = 0; j < HEIGHT; ++j)
                res[i][j] = Vector<T>{T(i) - HALF_X, T(j) - HALF_Y};

        return res;
    }


// Private Data Members
private:

    /// Matrix data.
    T m_data[Width][Height];

};

/* ************************************************************************ */

/**
 * @brief Addition operator.
 *
 * @tparam T1 Type of value in first matrix.
 * @tparam T2 Type of value in second matrix.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result matrix.
 */
template<typename T1, typename T2, unsigned Width, unsigned Height>
inline StaticMatrix<decltype(T1{} + T2{}), Width, Height> operator+(
    StaticMatrix<T1, Width, Height> lhs,
    const StaticMatrix<T2, Width, Height>& rhs
) NOEXCEPT
{
    return lhs += rhs;
}

/* ************************************************************************ */

/**
 * @brief Substraction operator.
 *
 * @tparam T1 Type of value in first matrix.
 * @tparam T2 Type of value in second matrix.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result matrix.
 */
template<typename T1, typename T2, unsigned Width, unsigned Height>
inline StaticMatrix<decltype(T1{} - T2{}), Width, Height> operator-(
    StaticMatrix<T1, Width, Height> lhs,
    const StaticMatrix<T2, Width, Height>& rhs
) NOEXCEPT
{
    return lhs -= rhs;
}

/* ************************************************************************ */

/**
 * @brief Multiplication operator.
 *
 * @tparam T1 Type of value in first matrix.
 * @tparam T2 Type of value in second matrix.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result matrix.
 */
template<typename T1, typename T2, unsigned Width, unsigned Height>
inline StaticMatrix<decltype(T1{} * T2{}), Width, Height> operator*(
    StaticMatrix<T1, Width, Height> lhs, T2 rhs
) NOEXCEPT
{
    StaticMatrix<decltype(T1{} / T2{}), Width, Height> res;

    for (decltype(Width) i = 0; i < Width; ++i)
        for (decltype(Height) j = 0; j < Height; ++j)
            res[i][j] = lhs[i][j] * rhs;

    return res;
}

/* ************************************************************************ */

/**
 * @brief Multiplication operator.
 *
 * @tparam T1 Type of value in first matrix.
 * @tparam T2 Type of value in second matrix.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result matrix.
 */
template<typename T1, typename T2, unsigned Width, unsigned Height>
inline StaticMatrix<decltype(T1{} / T2{}), Width, Height> operator/(
    StaticMatrix<T1, Width, Height> lhs, T2 rhs
) NOEXCEPT
{
    StaticMatrix<decltype(T1{} / T2{}), Width, Height> res;

    for (decltype(Width) i = 0; i < Width; ++i)
        for (decltype(Height) j = 0; j < Height; ++j)
            res[i][j] = lhs[i][j] / rhs;

    return res;
}

/* ************************************************************************ */

#ifndef _MSC_VER
}
#endif

/* ************************************************************************ */


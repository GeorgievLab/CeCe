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

// C++
#include <cmath>
#include <cstring>

// CeCe
#include "cece/core/Units.hpp"
#include "cece/core/Vector.hpp"

/* ************************************************************************ */

namespace cece {
inline namespace core {

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
    static constexpr auto WIDTH = Width;


    /**
     * @brief Matrix height.
     */
    static constexpr auto HEIGHT = Height;


// Public Ctors
public:


    /**
     * @brief Default constructor.
     */
    constexpr StaticMatrix() = default;


    /**
     * @brief Constructor.
     *
     * @param src Initial data
     */
    StaticMatrix(const T (&src)[Width][Height]) noexcept
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
    StaticMatrix& operator=(const StaticMatrix& v) noexcept
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
    T* operator[](decltype(Height) i) noexcept
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
    const T* operator[](decltype(Height) i) const noexcept
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
    T& operator[](Vector<decltype(Height)> coord) noexcept
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
    const T& operator[](Vector<decltype(Height)> coord) const noexcept
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
    StaticMatrix& operator+=(const StaticMatrix& rhs) noexcept
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
    StaticMatrix& operator-=(const StaticMatrix& rhs) noexcept
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
    StaticMatrix& operator*=(T rhs) noexcept
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
    StaticMatrix& operator/=(T rhs) noexcept
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
    T sum() const noexcept
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
    StaticMatrix& normalize() noexcept
    {
        *this /= sum();
        return *this;
    }


    /**
     * @brief Create normalized matrix.
     *
     * @return
     */
    StaticMatrix<decltype(T{} / T{}), Width, Height> normalized() noexcept
    {
        return *this / sum();
    }


    /**
     * @brief Compute matrix values.
     *
     * @return
     */
    template<typename F>
    static StaticMatrix generate(F fn) noexcept
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
    static StaticMatrix<Vector<T>, Width, Height> makeDistances() noexcept
    {
        static_assert(Width % 2 == 1, "Width must be odd");
        static_assert(Height % 2 == 1, "Height must be odd");
        static constexpr T HALF_X = Width / T(2);
        static constexpr T HALF_Y = Height / T(2);

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
) noexcept
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
) noexcept
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
) noexcept
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
) noexcept
{
    StaticMatrix<decltype(T1{} / T2{}), Width, Height> res;

    for (decltype(Width) i = 0; i < Width; ++i)
        for (decltype(Height) j = 0; j < Height; ++j)
            res[i][j] = lhs[i][j] / rhs;

    return res;
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */



#pragma once

/* ************************************************************************ */

// C++
#include <cmath>
#include <cstring>

// Simulator
#include "core/Units.hpp"
#include "core/Vector.hpp"

/* ************************************************************************ */

namespace core {

/* ************************************************************************ */

/**
 * @brief 2D matrix of fixed size.
 */
template<typename T, unsigned SIZE>
struct Matrix
{

// Public Data Members
public:


    /// Data
    T data[SIZE][SIZE];


// Public Ctors
public:


    /**
     * @brief Default constructor.
     */
    CONSTEXPR Matrix() = default;


    /**
     * @brief Constructor.
     *
     * @param src Initial data
     */
    Matrix(const T (&src)[SIZE][SIZE]) NOEXCEPT
    {
        for (size_t i = 0; i < SIZE; ++i)
            for (size_t j = 0; j < SIZE; ++j)
                data[i][j] = src[i][j];
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
    Matrix& operator=(const Matrix& v) NOEXCEPT
    {
        std::memcpy(data, v.data, sizeof(data));
        return *this;
    }


    /**
     * @brief Access operator.
     *
     * @param i
     *
     * @return
     */
	T* operator[](size_t i) NOEXCEPT
    {
        return data[i];
    }


    /**
     * @brief Access operator.
     *
     * @param i
     *
     * @return
     */
	const T* operator[](size_t i) const NOEXCEPT
    {
        return data[i];
    }


    /**
     * @brief Add operator.
     *
     * @param v
     *
     * @return
     */
    Matrix operator+(const Matrix& v) const NOEXCEPT
    {
        decltype(data) tmp;

        for (size_t i = 0; i < SIZE; ++i)
            for (size_t j = 0; j < SIZE; ++j)
                tmp[i][j] = data[i][j] + v.data[i][j];

        return Matrix{tmp};
    }


    /**
     * @brief Add operator.
     *
     * @param v
     *
     * @return
     */
    Matrix& operator+=(const Matrix& v) NOEXCEPT
    {
        for (size_t i = 0; i < SIZE; ++i)
            for (size_t j = 0; j < SIZE; ++j)
                data[i][j] += v.data[i][j];

        return *this;
    }


    /**
     * @brief Add operator.
     *
     * @param v
     *
     * @return
     */
    Matrix operator+(const T& val) const NOEXCEPT
    {
        decltype(data) tmp;

        for (size_t i = 0; i < SIZE; ++i)
            for (size_t j = 0; j < SIZE; ++j)
                tmp[i][j] = data[i][j] + val;

        return Matrix{tmp};
    }


    /**
     * @brief Add operator.
     *
     * @param v
     *
     * @return
     */
    Matrix& operator+=(const T& val) NOEXCEPT
    {
        for (size_t i = 0; i < SIZE; ++i)
            for (size_t j = 0; j < SIZE; ++j)
                data[i][j] += val;

        return *this;
    }


    /**
     * @brief Substract operator.
     *
     * @param v
     *
     * @return
     */
    Matrix operator-(const Matrix& v) const NOEXCEPT
    {
        decltype(data) tmp;

        for (size_t i = 0; i < SIZE; ++i)
            for (size_t j = 0; j < SIZE; ++j)
                tmp[i][j] = data[i][j] - v.data[i][j];

        return Matrix{tmp};
    }


    /**
     * @brief Substract operator.
     *
     * @param v
     *
     * @return
     */
    Matrix& operator-=(const Matrix& v) NOEXCEPT
    {
        for (size_t i = 0; i < SIZE; ++i)
            for (size_t j = 0; j < SIZE; ++j)
                data[i][j] -= v.data[i][j];

        return *this;
    }


    /**
     * @brief Substract operator.
     *
     * @param v
     *
     * @return
     */
    Matrix operator-(const T& val) const NOEXCEPT
    {
        decltype(data) tmp;

        for (size_t i = 0; i < SIZE; ++i)
            for (size_t j = 0; j < SIZE; ++j)
                tmp[i][j] = data[i][j] - val;

        return Matrix{tmp};
    }


    /**
     * @brief Substract operator.
     *
     * @param v
     *
     * @return
     */
    Matrix& operator-=(const T& val) NOEXCEPT
    {
        for (size_t i = 0; i < SIZE; ++i)
            for (size_t j = 0; j < SIZE; ++j)
                data[i][j] -= val;

        return *this;
    }


    /**
     * @brief Multiple operator.
     *
     * @param v
     *
     * @return
     */
    Matrix operator*(T val) const NOEXCEPT
    {
        decltype(data) tmp;

        for (size_t i = 0; i < SIZE; ++i)
            for (size_t j = 0; j < SIZE; ++j)
                tmp[i][j] = data[i][j] * val;

        return Matrix{tmp};
    }


    /**
     * @brief Multiple operator.
     *
     * @param v
     *
     * @return
     */
    Matrix& operator*=(T val) NOEXCEPT
    {
        for (size_t i = 0; i < SIZE; ++i)
            for (size_t j = 0; j < SIZE; ++j)
                data[i][j] *= val;

        return *this;
    }


    /**
     * @brief Divide operator.
     *
     * @param v
     *
     * @return
     */
    Matrix operator/(T val) const NOEXCEPT
    {
        decltype(data) tmp;

        for (size_t i = 0; i < SIZE; ++i)
            for (size_t j = 0; j < SIZE; ++j)
                tmp[i][j] = data[i][j] / val;

        return Matrix{tmp};
    }


    /**
     * @brief Divide operator.
     *
     * @param v
     *
     * @return
     */
    Matrix& operator/=(T val) NOEXCEPT
    {
        for (size_t i = 0; i < SIZE; ++i)
            for (size_t j = 0; j < SIZE; ++j)
                data[i][j] /= val;

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
        T val{};

        for (size_t i = 0; i < SIZE; ++i)
            for (size_t j = 0; j < SIZE; ++j)
                val += data[i][j];

        return val;
    }


    /**
     * @brief Normalize matrix values.
     *
     * @return
     */
    Matrix& normalize() NOEXCEPT
    {
        *this /= sum();
        return *this;
    }


    /**
     * @brief Create normalized matrix.
     *
     * @return
     */
    Matrix normalized() NOEXCEPT
    {
        return *this / sum();
    }


    /**
     * @brief Compute matrix values.
     *
     * @return
     */
    template<typename F>
    static Matrix generate(F fn) NOEXCEPT
    {
        Matrix val;

        for (size_t i = 0; i < SIZE; ++i)
            for (size_t j = 0; j < SIZE; ++j)
                val.data[i][j] = fn(i, j);

        return val;
    }


    /**
     * @brief Create distance matrix.
     *
     * @return
     */
    static Matrix<Vector<T>, SIZE> makeDistances() NOEXCEPT
    {
        static_assert(SIZE % 2 == 1, "Size must be odd");
        static CONSTEXPR T HALF = SIZE / T(2);

        Matrix<Vector<T>, SIZE> val;

        for (size_t i = 0; i < SIZE; ++i)
            for (size_t j = 0; j < SIZE; ++j)
                val.data[i][j] = Vector<T>{T(i) - HALF, T(j) - HALF};

        return val;
    }

};

/* ************************************************************************ */

}

/* ************************************************************************ */


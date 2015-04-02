
#pragma once

/* ************************************************************************ */

// C++
#include <cmath>
#include <cstring>

// Simulator
#include "core/Units.hpp"
#include "core/Vector.hpp"

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
    constexpr Matrix() = default;


    /**
     * @brief Constructor.
     *
     * @param src Initial data
     */
    Matrix(const T (&src)[SIZE][SIZE]) noexcept
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
    Matrix& operator=(const Matrix& v) noexcept
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
    T* operator[](unsigned i) noexcept
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
    const T* operator[](unsigned i) const noexcept
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
    Matrix operator+(const Matrix& v) const noexcept
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
    Matrix& operator+=(const Matrix& v) noexcept
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
    Matrix operator+(const T& val) const noexcept
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
    Matrix& operator+=(const T& val) noexcept
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
    Matrix operator-(const Matrix& v) const noexcept
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
    Matrix& operator-=(const Matrix& v) noexcept
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
    Matrix operator-(const T& val) const noexcept
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
    Matrix& operator-=(const T& val) noexcept
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
    Matrix operator*(T val) const noexcept
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
    Matrix& operator*=(T val) noexcept
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
    Matrix operator/(T val) const noexcept
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
    Matrix& operator/=(T val) noexcept
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
    T sum() const noexcept
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
    Matrix& normalize() noexcept
    {
        *this /= sum();
        return *this;
    }


    /**
     * @brief Create normalized matrix.
     *
     * @return
     */
    Matrix normalized() noexcept
    {
        return *this / sum();
    }


    /**
     * @brief Compute matrix values.
     *
     * @return
     */
    template<typename F>
    static Matrix generate(F fn) noexcept
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
    static Matrix<Vector<T>, SIZE> makeDistance() noexcept
    {
        static_assert(SIZE % 2 == 1, "Size must be odd");
        static constexpr T HALF = SIZE / T(2);

        Matrix<Vector<T>, SIZE> val;

        for (size_t i = 0; i < SIZE; ++i)
            for (size_t j = 0; j < SIZE; ++j)
                val.data[i][j] = Vector<T>{T(i) - HALF, T(j) - HALF};

        return val;
    }

};

/* ************************************************************************ */


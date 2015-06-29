
#pragma once

/* ************************************************************************ */

// C++
#include <array>

// Simulator
#include "core/compatibility.hpp"

#if ENABLE_SSE && __SSE__
#include <immintrin.h>
#endif

/* ************************************************************************ */

namespace plugin {
namespace diffusion {

/* ************************************************************************ */

/**
 * @brief Signal container - it can store multiple signals.
 *
 * @note If SSE is enabled multiple signals are computed at once.
 */
class Signal
{

// Public Constants
public:


    /**
     * @brief Number of stored signals
     */
#ifdef __AVX__
#ifdef _MSC_VER
	static const unsigned int COUNT = 8;
#else
    static CONSTEXPR unsigned int COUNT = 8;
#endif
#else
#ifdef _MSC_VER
	static const unsigned int COUNT = 4;
#else
    static constexpr unsigned int COUNT = 4;
#endif
#endif


    /**
     * @brief Minimum signal value to be ignored.
     */
#ifndef _MSC_VER
    static constexpr float IGNORE_LEVEL = 0.f;
#endif


// Public Ctors & Dtors
public:


    /**
     * @brief Default constructor.
     */
    Signal()
#if ENABLE_SSE && __SSE__
#ifdef __AVX__
        : m_sse(_mm256_setzero_ps())
#else
        : m_sse(_mm_setzero_ps())
#endif
#else
        : m_values{}
#endif
    {
        // Nothing to do
    }


    /**
     * @brief Constructor.
     *
     * @param value
     */
    explicit Signal(float value)
#if ENABLE_SSE && __SSE__
#ifdef __AVX__
        : m_sse(_mm256_set1_ps(value))
#else
        : m_sse(_mm_set1_ps(value))
#endif
#elif !defined(_MSC_VER)
		: m_values{ value, value, value, value }
#endif
    {
#ifdef _MSC_VER
		m_values[0] = value;
		m_values[1] = value;
		m_values[2] = value;
		m_values[3] = value;
#endif
    }


    /**
     * @brief Constructor.
     *
     * @param src
     */
    Signal(std::array<float, COUNT> src)
        : m_values(std::move(src))
    {
        // Nothing to do
    }


#if ENABLE_SSE && __SSE__
    /**
     * @brief Constructor.
     *
     * @param values
     */
#ifdef __AVX__
    CONSTEXPR Signal(__m256 values)
#else
    CONSTEXPR Signal(__m128 values)
#endif
        : m_sse(values)
    {
        // Nothing to do
    }
#endif


// Public Operators
public:


    /**
     * @brief Check if values are beyond some level.
     */
    explicit operator bool() const NOEXCEPT
    {
#if ENABLE_SSE && __SSE__
#ifdef __AVX__
        return _mm256_movemask_ps(_mm256_cmp_ps(m_sse, _mm256_set1_ps(IGNORE_LEVEL), _CMP_GT_OS));
#else
#if _MSC_VER
		return _mm_movemask_ps(_mm_cmpgt_ps(m_sse, _mm_set1_ps(0.f)));
#else
        return _mm_movemask_ps(_mm_cmpgt_ps(m_sse, _mm_set1_ps(IGNORE_LEVEL)));
#endif
#endif
#else
        for (float v : m_values)
#if _MSC_VER
            if (v > 0.f)
#else
			if (v > IGNORE_LEVEL)
#endif
                return true;

        return false;
#endif
    }


    /**
     * @brief Returns n-th signal value.
     *
     * @param pos
     *
     * @return
     */
    float& operator[](unsigned int pos) NOEXCEPT
    {
        return m_values[pos];
    }


    /**
     * @brief Returns n-th signal value.
     *
     * @param pos
     *
     * @return
     */
    float operator[](unsigned int pos) const NOEXCEPT
    {
        return m_values[pos];
    }


    /**
     * @brief Add two signals.
     *
     * @param lhs
     * @param rhs
     *
     * @return New signal
     */
    Signal operator+(const Signal& rhs) const NOEXCEPT
    {
#if ENABLE_SSE && __SSE__
#ifdef __AVX__
        return Signal{_mm256_add_ps(getSseValue(), rhs.getSseValue())};
#else
        return Signal{_mm_add_ps(getSseValue(), rhs.getSseValue())};
#endif
#else
        Signal tmp;

        for (unsigned i = 0; i < Signal::COUNT; ++i)
            tmp[i] = m_values[i] + rhs[i];

        return tmp;
#endif
    }


    /**
     * @brief Add other signal values.
     *
     * @param rhs
     *
     * @return this
     */
    Signal& operator+=(const Signal& rhs) NOEXCEPT
    {
        *this = *this + rhs;
        return *this;
    }


    /**
     * @brief Add value to all signals.
     *
     * @param rhs
     *
     * @return this
     */
    Signal& operator+=(float rhs) NOEXCEPT
    {
        *this = *this + Signal{rhs};
        return *this;
    }


    /**
     * @brief Substract single value from all signals.
     *
     * @param rhs
     *
     * @return New signal
     */
    Signal operator-(const Signal& rhs) const NOEXCEPT
    {
#if ENABLE_SSE && __SSE__
#ifdef __AVX__
        return Signal{_mm256_sub_ps(getSseValue(), rhs.getSseValue())};
#else
        return Signal{_mm_sub_ps(getSseValue(), rhs.getSseValue())};
#endif
#else
        Signal tmp;

        for (unsigned i = 0; i < Signal::COUNT; ++i)
            tmp[i] = m_values[i] - rhs[i];

        return tmp;
#endif
    }


    /**
     * @brief Substract all signals by one value.
     *
     * @param rhs
     *
     * @return this
     */
    Signal& operator-=(const Signal& rhs) NOEXCEPT
    {
        *this = *this - rhs;
        return *this;
    }


    /**
     * @brief Substract all signals by one value.
     *
     * @param rhs
     *
     * @return this
     */
    Signal& operator-=(float rhs) NOEXCEPT
    {
        *this = *this - Signal{rhs};
        return *this;
    }


    /**
     * @brief Multiple single value from all signals.
     *
     * @param rhs
     *
     * @return New signal
     */
    Signal operator*(const Signal& rhs) const NOEXCEPT
    {
#if ENABLE_SSE && __SSE__
#ifdef __AVX__
        return Signal{_mm256_mul_ps(getSseValue(), rhs.getSseValue())};
#else
        return Signal{_mm_mul_ps(getSseValue(), rhs.getSseValue())};
#endif
#else
        Signal tmp;

        for (unsigned i = 0; i < Signal::COUNT; ++i)
            tmp[i] = m_values[i] * rhs[i];

        return tmp;
#endif
    }


    /**
     * @brief Multiply signals.
     *
     * @param rhs
     *
     * @return this
     */
    Signal& operator*=(const Signal& rhs) NOEXCEPT
    {
        *this = *this * rhs;
        return *this;
    }


    /**
     * @brief Multiply all signals by one value.
     *
     * @param rhs
     *
     * @return this
     */
    Signal& operator*=(float rhs) NOEXCEPT
    {
        *this = *this * Signal{rhs};
        return *this;
    }


    /**
     * @brief Multiple single value from all signals.
     *
     * @param rhs
     *
     * @return New signal
     */
    Signal operator/(const Signal& rhs) const NOEXCEPT
    {
#if ENABLE_SSE && __SSE__
#ifdef __AVX__
        return Signal{_mm256_div_ps(getSseValue(), rhs.getSseValue())};
#else
        return Signal{_mm_div_ps(getSseValue(), rhs.getSseValue())};
#endif
#else
        Signal tmp;

        for (unsigned i = 0; i < Signal::COUNT; ++i)
            tmp[i] = m_values[i] / rhs[i];

        return tmp;
#endif
    }


    /**
     * @brief Divide signals
     *
     * @param rhs
     *
     * @return Modified signal.
     */
    Signal& operator/=(const Signal& rhs) NOEXCEPT
    {
        *this = *this / rhs;
        return *this;
    }


    /**
     * @brief Divide all signals by one value.
     *
     * @param rhs
     *
     * @return this
     */
    Signal& operator/=(float rhs) NOEXCEPT
    {
        *this = *this / Signal{rhs};
        return *this;
    }


// Public Accessors
public:


#if ENABLE_SSE && __SSE__
    /**
     * @brief Returns SSE value.
     *
     * @return
     */
#ifdef __AVX__
    __m256& getSseValue() NOEXCEPT
#else
    __m128& getSseValue() NOEXCEPT
#endif
    {
        return m_sse;
    }
#endif


#if ENABLE_SSE && __SSE__
    /**
     * @brief Returns SSE value.
     *
     * @return
     */
#ifdef __AVX__
    const __m256& getSseValue() const NOEXCEPT
#else
    const __m128& getSseValue() const NOEXCEPT
#endif
    {
        return m_sse;
    }
#endif


// Private Data Members
private:

    /// Container for values.
#if ENABLE_SSE && __SSE__
    union {
#ifdef __AVX__
        __m256 m_sse;
#else
        __m128 m_sse;
#endif
        std::array<float, COUNT> m_values;
    };
#else
    std::array<float, COUNT> m_values{};
#endif

};

/* ************************************************************************ */

/**
 * @brief Add single value to all signals.
 *
 * @param lhs
 * @param rhs
 *
 * @return New signal
 */
inline Signal operator+(const Signal& lhs, float rhs) NOEXCEPT
{
    return lhs + Signal{rhs};
}

/* ************************************************************************ */

/**
 * @brief Add single value to all signals.
 *
 * @param lhs
 * @param rhs
 *
 * @return New signal
 */
inline Signal operator+(float lhs, const Signal& rhs) NOEXCEPT
{
    return Signal{lhs} + rhs;
}

/* ************************************************************************ */

/**
 * @brief Substract single value from all signals.
 *
 * @param lhs
 * @param rhs
 *
 * @return New signal
 */
inline Signal operator-(const Signal& lhs, float rhs) NOEXCEPT
{
    return lhs - Signal{rhs};
}

/* ************************************************************************ */

/**
 * @brief Substract single value from all signals.
 *
 * @param lhs
 * @param rhs
 *
 * @return New signal
 */
inline Signal operator-(float lhs, const Signal& rhs) NOEXCEPT
{
    return Signal{lhs} - rhs;
}

/* ************************************************************************ */

/**
 * @brief Multiple single value from all signals.
 *
 * @param lhs
 * @param rhs
 *
 * @return New signal
 */
inline Signal operator*(const Signal& lhs, float rhs) NOEXCEPT
{
    return lhs * Signal{rhs};
}

/* ************************************************************************ */

/**
 * @brief Multiple single value from all signals.
 *
 * @param lhs
 * @param rhs
 *
 * @return New signal
 */
inline Signal operator*(float lhs, const Signal& rhs) NOEXCEPT
{
    return Signal{lhs} * rhs;
}

/* ************************************************************************ */

/**
 * @brief Divide signals by one value.
 *
 * @param lhs
 * @param rhs
 *
 * @return New signal
 */
inline Signal operator/(const Signal& lhs, float rhs) NOEXCEPT
{
    return lhs / Signal{rhs};
}

/* ************************************************************************ */

/**
 * @brief Divide single value by signals.
 *
 * @param lhs
 * @param rhs
 *
 * @return New signal
 */
inline Signal operator/(float lhs, const Signal& rhs) NOEXCEPT
{
    return Signal{lhs} / rhs;
}

/* ************************************************************************ */

/**
 * @brief Computes the e (Euler's number, 2.7182818) raised to the given
 * power arg.
 *
 * @param arg
 *
 * @return
 */
inline Signal exp(const Signal& arg) NOEXCEPT
{
#if OFF // ENABLE_SSE && __SSE__
    return Signal{_mm_exp_ps(arg.getSseValue())};
#else
    Signal tmp;

    for (unsigned i = 0; i < Signal::COUNT; ++i)
        tmp[i] = std::exp(arg[i]);

    return tmp;
#endif
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

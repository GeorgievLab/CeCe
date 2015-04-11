
#pragma once

/* ************************************************************************ */

// C++
#include <array>

#if ENABLE_SSE && __SSE__
#include <xmmintrin.h>
#endif

/* ************************************************************************ */

namespace module {
namespace diffusion {

/* ************************************************************************ */

/**
 * @brief Signal container - it can store multiple signals.
 *
 * @note If SSE is enabled multiple signals are compute at once.
 */
class Signal
{

// Public Types
public:


    /**
     * @brief Type used for store signal value.
     */
    using ValueType = float;


// Public Constants
public:


    /**
     * @brief Number of stored signals
     */
    static constexpr unsigned int COUNT = 4;


    /**
     * @brief Minimum signal value to be ignored.
     */
    static constexpr ValueType IGNORE_LEVEL = 0;


// Public Ctors & Dtors
public:


    /**
     * @brief Default constructor.
     */
    Signal()
#if ENABLE_SSE && __SSE__
        : m_sse(_mm_setzero_ps())
#else
        : m_values{}
#endif
    {
        // Nothing to do
    }


    /**
     * @brief Constructor.
     *
     * @param src
     */
    Signal(std::array<ValueType, COUNT> src)
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
    Signal(__m128 values)
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
    operator bool() const noexcept
    {
#if ENABLE_SSE && __SSE__
        return _mm_movemask_ps(_mm_cmpgt_ps(m_sse, _mm_set1_ps(IGNORE_LEVEL)));
#else
        for (ValueType v : m_values)
            if (v > IGNORE_LEVEL)
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
    ValueType& operator[](unsigned int pos) noexcept
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
    ValueType operator[](unsigned int pos) const noexcept
    {
        return m_values[pos];
    }


    /**
     * @brief Add other signal values.
     *
     * @param sig Second signals array.
     *
     * @return Modified signal.
     */
    Signal operator+(Signal sig) const noexcept
    {
#if ENABLE_SSE && __SSE__
        return Signal{_mm_add_ps(m_sse, sig.m_sse)};
#else
        Signal tmp(m_values);

        for (unsigned i = 0; i < COUNT; ++i)
            tmp.m_values[i] += sig[i];

        return tmp;
#endif
    }


    /**
     * @brief Add other signal values.
     *
     * @param sig Second signals array.
     *
     * @return this
     */
    Signal& operator+=(Signal sig) noexcept
    {
#if ENABLE_SSE && __SSE__
        m_sse = _mm_add_ps(m_sse, sig.m_sse);
#else
        for (unsigned i = 0; i < COUNT; ++i)
            m_values[i] += sig[i];

        return *this;
#endif
    }


    /**
     * @brief Add value to all signals.
     *
     * @param val Signal value.
     *
     * @return Modified signal.
     */
    Signal operator+(ValueType val) const noexcept
    {
#if ENABLE_SSE && __SSE__
        return Signal{_mm_add_ps(m_sse, _mm_set1_ps(val))};
#else
        Signal tmp(m_values);

        for (auto& v : tmp.m_values)
            v += val;

        return tmp;
#endif
    }


    /**
     * @brief Add value to all signals.
     *
     * @param val Signal value.
     *
     * @return this
     */
    Signal& operator+=(ValueType val) noexcept
    {
#if ENABLE_SSE && __SSE__
        m_sse = _mm_add_ps(m_sse, _mm_set1_ps(val));
#else
        for (auto& v : m_values)
            v += val;
#endif
        return *this;
    }


    /**
     * @brief Substract all signals by one value.
     *
     * @param sig Second signals array.
     *
     * @return Modified signals.
     */
    Signal operator-(Signal sig) const noexcept
    {
#if ENABLE_SSE && __SSE__
        return Signal{_mm_add_ps(m_sse, sig.m_sse)};
#else
        Signal tmp(m_values);

        for (unsigned i = 0; i < COUNT; ++i)
            tmp.m_values[i] += sig[i];

        return tmp;
#endif
    }


    /**
     * @brief Substract all signals by one value.
     *
     * @param sig Second signals array.
     *
     * @return this
     */
    Signal& operator-=(Signal sig) noexcept
    {
#if ENABLE_SSE && __SSE__
        m_sse = _mm_sub_ps(m_sse, sig.m_sse);
#else
        for (unsigned i = 0; i < COUNT; ++i)
            m_values[i] += sig[i];

        return *this;
#endif
    }


    /**
     * @brief Substract all signals by one value.
     *
     * @param val
     *
     * @return Modified signal.
     */
    Signal operator-(ValueType val) const noexcept
    {
#if ENABLE_SSE && __SSE__
        return Signal{_mm_sub_ps(m_sse, _mm_set1_ps(val))};
#else
        Signal tmp(m_values);

        for (auto& v : tmp.m_values)
            v += val;

        return tmp;
#endif
    }


    /**
     * @brief Substract all signals by one value.
     *
     * @param val
     *
     * @return this
     */
    Signal& operator-=(ValueType val) noexcept
    {
#if ENABLE_SSE && __SSE__
        m_sse = _mm_sub_ps(m_sse, _mm_set1_ps(val));
#else
        for (auto& v : m_values)
            v += val;
#endif
        return *this;
    }


    /**
     * @brief Multiply all signals by one value.
     *
     * @param val
     *
     * @return Modified signal.
     */
    Signal operator*(ValueType val) const noexcept
    {
#if ENABLE_SSE && __SSE__
        return Signal{_mm_mul_ps(m_sse, _mm_set1_ps(val))};
#else
        Signal tmp(m_values);

        for (auto& v : tmp.m_values)
            v *= val;

        return tmp;
#endif
    }


    /**
     * @brief Multiply all signals by one value.
     *
     * @param val
     *
     * @return this
     */
    Signal& operator*=(ValueType val) noexcept
    {
#if ENABLE_SSE && __SSE__
        m_sse = _mm_mul_ps(m_sse, _mm_set1_ps(val));
#else
        for (auto& v : m_values)
            v *= val;
#endif
        return *this;
    }


    /**
     * @brief Divide all signals by one value.
     *
     * @param val
     *
     * @return Modified signal.
     */
    Signal operator/(ValueType val) const noexcept
    {
#if ENABLE_SSE && __SSE__
        return Signal{_mm_div_ps(m_sse, _mm_set1_ps(val))};
#else
        Signal tmp(m_values);

        for (auto& v : tmp.m_values)
            v *= val;

        return tmp;
#endif
    }


    /**
     * @brief Divide all signals by one value.
     *
     * @param val
     *
     * @return this
     */
    Signal& operator/=(ValueType val) noexcept
    {
#if ENABLE_SSE && __SSE__
        m_sse = _mm_div_ps(m_sse, _mm_set1_ps(val));
#else
        for (auto& v : m_values)
            v *= val;
#endif
        return *this;
    }


// Private Data Members
private:

    /// Container for values.
#if ENABLE_SSE && __SSE__
    union {
        __m128 m_sse;
        std::array<ValueType, COUNT> m_values;
    };
#else
    std::array<ValueType, COUNT> m_values{};
#endif

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */

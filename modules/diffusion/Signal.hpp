
#pragma once

/* ************************************************************************ */

// C++
#include <array>

/* ************************************************************************ */

namespace module {
namespace diffusion {

/* ************************************************************************ */

#if 1 // BETTER_SIGNAL
/**
 * @brief Signal container - it can store multiple signals.
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
    static constexpr unsigned int COUNT = 3;


    /**
     * @brief Minimum signal value to be ignored.
     */
    static constexpr ValueType IGNORE_LEVEL = 0;


// Public Ctors & Dtors
public:


    /**
     * @brief Default constructor.
     */
    Signal() = default;


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


// Public Operators
public:


    /**
     * @brief Check if values are beyond some level.
     */
    operator bool() const noexcept
    {
        // TODO: Optimized??
        for (ValueType v : m_values)
            if (v > IGNORE_LEVEL)
                return true;

        return false;
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
     * @brief Multiply all signals by one value.
     *
     * @param val
     *
     * @return Modified signal.
     */
    Signal operator+(Signal sig) const noexcept
    {
        Signal tmp(m_values);

        for (unsigned i = 0; i < COUNT; ++i)
            tmp.m_values[i] += sig[i];

        return tmp;
    }


    /**
     * @brief Multiply all signals by one value.
     *
     * @param val
     *
     * @return this
     */
    Signal& operator+=(Signal sig) noexcept
    {
        for (unsigned i = 0; i < COUNT; ++i)
            m_values[i] += sig[i];

        return *this;
    }


    /**
     * @brief Multiply all signals by one value.
     *
     * @param val
     *
     * @return Modified signal.
     */
    template<typename T>
    Signal operator+(T val) const noexcept
    {
        Signal tmp(m_values);

        for (auto& v : tmp.m_values)
            v += val;

        return tmp;
    }


    /**
     * @brief Multiply all signals by one value.
     *
     * @param val
     *
     * @return this
     */
    template<typename T>
    Signal& operator+=(T val) noexcept
    {
        for (auto& v : m_values)
            v += val;

        return *this;
    }


    /**
     * @brief Multiply all signals by one value.
     *
     * @param val
     *
     * @return Modified signal.
     */
    template<typename T>
    Signal operator*(T val) const noexcept
    {
        Signal tmp(m_values);

        for (auto& v : tmp.m_values)
            v *= val;

        return tmp;
    }


    /**
     * @brief Multiply all signals by one value.
     *
     * @param val
     *
     * @return this
     */
    template<typename T>
    Signal& operator*=(ValueType val) noexcept
    {
        for (auto& v : m_values)
            v *= val;

        return *this;
    }


// Private Data Members
private:

    /// Container for values.
    std::array<ValueType, COUNT> m_values{};

};
#else
using Signal = float;
#endif
/* ************************************************************************ */

}
}

/* ************************************************************************ */

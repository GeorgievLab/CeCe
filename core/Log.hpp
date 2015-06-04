
#pragma once

/* ************************************************************************ */

// C++
#include <ostream>

// Simulator
#include "core/compatibility.hpp"

/* ************************************************************************ */

#ifndef _MSC_VER
inline namespace core {
#endif

/* ************************************************************************ */

/**
 * @brief Logging class.
 */
class DLL_EXPORT Log
{

// Private Types
private:


    /**
     * @brief End of Line marker.
     */
    struct eol_t {};


// Public Mutators
public:


    /**
     * @brief Set output stream.
     *
     * @param os
     */
    static void setOutput(std::ostream* os) NOEXCEPT
    {
        m_output = os;
    }


    /**
     * @brief Set error stream.
     *
     * @param os
     */
    static void setError(std::ostream* os) NOEXCEPT
    {
        m_error = os;
    }


// Public Operators
public:


    /**
     * @brief Log info message.
     *
     * @param args
     */
    template<typename... Args>
    static void info(Args&&... args)
    {
        message(m_output, std::forward<Args>(args)..., eol_t{});
    }


    /**
     * @brief Log debug message.
     *
     * @param args
     */
    template<typename... Args>
    static void debug(Args&&... args)
    {
#ifndef NDEBUG
        message(m_output, "DEBUG: ", std::forward<Args>(args)..., eol_t{});
#endif
    }


    /**
     * @brief Log warning message.
     *
     * @param args
     */
    template<typename... Args>
    static void warning(Args&&... args)
    {
        message(m_output, "WARNING: ", std::forward<Args>(args)..., eol_t{});
    }


    /**
     * @brief Log error message.
     *
     * @param args
     */
    template<typename... Args>
    static void error(Args&&... args)
    {
        message(m_error, "ERROR: ", std::forward<Args>(args)..., eol_t{});
    }


// Private Operations
private:


    /**
     * @brief Log message.
     */
    static void message(std::ostream* os)
    {
        // Nothing to do
    }


    /**
     * @brief Log message - EOL.
     */
    static void message(std::ostream* os, eol_t)
    {
        if (os)
            *os << std::endl;
    }


    /**
     * @brief Log message.
     *
     * @param args
     */
    template<typename Arg, typename... Args>
    static void message(std::ostream* os, Arg&& arg, Args&&... args)
    {
        if (os)
        {
            *os << arg;
            message(os, std::forward<Args>(args)...);
        }
    }


// Private Data Members
private:

    /// Standard output.
    static std::ostream* m_output;

    /// Error output.
    static std::ostream* m_error;

};

/* ************************************************************************ */

#ifndef _MSC_VER
}
#endif

/* ************************************************************************ */

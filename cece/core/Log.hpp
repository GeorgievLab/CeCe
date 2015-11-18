/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
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

// CeCe
#include "cece/core/OutStream.hpp"

/* ************************************************************************ */

namespace cece {
inline namespace core {

/* ************************************************************************ */

/**
 * @brief Logging class.
 */
class Log
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
    static void setOutput(OutStream* os) noexcept
    {
        m_output = os;
    }


    /**
     * @brief Set error stream.
     *
     * @param os
     */
    static void setError(OutStream* os) noexcept
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
    static void message(OutStream* os)
    {
        // Nothing to do
    }


    /**
     * @brief Log message - EOL.
     */
    static void message(OutStream* os, eol_t)
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
    static void message(OutStream* os, Arg&& arg, Args&&... args)
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
    static OutStream* m_output;

    /// Error output.
    static OutStream* m_error;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */

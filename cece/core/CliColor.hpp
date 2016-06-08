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

// CeCe
#include "cece/export.hpp"
#include "cece/core/OutStream.hpp"

/* ************************************************************************ */

namespace cece {
inline namespace core {

/* ************************************************************************ */

/**
 * @brief IO stream manipulator for output color.
 */
class CliColor
{

// Public Enums
public:


    /**
     * @brief CliColor codes.
     */
    enum class Code
    {
        FG_DEFAULT          = 39,
        FG_BLACK            = 30,
        FG_RED              = 31,
        FG_GREEN            = 32,
        FG_YELLOW           = 33,
        FG_BLUE             = 34,
        FG_MAGENTA          = 35,
        FG_CYAN             = 36,
        FG_LIGHT_GRAY       = 37,
        FG_DARK_GRAY        = 90,
        FG_LIGHT_RED        = 91,
        FG_LIGHT_GREEN      = 92,
        FG_LIGHT_YELLOW     = 93,
        FG_LIGHT_BLUE       = 94,
        FG_LIGHT_MAGENTA    = 95,
        FG_LIGHT_CYAN       = 96,
        FG_WHITE            = 97,
        BG_RED              = 41,
        BG_GREEN            = 42,
        BG_BLUE             = 44,
        BG_DEFAULT          = 49
    };


// Public Constants
public:

    /**
     * @brief Predefined foreground colors.
     */
    static const CliColor CECE_EXPORT Default;
    static const CliColor CECE_EXPORT Black;
    static const CliColor CECE_EXPORT Red;
    static const CliColor CECE_EXPORT Green;
    static const CliColor CECE_EXPORT Yellow;
    static const CliColor CECE_EXPORT Blue;
    static const CliColor CECE_EXPORT Magenta;
    static const CliColor CECE_EXPORT Cyan;
    static const CliColor CECE_EXPORT LightGray;
    static const CliColor CECE_EXPORT DarkGray;
    static const CliColor CECE_EXPORT LightRed;
    static const CliColor CECE_EXPORT LightGreen;
    static const CliColor CECE_EXPORT LightYellow;
    static const CliColor CECE_EXPORT LightBlue;
    static const CliColor CECE_EXPORT LightMagenta;
    static const CliColor CECE_EXPORT LightCyan;
    static const CliColor CECE_EXPORT White;


// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param code Color code.
     */
    constexpr CliColor(Code code) noexcept
        : m_code(code)
    {
        // Nothing to do
    }


// Public Accessors
public:


    /**
     * @brief Returns color code.
     *
     * @return
     */
    Code getCode() const noexcept
    {
        return m_code;
    }


// Public Operations
public:


    /**
     * @brief Output stream operator.
     *
     * @param os    Output stream.
     * @param color Output color.
     *
     * @return os
     */
    friend OutStream& operator<<(OutStream& os, const CliColor& color) noexcept;


// Private Data Members
private:

    /// Format code.
    Code m_code;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */

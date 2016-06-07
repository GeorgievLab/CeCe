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

// Declaration
#include "cece/core/CliColor.hpp"

// Windows
#if _WIN32
#include <Windows.h>
#endif

/* ************************************************************************ */

namespace cece {
inline namespace core {

/* ************************************************************************ */

const CliColor CliColor::Default(CliColor::Code::FG_DEFAULT);
const CliColor CliColor::Black(CliColor::Code::FG_BLACK);
const CliColor CliColor::Red(CliColor::Code::FG_RED);
const CliColor CliColor::Green(CliColor::Code::FG_GREEN);
const CliColor CliColor::Yellow(CliColor::Code::FG_YELLOW);
const CliColor CliColor::Blue(CliColor::Code::FG_BLUE);
const CliColor CliColor::Magenta(CliColor::Code::FG_MAGENTA);
const CliColor CliColor::Cyan(CliColor::Code::FG_CYAN);
const CliColor CliColor::LightGray(CliColor::Code::FG_LIGHT_GRAY);
const CliColor CliColor::DarkGray(CliColor::Code::FG_DARK_GRAY);
const CliColor CliColor::LightRed(CliColor::Code::FG_LIGHT_RED);
const CliColor CliColor::LightGreen(CliColor::Code::FG_LIGHT_GREEN);
const CliColor CliColor::LightYellow(CliColor::Code::FG_LIGHT_YELLOW);
const CliColor CliColor::LightBlue(CliColor::Code::FG_LIGHT_BLUE);
const CliColor CliColor::LightMagenta(CliColor::Code::FG_LIGHT_MAGENTA);
const CliColor CliColor::LightCyan(CliColor::Code::FG_LIGHT_CYAN);
const CliColor CliColor::White(CliColor::Code::FG_WHITE);

/* ************************************************************************ */

OutStream& operator<<(OutStream& os, const CliColor& color) noexcept
{
#if __unix__
    // Color output on Unix based OS (Linux, OSX)
    return os << "\033[" << static_cast<int>(color.getCode()) << "m";
#elif _WIN32
    static HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    static HANDLE err = GetStdHandle(STD_ERROR_HANDLE);
    WORD attr = 0;
    switch (color.getCode())
    {
    default:
    case CliColor::Code::FG_WHITE:
    case CliColor::Code::FG_DEFAULT:        attr = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE; break;
    case CliColor::Code::FG_BLACK:          attr = 0; break;
    case CliColor::Code::FG_RED:            attr = FOREGROUND_RED; break;
    case CliColor::Code::FG_GREEN:          attr = FOREGROUND_GREEN; break;
    case CliColor::Code::FG_YELLOW:         attr = FOREGROUND_RED | FOREGROUND_GREEN; break;
    case CliColor::Code::FG_BLUE:           attr = FOREGROUND_BLUE; break;
    case CliColor::Code::FG_MAGENTA:        attr = FOREGROUND_RED | FOREGROUND_BLUE; break;
    case CliColor::Code::FG_CYAN:           attr = FOREGROUND_GREEN | FOREGROUND_BLUE; break;
    //case CliColor::Code::FG_LIGHT_GRAY:
    //case CliColor::Code::FG_DARK_GRAY:
    case CliColor::Code::FG_LIGHT_RED:      attr = FOREGROUND_RED | FOREGROUND_INTENSITY; break;
    case CliColor::Code::FG_LIGHT_GREEN:    attr = FOREGROUND_GREEN | FOREGROUND_INTENSITY; break;
    case CliColor::Code::FG_LIGHT_YELLOW:   attr = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY; break;
    case CliColor::Code::FG_LIGHT_BLUE:     attr = FOREGROUND_BLUE | FOREGROUND_INTENSITY; break;
    case CliColor::Code::FG_LIGHT_MAGENTA:  attr = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY; break;
    case CliColor::Code::FG_LIGHT_CYAN:     attr = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY; break;
    case CliColor::Code::BG_RED:            attr = BACKGROUND_RED; break;
    case CliColor::Code::BG_GREEN:          attr = BACKGROUND_GREEN; break;
    case CliColor::Code::BG_BLUE:           attr = BACKGROUND_BLUE; break;
    case CliColor::Code::BG_DEFAULT:        attr = 0;
    }

    // Set attribute for stdout & stderr
    SetConsoleTextAttribute(out, attr);
    SetConsoleTextAttribute(err, attr);

    return os;
#else
    // Ignore on others
    return os;
#endif
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

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
#include "cece/render/Color.hpp"

// C++
#include <cstdlib>

// Unix
#if defined(__unix__)
#include <arpa/inet.h>
#elif defined(__MINGW32__) || defined(__MINGW64__)
#include <winsock.h>
// Everyone likes WINAPI macros...
#undef TRANSPARENT
#endif

// CeCe
#include "cece/core/String.hpp"
#include "cece/core/Map.hpp"
#include "cece/core/Exception.hpp"

/* ************************************************************************ */

namespace cece {
namespace render {

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

// Predefined colors
const Map<String, render::Color> g_colors{{
    {"transparent", render::colors::TRANSPARENT},
    {"black", render::colors::BLACK},
    {"white", render::colors::WHITE},
    {"red", render::colors::RED},
    {"green", render::colors::GREEN},
    {"blue", render::colors::BLUE},
    {"yellow", render::colors::YELLOW},
    {"cyan", render::colors::CYAN},
    {"magenta", render::colors::MAGENTA}
}};

/* ************************************************************************ */

}

/* ************************************************************************ */

InStream& operator>>(InStream& is, Color& color)
{
    String str;
    is >> str;

    if (str.empty())
        throw InvalidArgumentException("Empty color value");

    // Find color by name
    auto it = g_colors.find(str);
    if (it != g_colors.end())
    {
        color = it->second;
        return is;
    }

    if (str[0] != '#')
        throw InvalidArgumentException("Invalid color value");

    // Parse hexadecimal value
    uint32_t value = htonl(std::strtoul(str.c_str() + 1, nullptr, 16));
    const uint8_t* bytes = reinterpret_cast<uint8_t*>(&value);

    // Alpha value
    if (bytes[0] > 0)
    {
        color = Color::fromUchar(bytes[0], bytes[1], bytes[2], bytes[3]);
    }
    else
    {
        color = Color::fromUchar(bytes[1], bytes[2], bytes[3]);
    }

    return is;
}

/* ************************************************************************ */

OutStream& operator<<(OutStream& os, const Color& color) noexcept
{
    // Store predefined color
    for (const auto& p : g_colors)
    {
        if (p.second == color)
            return os << p.first;
    }

    const uint8_t bytes[4] = {
        static_cast<uint8_t>(color.getRed()   * 255),
        static_cast<uint8_t>(color.getGreen() * 255),
        static_cast<uint8_t>(color.getBlue()  * 255),
        static_cast<uint8_t>(color.getAlpha() * 255)
    };

    os << '#' << std::hex << bytes[0] << bytes[1] << bytes[2] << bytes[3];

    return os;
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

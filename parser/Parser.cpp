/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* ************************************************************************ */

// Declaration
#include "parser/Parser.hpp"

// C++
#include <cstdint>
#include <cstdlib>
#include <cstring>

// Unix
#if defined(__unix__)
#include <arpa/inet.h>
#elif defined(__MINGW32__) || defined(__MINGW64__)
#include <winsock.h>
#endif

#ifdef _MSC_VER
#pragma comment(lib, "Ws2_32.lib")
#endif

/* ************************************************************************ */

namespace parser {

/* ************************************************************************ */

float parse_number(const char* str, const char** end)
{
    return std::strtof(str, const_cast<char**>(end));
}

/* ************************************************************************ */

String parse_suffix(const char* str, const char** end)
{
    String res;

    for (; *str != '\0' && isalnum(*str); ++str)
    {
        res.push_back(*str);
    }

    if (end) *end = str;

    return res;
}

/* ************************************************************************ */

bool parse_bool(const char* str, const char** end)
{
    return !strcmp(str, "true");
}

/* ************************************************************************ */

#if ENABLE_RENDER
/**
 * @brief Parse color value in format #RRGGBB(AA).
 *
 * @param str
 * @param end
 *
 * @return
 */
render::Color parse_color(const char* str, const char** end)
{
    if (*str != '#')
        throw InvalidArgumentException("Invalid color value");

    // Parse hexadecimal value
    uint32_t value = htonl(std::strtoul(str + 1, const_cast<char**>(end), 16));
    const uint8_t* bytes = reinterpret_cast<uint8_t*>(&value);

    render::Color color;

    // Alpha value
    if (bytes[0] > 0)
    {
        color = {bytes[0] / 255.f, bytes[1] / 255.f, bytes[2] / 255.f, bytes[3] / 255.f};
    }
    else
    {
        color = {bytes[1] / 255.f, bytes[2] / 255.f, bytes[3] / 255.f, 1.f};
    }

    return color;
}
#endif

/* ************************************************************************ */

}

/* ************************************************************************ */


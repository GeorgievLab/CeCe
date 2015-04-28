
/* ************************************************************************ */

// Declaration
#include "parser/Parser.hpp"

// C++
#include <cstdlib>

/* ************************************************************************ */

namespace parser {

/* ************************************************************************ */

float parse_number(const char* str, const char** end)
{
    return std::strtof(str, const_cast<char**>(end));
}

/* ************************************************************************ */

std::string parse_suffix(const char* str, const char** end)
{
    std::string res;

    for (; *str != '\0' && isalnum(*str); ++str)
    {
        res.push_back(*str);
    }

    if (end) *end = str;

    return res;
}

/* ************************************************************************ */

}

/* ************************************************************************ */


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

// C++
#include <string>
#ifdef _WIN32
#  include <cstdio>
#  include <cstdlib>
#endif

/* ************************************************************************ */

namespace cece {
inline namespace core {

/* ************************************************************************ */

/**
 * @brief String type.
 */
using String = std::string;

/* ************************************************************************ */

/**
 * @brief Converts integer value to string.
 *
 * @param value Input value.
 *
 * @return String value.
 */
inline String toString(int value)
{
#if defined(_WIN32)
    {
        char buffer[64];
        snprintf(buffer, sizeof buffer, "%d", value);
        return buffer;
    }
#else
    return std::to_string(value);
#endif
}

/* ************************************************************************ */

/**
 * @brief Converts long value to string.
 *
 * @param value Input value.
 *
 * @return String value.
 */
inline String toString(long value)
{
#if defined(_WIN32)
    {
        char buffer[64];
        snprintf(buffer, sizeof buffer, "%ld", value);
        return buffer;
    }
#else
    return std::to_string(value);
#endif
}

/* ************************************************************************ */

/**
 * @brief Converts long long value to string.
 *
 * @param value Input value.
 *
 * @return String value.
 */
#ifndef _WIN32
inline String toString(long long value)
{
    return std::to_string(value);
}
#endif

/* ************************************************************************ */

/**
 * @brief Converts unsigned value to string.
 *
 * @param value Input value.
 *
 * @return String value.
 */
inline String toString(unsigned value)
{
#if defined(_WIN32)
    {
        char buffer[64];
        snprintf(buffer, sizeof buffer, "%u", value);
        return buffer;
    }
#else
    return std::to_string(value);
#endif
}

/* ************************************************************************ */

/**
 * @brief Converts unsigned long value to string.
 *
 * @param value Input value.
 *
 * @return String value.
 */
inline String toString(unsigned long value)
{
#if defined(_WIN32)
    {
        char buffer[64];
        snprintf(buffer, sizeof buffer, "%lu", value);
        return buffer;
    }
#else
    return std::to_string(value);
#endif
}

/* ************************************************************************ */

/**
 * @brief Converts unsigned long long value to string.
 *
 * @param value Input value.
 *
 * @return String value.
 */
inline String toString(unsigned long long value)
{
#if defined(_WIN32)
    {
        char buffer[64];
        snprintf(buffer, sizeof buffer, "%lu", static_cast<unsigned long>(value));
        return buffer;
    }
#else
    return std::to_string(value);
#endif
}

/* ************************************************************************ */

/**
 * @brief Converts float value to string.
 *
 * @param value Input value.
 *
 * @return String value.
 */
inline String toString(float value)
{
#if defined(_WIN32)
    {
        char buffer[64];
        snprintf(buffer, sizeof buffer, "%f", value);
        return buffer;
    }
#else
    return std::to_string(value);
#endif
}

/* ************************************************************************ */

/**
 * @brief Converts double value to string.
 *
 * @param value Input value.
 *
 * @return String value.
 */
inline String toString(double value)
{
#if defined(_WIN32)
    {
        char buffer[64];
        snprintf(buffer, sizeof buffer, "%f", value);
        return buffer;
    }
#else
    return std::to_string(value);
#endif
}

/* ************************************************************************ */

/**
 * @brief Converts long double value to string.
 *
 * @param value Input value.
 *
 * @return String value.
 */
#ifndef _WIN32
inline String toString(long double value)
{
    return std::to_string(value);
}
#endif

/* ************************************************************************ */

/**
 * @brief Converts string value to integer.
 *
 * @param value Input value.
 *
 * @return Parsed value.
 */
inline int str2i(const String& value)
{
#if defined(_WIN32)
    return std::atoi(value.c_str());
#else
    return std::stoi(value);
#endif
}

/* ************************************************************************ */

/**
 * @brief Converts string value to integer.
 *
 * @param value Input value.
 *
 * @return Parsed value.
 */
inline float str2f(const String& value)
{
#if defined(_WIN32)
    return static_cast<float>(std::atof(value.c_str()));
#else
    return std::stof(value);
#endif
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

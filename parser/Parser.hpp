
#pragma once

/* ************************************************************************ */

// C++
#include <string>
#include <stdexcept>

// Simulator
#include "core/Units.hpp"
#include "core/Vector.hpp"

/* ************************************************************************ */

namespace parser {

/* ************************************************************************ */

using Exception = std::runtime_error;

/* ************************************************************************ */

/**
 * @brief Structure for constructing values from number and suffix.
 */
template<typename T>
struct value_constructor
{
    static T construct(float val, const std::string& suffix)
    {
        return static_cast<T>(val);
    }
};

/* ************************************************************************ */

template<>
struct value_constructor<float>
{
    static float construct(float val, const std::string& suffix)
    {
        if (suffix.empty())
            return val;

        if (suffix == "um")
            return units::um(val);

        if (suffix == "um3")
            return units::um3(val);

        if (suffix == "s")
            return units::s(val);

        if (suffix == "ms")
            return units::ms(val);

        if (suffix == "us")
            return units::us(val);

        throw Exception("Unsupported suffix: " + suffix);
    }
};

/* ************************************************************************ */

/**
 * @brief Parse float value from string.
 *
 * @param str
 * @param end Optional pointer to following sequence.
 *
 * @return
 */
float parse_number(const char* str, const char** end = nullptr);

/* ************************************************************************ */

/**
 * @brief Parse suffix from string.
 *
 * @param str
 * @param end Optional pointer to following sequence.
 *
 * @return
 */
std::string parse_suffix(const char* str, const char** end = nullptr);

/* ************************************************************************ */

/**
 * @brief Parse bool value.
 *
 * @param str
 * @param end Optional pointer to following sequence.
 *
 * @return
 */
bool parse_bool(const char* str, const char** end = nullptr);

/* ************************************************************************ */

/**
 * @brief Parse bool value.
 *
 * @param str
 * @param end Optional pointer to following sequence.
 *
 * @return
 */
inline bool parse_bool(const std::string& str, const char** end = nullptr)
{
    return parse_bool(str.c_str(), end);
}

/* ************************************************************************ */

/**
 * @brief Parse simple float value with optional suffix.
 *
 * @param str
 * @param end
 *
 * @return
 */
template<typename T>
T parse_value(const char* str, const char** end = nullptr)
{
    const char* in_end = str;
    float val = parse_number(in_end, &in_end);
    std::string suffix = parse_suffix(in_end, &in_end);

    if (end) *end = in_end;

    return value_constructor<T>::construct(val, suffix);
}

/* ************************************************************************ */

/**
 * @brief Parse simple float value with optional suffix.
 *
 * @param str
 * @param end
 *
 * @return
 */
template<typename T>
inline T parse_value(const std::string& str, const char** end = nullptr)
{
    return parse_value<T>(str.c_str(), end);
}

/* ************************************************************************ */

/**
 * @brief Parse vector values with optional suffixes separated by space.
 *
 * @param str
 * @param end
 *
 * @return
 */
template<typename T>
Vector<T> parse_vector(const char* str, const char** end = nullptr)
{
    const char* in_end = str;
    T x = parse_value<T>(in_end, &in_end);
    T y = parse_value<T>(in_end, &in_end);

    return Vector<T>{x, y};
}

/* ************************************************************************ */

/**
 * @brief Parse vector values with optional suffixes separated by space.
 *
 * @param str
 * @param end
 *
 * @return
 */
template<typename T>
inline Vector<T> parse_vector(const std::string& str, const char** end = nullptr)
{
    return parse_vector<T>(str.c_str(), end);
}

/* ************************************************************************ */

}

/* ************************************************************************ */

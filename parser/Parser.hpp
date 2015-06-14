/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* ************************************************************************ */

#pragma once

/* ************************************************************************ */

// Simulator
#include "core/compatibility.hpp"
#include "core/String.hpp"
#include "core/Exception.hpp"
#include "core/Units.hpp"
#include "core/Vector.hpp"

#if ENABLE_RENDER
#include "render/Color.hpp"
#endif

/* ************************************************************************ */

namespace parser {

/* ************************************************************************ */

using Exception = RuntimeException;

/* ************************************************************************ */

/**
 * @brief Structure for constructing values from number and suffix.
 */
template<typename T>
struct value_constructor
{
    static T construct(float val, const String& suffix)
    {
        return static_cast<T>(val);
    }
};

/* ************************************************************************ */

template<>
struct value_constructor<float>
{
    static float construct(float val, const String& suffix)
    {
        if (suffix.empty())
            return val;

        throw Exception("Unsupported suffix: " + suffix);
    }
};

/* ************************************************************************ */

template<>
struct value_constructor<units::Length>
{
    static units::Length construct(float val, const String& suffix)
    {
        if (suffix.empty())
            return val;

        if (suffix == "m")
            return units::m(val);

        if (suffix == "mm")
            return units::mm(val);

        if (suffix == "um")
            return units::um(val);

        throw Exception("Unsupported suffix: " + suffix);
    }
};

/* ************************************************************************ */

template<>
struct value_constructor<units::Mass>
{
    static units::Mass construct(float val, const String& suffix)
    {
        if (suffix.empty())
            return val;

        if (suffix == "kg")
            return units::kg(val);

        if (suffix == "g")
            return units::g(val);

        if (suffix == "mg")
            return units::mg(val);

        if (suffix == "ug")
            return units::ug(val);

        throw Exception("Unsupported suffix: " + suffix);
    }
};

/* ************************************************************************ */

template<>
struct value_constructor<units::Time>
{
    static units::Time construct(float val, const String& suffix)
    {
        if (suffix.empty())
            return val;

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

template<>
struct value_constructor<units::Volume>
{
    static units::Volume construct(float val, const String& suffix)
    {
        if (suffix.empty())
            return val;

        if (suffix == "m3")
            return units::m3(val);

        if (suffix == "mm3")
            return units::mm3(val);

        if (suffix == "um3")
            return units::um3(val);

        throw Exception("Unsupported suffix: " + suffix);
    }
};

/* ************************************************************************ */

template<>
struct value_constructor<units::Velocity>
{
    static units::Velocity construct(float val, const String& suffix)
    {
        if (suffix.empty())
            return val;

        if (suffix == "m/s")
            return units::m_s(val);

        if (suffix == "mm/s")
            return units::mm_s(val);

        if (suffix == "um/s")
            return units::um_s(val);

        throw Exception("Unsupported suffix: " + suffix);
    }
};

/* ************************************************************************ */

template<>
struct value_constructor<units::Acceleration>
{
    static units::Acceleration construct(float val, const String& suffix)
    {
        if (suffix.empty())
            return val;

        if (suffix == "m/s2")
            return units::m_s2(val);

        if (suffix == "mm/s2")
            return units::mm_s2(val);

        if (suffix == "um/s2")
            return units::um_s2(val);

        throw Exception("Unsupported suffix: " + suffix);
    }
};

/* ************************************************************************ */

template<>
struct value_constructor<units::Force>
{
    static units::Force construct(float val, const String& suffix)
    {
        if (suffix.empty())
            return val;

        if (suffix == "N")
            return units::N(val);

        if (suffix == "mN")
            return units::mN(val);

        if (suffix == "uN")
            return units::uN(val);

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
float DLL_EXPORT parse_number(const char* str, const char** end = nullptr);

/* ************************************************************************ */

/**
 * @brief Parse suffix from string.
 *
 * @param str
 * @param end Optional pointer to following sequence.
 *
 * @return
 */
String DLL_EXPORT parse_suffix(const char* str, const char** end = nullptr);

/* ************************************************************************ */

/**
 * @brief Parse bool value.
 *
 * @param str
 * @param end Optional pointer to following sequence.
 *
 * @return
 */
bool DLL_EXPORT parse_bool(const char* str, const char** end = nullptr);

/* ************************************************************************ */

/**
 * @brief Parse bool value.
 *
 * @param str
 * @param end Optional pointer to following sequence.
 *
 * @return
 */
inline bool parse_bool(const String& str, const char** end = nullptr)
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
    String suffix = parse_suffix(in_end, &in_end);

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
inline T parse_value(const String& str, const char** end = nullptr)
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
inline Vector<T> parse_vector(const String& str, const char** end = nullptr)
{
    return parse_vector<T>(str.c_str(), end);
}

/* ************************************************************************ */

/**
 * @brief Parse vector values with optional suffixes separated by space.
 *
 * This is alternative version that doesn't require to have all values.
 *
 * @param str
 * @param end
 *
 * @return
 */
template<typename T>
Vector<T> parse_vector_single(const char* str, const char** end = nullptr)
{
    const char* in_end = str;
    T x = parse_value<T>(in_end, &in_end);
    T y;

    if (*in_end != '\0')
        y = parse_value<T>(in_end, &in_end);
    else
        y = x;

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
inline Vector<T> parse_vector_single(const String& str, const char** end = nullptr)
{
    return parse_vector_single<T>(str.c_str(), end);
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
render::Color DLL_EXPORT parse_color(const char* str, const char** end = nullptr);
#endif

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
inline render::Color parse_color(const String& str, const char** end = nullptr)
{
    return parse_color(str.c_str(), end);
}
#endif

/* ************************************************************************ */

/**
 * @brief Parse N values.
 *
 * @tparam T
 * @tparam N
 *
 * @param str
 * @param fn
 * @param end
 */
template<typename T, size_t N>
std::array<T, N> parse_array(const char* str, T (*fn)(const char*, const char**), const char** end = nullptr)
{
    std::array<T, N> res;
    const char* in_end = str;

    for (size_t i = 0; i < N; ++i)
    {
        // Skip spaces
        while (*in_end == ' ')
            ++in_end;

        if (*in_end == '\0')
            break;

        // Parse value
        res[i] = fn(in_end, &in_end);
    }

    return res;
}

/* ************************************************************************ */

/**
 * @brief Parse N values.
 *
 * @tparam T
 * @tparam N
 *
 * @param str
 * @param fn
 * @param end
 */
template<typename T, size_t N>
inline std::array<T, N> parse_array(const String& str, T (*fn)(const char*, const char**), const char** end = nullptr)
{
    return parse_array<T, N>(str.c_str(), fn, end);
}

/* ************************************************************************ */

}

/* ************************************************************************ */

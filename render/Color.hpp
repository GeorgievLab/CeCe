/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* ************************************************************************ */

#pragma once

/* ************************************************************************ */

#if ENABLE_SSE && __SSE__
#include <xmmintrin.h>
#endif

// Simulator
#include "core/StaticArray.hpp"
#include "core/InStream.hpp"
#include "core/OutStream.hpp"

/* ************************************************************************ */

namespace render {

/* ************************************************************************ */

/**
 * @brief Color structure.
 */
class Color
{

// Public Types
public:


    /**
     * @brief Component type.
     */
    using ComponentType = float;


// Public Ctors & Dtors
public:


    /**
     * @brief Default constructor.
     */
    Color() = default;


    /**
     * @brief Constructor.
     *
     * @param gray
     */
    constexpr explicit Color(ComponentType gray) noexcept
        : Color(gray, gray, gray)
    {
        // Nothing to do
    }



    /**
     * @brief Constructor.
     *
     * @param red
     * @param green
     * @param blue
     * @param alpha
     */
    constexpr Color(ComponentType red, ComponentType green, ComponentType blue, ComponentType alpha = 1.0f) noexcept
#ifndef _MSC_VER
        : m_components{{red, green, blue, alpha}}
#endif
    {
#ifdef _MSC_VER
        m_components[0] = red;
        m_components[1] = green;
        m_components[2] = blue;
        m_components[3] = alpha;
#endif
    }


#if ENABLE_SSE && __SSE__
    /**
     * @brief Constructor.
     *
     * @param values
     */
    constexpr Color(__m128 values)
        : m_sse(values)
    {
        // Nothing to do
    }
#endif


// Public Operators
public:


    /**
     * @brief Add colors into other one.
     *
     * @param c
     *
     * @return
     */
    Color operator+(const Color& c) const noexcept
    {
#if ENABLE_SSE && __SSE__
        return Color{_mm_add_ps(m_sse, c.m_sse)};
#else
        Color tmp(*this);

        for (unsigned i = 0; i < 4; ++i)
            tmp.m_components[i] += c.m_components[i];

        return tmp;
#endif
    }


    /**
     * @brief Add other color.
     *
     * @param c
     *
     * @return
     */
    Color& operator+=(const Color& c) noexcept
    {
#if ENABLE_SSE && __SSE__
        m_sse = _mm_add_ps(m_sse, c.m_sse);
#else
        for (unsigned i = 0; i < 4; ++i)
            m_components[i] += c.m_components[i];
#endif
        return *this;
    }


    /**
     * @brief Substract colors into other one.
     *
     * @param c
     *
     * @return
     */
    Color operator-(const Color& c) const noexcept
    {
#if ENABLE_SSE && __SSE__
        return Color{_mm_sub_ps(m_sse, c.m_sse)};
#else
        Color tmp(*this);

        for (unsigned i = 0; i < 4; ++i)
            tmp.m_components[i] -= c.m_components[i];

        return tmp;
#endif
    }


    /**
     * @brief Substract other color.
     *
     * @param c
     *
     * @return
     */
    Color& operator-=(const Color& c) noexcept
    {
#if ENABLE_SSE && __SSE__
        m_sse = _mm_sub_ps(m_sse, c.m_sse);
#else
        for (unsigned i = 0; i < 4; ++i)
            m_components[i] -= c.m_components[i];
#endif
        return *this;
    }


    /**
     * @brief Multiple color by real value.
     *
     * @param val
     *
     * @return
     */
    Color operator*(float val) const noexcept
    {
#if ENABLE_SSE && __SSE__
        return Color{_mm_mul_ps(m_sse, _mm_set1_ps(val))};
#else
        Color tmp(*this);

        for (unsigned i = 0; i < 4; ++i)
            tmp.m_components[i] *= val;

        return tmp;
#endif
    }


    /**
     * @brief Multiple color by real value.
     *
     * @param val
     *
     * @return
     */
    Color& operator*=(float val) noexcept
    {
#if ENABLE_SSE && __SSE__
        m_sse = _mm_mul_ps(m_sse, _mm_set1_ps(val));
#else
        for (unsigned i = 0; i < 4; ++i)
            m_components[i] *= val;
#endif
        return *this;
    }


    /**
     * @brief Divide color by real value.
     *
     * @param val
     *
     * @return
     */
    Color operator/(float val) const noexcept
    {
#if ENABLE_SSE && __SSE__
        return Color{_mm_div_ps(m_sse, _mm_set1_ps(val))};
#else
        Color tmp(*this);

        for (unsigned i = 0; i < 4; ++i)
            tmp.m_components[i] /= val;

        return tmp;
#endif
    }


    /**
     * @brief Divide color by real value.
     *
     * @param val
     *
     * @return
     */
    Color& operator/=(float val) noexcept
    {
#if ENABLE_SSE && __SSE__
        m_sse = _mm_div_ps(m_sse, _mm_set1_ps(val));
#else
        for (unsigned i = 0; i < 4; ++i)
            m_components[i] /= val;
#endif
        return *this;
    }


// Public Accessors
public:


    /**
     * @brief Returns mutable red component.
     *
     * @return
     */
    ComponentType& red() noexcept
    {
        return m_components[0];
    }


    /**
     * @brief Returns red component.
     *
     * @return
     */
    constexpr ComponentType getRed() const noexcept
    {
        return m_components[0];
    }


    /**
     * @brief Returns mutable green component.
     *
     * @return
     */
    ComponentType& green() noexcept
    {
        return m_components[1];
    }


    /**
     * @brief Returns green component.
     *
     * @return
     */
    constexpr ComponentType getGreen() const noexcept
    {
        return m_components[1];
    }


    /**
     * @brief Returns mutable blue component.
     *
     * @return
     */
    ComponentType& blue() noexcept
    {
        return m_components[2];
    }


    /**
     * @brief Returns blue component.
     *
     * @return
     */
    constexpr ComponentType getBlue() const noexcept
    {
        return m_components[2];
    }


    /**
     * @brief Returns mutable alpha component.
     *
     * @return
     */
    ComponentType& alpha() noexcept
    {
        return m_components[3];
    }


    /**
     * @brief Returns alpha component.
     *
     * @return
     */
    constexpr ComponentType getAlpha() const noexcept
    {
        return m_components[3];
    }


// Public Mutators
public:


    /**
     * @brief Set red component.
     *
     * @param red
     */
    void setRed(ComponentType red) noexcept
    {
        m_components[0] = red;
    }


    /**
     * @brief Set green component.
     *
     * @param green
     */
    void setGreen(ComponentType green) noexcept
    {
        m_components[1] = green;
    }


    /**
     * @brief Set blue component.
     *
     * @param blue
     */
    void setBlue(ComponentType blue) noexcept
    {
        m_components[2] = blue;
    }


    /**
     * @brief Set alpha component.
     *
     * @param alpha
     */
    void setAlpha(ComponentType alpha) noexcept
    {
        m_components[3] = alpha;
    }


// Private Data Members
private:

#if ENABLE_SSE && __SSE__
    union {
        __m128 m_sse;
#endif
        StaticArray<ComponentType, 4> m_components;
#if ENABLE_SSE && __SSE__
    };
#endif
};

/* ************************************************************************ */

/**
 * @brief Input stream operator.
 *
 * @param is    Input stream.
 * @param color Result value.
 *
 * @return is.
 */
InStream& operator>>(InStream& is, Color& color);

/* ************************************************************************ */

/**
 * @brief Output stream operator.
 *
 * @param os    Output stream.
 * @param color Input value.
 *
 * @return os.
 */
OutStream& operator<<(OutStream& os, const Color& color) noexcept;

/* ************************************************************************ */

namespace colors  {

/* ************************************************************************ */

constexpr Color BLACK  {0, 0, 0};
constexpr Color WHITE  {1, 1, 1};
constexpr Color RED    {1, 0, 0};
constexpr Color GREEN  {0, 1, 0};
constexpr Color BLUE   {0, 0, 1};
constexpr Color YELLOW {1, 1, 0};
constexpr Color CYAN   {0, 1, 1};
constexpr Color MAGENTA{1, 0, 1};

/* ************************************************************************ */

}

/* ************************************************************************ */

}

/* ************************************************************************ */

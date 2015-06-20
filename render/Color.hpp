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
#include "core/compatibility.hpp"
#include "core/StaticArray.hpp"

/* ************************************************************************ */

namespace render {

/* ************************************************************************ */

/**
 * @brief Color structure.
 */
class DLL_EXPORT Color
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
    CONSTEXPR explicit Color(ComponentType gray) NOEXCEPT
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
    CONSTEXPR Color(ComponentType red, ComponentType green, ComponentType blue, ComponentType alpha = 1.0f) NOEXCEPT
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
    CONSTEXPR Color(__m128 values)
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
    Color operator+(const Color& c) const NOEXCEPT
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
    Color& operator+=(const Color& c) NOEXCEPT
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
    Color operator-(const Color& c) const NOEXCEPT
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
    Color& operator-=(const Color& c) NOEXCEPT
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
    Color operator*(float val) const NOEXCEPT
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
    Color& operator*=(float val) NOEXCEPT
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
    Color operator/(float val) const NOEXCEPT
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
    Color& operator/=(float val) NOEXCEPT
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
    ComponentType& red() NOEXCEPT
    {
        return m_components[0];
    }


    /**
     * @brief Returns red component.
     *
     * @return
     */
    CONSTEXPR ComponentType getRed() const NOEXCEPT
    {
        return m_components[0];
    }


    /**
     * @brief Returns mutable green component.
     *
     * @return
     */
    ComponentType& green() NOEXCEPT
    {
        return m_components[1];
    }


    /**
     * @brief Returns green component.
     *
     * @return
     */
    CONSTEXPR ComponentType getGreen() const NOEXCEPT
    {
        return m_components[1];
    }


    /**
     * @brief Returns mutable blue component.
     *
     * @return
     */
    ComponentType& blue() NOEXCEPT
    {
        return m_components[2];
    }


    /**
     * @brief Returns blue component.
     *
     * @return
     */
    CONSTEXPR ComponentType getBlue() const NOEXCEPT
    {
        return m_components[2];
    }


    /**
     * @brief Returns mutable alpha component.
     *
     * @return
     */
    ComponentType& alpha() NOEXCEPT
    {
        return m_components[3];
    }


    /**
     * @brief Returns alpha component.
     *
     * @return
     */
    CONSTEXPR ComponentType getAlpha() const NOEXCEPT
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
    void setRed(ComponentType red) NOEXCEPT
    {
        m_components[0] = red;
    }


    /**
     * @brief Set green component.
     *
     * @param green
     */
    void setGreen(ComponentType green) NOEXCEPT
    {
        m_components[1] = green;
    }


    /**
     * @brief Set blue component.
     *
     * @param blue
     */
    void setBlue(ComponentType blue) NOEXCEPT
    {
        m_components[2] = blue;
    }


    /**
     * @brief Set alpha component.
     *
     * @param alpha
     */
    void setAlpha(ComponentType alpha) NOEXCEPT
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

namespace colors  {

/* ************************************************************************ */

#ifdef _MSC_VER
extern Color BLACK;
extern Color WHITE;
extern Color RED;
extern Color GREEN;
extern Color BLUE;
extern Color YELLOW;
extern Color CYAN;
extern Color MAGENTA;
#else
CONSTEXPR Color BLACK  {0, 0, 0};
CONSTEXPR Color WHITE  {1, 1, 1};
CONSTEXPR Color RED    {1, 0, 0};
CONSTEXPR Color GREEN  {0, 1, 0};
CONSTEXPR Color BLUE   {0, 0, 1};
CONSTEXPR Color YELLOW {1, 1, 0};
CONSTEXPR Color CYAN   {0, 1, 1};
CONSTEXPR Color MAGENTA{1, 0, 1};
#endif

/* ************************************************************************ */

}

/* ************************************************************************ */

}

/* ************************************************************************ */

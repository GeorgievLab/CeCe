
#pragma once

/* ************************************************************************ */

// C++
#include <array>

#if ENABLE_SSE && __SSE__
#include <xmmintrin.h>
#endif

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
        : m_components{red, green, blue, alpha}
    {
        // Nothing to do
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
    ComponentType& getRed() noexcept
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
    ComponentType& getGreen() noexcept
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
    ComponentType& getBlue() noexcept
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
    ComponentType& getAlpha() noexcept
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


// Private Data Members
private:

#if ENABLE_SSE && __SSE__
    union {
        __m128 m_sse;
#endif
        std::array<ComponentType, 4> m_components;
#if ENABLE_SSE && __SSE__
    };
#endif
};

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

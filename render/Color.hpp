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

// Simulator
#include "core/Real.hpp"
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
    using ComponentType = RealType;


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
        : m_red{red}
        , m_green{green}
        , m_blue{blue}
        , m_alpha{alpha}
    {
        // Nothing to do
    }


// Public Operators
public:


    /**
     * @brief Compare operator.
     *
     * @param rhs Color compare to.
     *
     * @return
     */
    constexpr bool operator==(const Color& rhs) const noexcept
    {
        return (
            m_red   == rhs.m_red   &&
            m_green == rhs.m_green &&
            m_blue  == rhs.m_blue  &&
            m_alpha == rhs.m_alpha
        );
    }


    /**
     * @brief Compare operator.
     *
     * @param rhs Color compare to.
     *
     * @return
     */
    constexpr bool operator!=(const Color& rhs) const noexcept
    {
        return !operator==(rhs);
    }


    /**
     * @brief Add colors into other one.
     *
     * @param rhs Color.
     *
     * @return
     */
    constexpr Color operator+(const Color& rhs) const noexcept
    {
        return Color{
            m_red   + rhs.m_red,
            m_green + rhs.m_green,
            m_blue  + rhs.m_blue,
            m_alpha + rhs.m_alpha
        };
    }


    /**
     * @brief Add other color.
     *
     * @param rhs Color.
     *
     * @return
     */
    Color& operator+=(const Color& rhs) noexcept
    {
        m_red   += rhs.m_red;
        m_green += rhs.m_green;
        m_blue  += rhs.m_blue;
        m_alpha += rhs.m_alpha;

        return *this;
    }


    /**
     * @brief Substract colors into other one.
     *
     * @param rhs Color.
     *
     * @return
     */
    constexpr Color operator-(const Color& rhs) const noexcept
    {
        return Color{
            m_red   - rhs.m_red,
            m_green - rhs.m_green,
            m_blue  - rhs.m_blue,
            m_alpha - rhs.m_alpha
        };
    }


    /**
     * @brief Substract other color.
     *
     * @param rhs Color.
     *
     * @return
     */
    Color& operator-=(const Color& rhs) noexcept
    {
        m_red   -= rhs.m_red;
        m_green -= rhs.m_green;
        m_blue  -= rhs.m_blue;
        m_alpha -= rhs.m_alpha;

        return *this;
    }


    /**
     * @brief Multiple color by real value.
     *
     * @param val
     *
     * @return
     */
    constexpr Color operator*(RealType val) const noexcept
    {
        return Color{
            m_red   * val,
            m_green * val,
            m_blue  * val,
            m_alpha * val
        };
    }


    /**
     * @brief Multiple color by real value.
     *
     * @param val
     *
     * @return
     */
    Color& operator*=(RealType val) noexcept
    {
        m_red   *= val;
        m_green *= val;
        m_blue  *= val;
        m_alpha *= val;

        return *this;
    }


    /**
     * @brief Divide color by real value.
     *
     * @param val
     *
     * @return
     */
    constexpr Color operator/(RealType val) const noexcept
    {
        return Color{
            m_red   / val,
            m_green / val,
            m_blue  / val,
            m_alpha / val
        };
    }


    /**
     * @brief Divide color by real value.
     *
     * @param val
     *
     * @return
     */
    Color& operator/=(RealType val) noexcept
    {
        m_red   /= val;
        m_green /= val;
        m_blue  /= val;
        m_alpha /= val;

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
        return m_red;
    }


    /**
     * @brief Returns red component.
     *
     * @return
     */
    constexpr ComponentType getRed() const noexcept
    {
        return m_red;
    }


    /**
     * @brief Returns mutable green component.
     *
     * @return
     */
    ComponentType& green() noexcept
    {
        return m_green;
    }


    /**
     * @brief Returns green component.
     *
     * @return
     */
    constexpr ComponentType getGreen() const noexcept
    {
        return m_green;
    }


    /**
     * @brief Returns mutable blue component.
     *
     * @return
     */
    ComponentType& blue() noexcept
    {
        return m_blue;
    }


    /**
     * @brief Returns blue component.
     *
     * @return
     */
    constexpr ComponentType getBlue() const noexcept
    {
        return m_blue;
    }


    /**
     * @brief Returns mutable alpha component.
     *
     * @return
     */
    ComponentType& alpha() noexcept
    {
        return m_alpha;
    }


    /**
     * @brief Returns alpha component.
     *
     * @return
     */
    constexpr ComponentType getAlpha() const noexcept
    {
        return m_alpha;
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
        m_red = red;
    }


    /**
     * @brief Set green component.
     *
     * @param green
     */
    void setGreen(ComponentType green) noexcept
    {
        m_green = green;
    }


    /**
     * @brief Set blue component.
     *
     * @param blue
     */
    void setBlue(ComponentType blue) noexcept
    {
        m_blue = blue;
    }


    /**
     * @brief Set alpha component.
     *
     * @param alpha
     */
    void setAlpha(ComponentType alpha) noexcept
    {
        m_alpha = alpha;
    }


// Public Operations
public:


    /**
     * @brief Create gray color.
     *
     * @param value Gray value.
     * @param alpha Optional alpha value.
     *
     * @return
     */
    static constexpr fromGray(ComponentType value, ComponentType alpha = 1) noexcept
    {
        return Color{value, value, value, alpha};
    }


// Private Data Members
private:


    /// Red component.
    ComponentType m_red;

    /// Green component.
    ComponentType m_green;

    /// Blue component.
    ComponentType m_blue;

    /// Alpha component.
    ComponentType m_alpha;
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

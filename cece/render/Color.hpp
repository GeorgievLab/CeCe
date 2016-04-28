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

// CeCe
#include "cece/export.hpp"
#include "cece/core/Real.hpp"
#include "cece/core/InStream.hpp"
#include "cece/core/OutStream.hpp"

/* ************************************************************************ */

namespace cece {
namespace render {

/* ************************************************************************ */

/**
 * @brief Color structure.
 */
class CECE_EXPORT Color
{

// Public Types
public:


    /// Unsigned character type.
    using UCharType = unsigned char;

    /// Component type.
    using ComponentType = float;


// Public Ctors & Dtors
public:


    /**
     * @brief Default constructor.
     */
    constexpr Color() noexcept
        : Color({}, {}, {})
    {
        // Nothing to do
    }


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
     * @brief Return inverted color.
     *
     * @param alpha Invert alpha.
     *
     * @return
     */
    Color inverted(bool alpha = false) const noexcept
    {
        return {1 - getRed(), 1 - getGreen(), 1 - getBlue(), alpha ? 1 - getAlpha() : getAlpha()};
    }


    /**
     * @brief Create gray color.
     *
     * @param value Gray value.
     * @param alpha Optional alpha value.
     *
     * @return
     */
    static constexpr Color fromGray(ComponentType value, ComponentType alpha = 1) noexcept
    {
        return Color{value, value, value, alpha};
    }


    /**
     * @brief Create color from unsigned char values.
     *
     * @param red   Red color.
     * @param green Green color.
     * @param blue  Blue color.
     * @param alpha Optional alpha value.
     *
     * @return
     */
    static constexpr Color fromUchar(UCharType red, UCharType green, UCharType blue, UCharType alpha = 255) noexcept
    {
        return Color{red / 255.f, green / 255.f, blue / 255.f, alpha / 255.f};
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
 * @brief Color addition.
 *
 * @param lhs
 * @param rhs
 *
 * @return
 */
inline Color operator+(Color lhs, const Color& rhs) noexcept
{
    return lhs += rhs;
}

/* ************************************************************************ */

/**
 * @brief Color substraction.
 *
 * @param lhs
 * @param rhs
 *
 * @return
 */
inline Color operator-(Color lhs, const Color& rhs) noexcept
{
    return lhs -= rhs;
}

/* ************************************************************************ */

/**
 * @brief Color multiplication.
 *
 * @param lhs
 * @param rhs
 *
 * @return
 */
inline Color operator*(Color lhs, RealType rhs) noexcept
{
    return lhs *= rhs;
}

/* ************************************************************************ */

/**
 * @brief Color multiplication.
 *
 * @param lhs
 * @param rhs
 *
 * @return
 */
inline Color operator*(RealType lhs, Color rhs) noexcept
{
    return rhs *= lhs;
}

/* ************************************************************************ */

/**
 * @brief Color division.
 *
 * @param lhs
 * @param rhs
 *
 * @return
 */
inline Color operator/(Color lhs, RealType rhs) noexcept
{
    return lhs /= rhs;
}

/* ************************************************************************ */

/**
 * @brief Color division.
 *
 * @param lhs
 * @param rhs
 *
 * @return
 */
inline Color operator/(RealType lhs, Color rhs) noexcept
{
    return rhs /= lhs;
}

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

constexpr Color TRANSPARENT {0, 0, 0, 0};
constexpr Color BLACK       {0, 0, 0};
constexpr Color WHITE       {1, 1, 1};
constexpr Color RED         {1, 0, 0};
constexpr Color GREEN       {0, 1, 0};
constexpr Color BLUE        {0, 0, 1};
constexpr Color YELLOW      {1, 1, 0};
constexpr Color CYAN        {0, 1, 1};
constexpr Color MAGENTA     {1, 0, 1};

/* ************************************************************************ */

}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

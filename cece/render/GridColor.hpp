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
#include "cece/core/Vector.hpp"
#include "cece/core/Grid.hpp"
#include "cece/render/Buffer.hpp"
#include "cece/render/Color.hpp"
#include "cece/render/Texture.hpp"
#include "cece/render/GridBase.hpp"

/* ************************************************************************ */

namespace cece {
namespace render {

/* ************************************************************************ */

class Context;

/* ************************************************************************ */

/**
 * @brief Texture-like grid.
 */
class CECE_EXPORT GridColor : public GridBase
{

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param context Rendering context.
     */
    explicit GridColor(Context& context);


    /**
     * @brief Constructor.
     *
     * @param context Rendering context.
     * @param size
     */
    GridColor(Context& context, Size size);


// Public Operators
public:


    /**
     * @brief Get mutable pixel color.
     *
     * @param coord Pixel coordinates.
     * @param color Pixel color.
     */
    Color& operator[](const Coordinate& coord) noexcept
    {
        return get(coord);
    }


    /**
     * @brief Get pixel color.
     *
     * @param coord Pixel coordinates.
     * @param color Pixel color.
     */
    const Color& operator[](const Coordinate& coord) const noexcept
    {
        return get(coord);
    }


// Public Accessors
public:


    /**
     * @brief Returns if object is initialized.
     */
    bool isInitialized() const noexcept
    {
        return m_buffer.isInitialized()/* && m_texture.isInitialized() */;
    }


    /**
     * @brief Get mutable pixel color.
     *
     * @param coord Pixel coordinates.
     * @param color Pixel color.
     */
    Color& get(const Coordinate& coord) noexcept
    {
        return m_colors[coord];
    }


    /**
     * @brief Get pixel color.
     *
     * @param coord Pixel coordinates.
     * @param color Pixel color.
     */
    const Color& get(const Coordinate& coord) const noexcept
    {
        return m_colors[coord];
    }


// Public Mutators
public:


    /**
     * @brief Set pixel color.
     *
     * @param coord Pixel coordinates.
     * @param color Pixel color.
     */
    void set(const Coordinate& coord, const Color& color) noexcept
    {
        // Set color
        m_colors[coord] = color;
        m_colorsUpdated = true;
    }


    /**
     * @brief Set flag that indicates the texture on GPU should be updated.
     */
    void colorsUpdated() noexcept
    {
        m_colorsUpdated = true;
    }


// Public Operators
public:


    /**
     * @brief Render at current position.
     *
     * @param context Rendering context.
     */
    void draw(Context& context) noexcept;


    /**
     * @brief Resize grid.
     *
     * @param size  New grid size.
     * @param color Background color.
     */
    void resize(Size size, const Color& color = colors::BLACK);


    /**
     * @brief Clear grid to default color.
     *
     * @param color Clear color.
     */
    void clear(const Color& color);


    /**
     * @brief Synchronize local buffer with GPU texture.
     */
    void sync();


// Private Data Members
private:

    /// Texture.
    Texture m_texture;

    /// Buffer for texture rendering.
    Buffer m_buffer;

    /// Buffer for storing texture data.
    Grid<Color> m_colors;

    /// If colors were updated.
    bool m_colorsUpdated = false;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */

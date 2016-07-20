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
#include "cece/core/Vector.hpp"
#include "cece/core/Grid.hpp"
#include "cece/render/Color.hpp"

/* ************************************************************************ */

namespace cece {
namespace render {

/* ************************************************************************ */

/**
 * @brief Image (color grid).
 */
class Image
{

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     */
    Image();


    /**
     * @brief Constructor.
     *
     * @param size Image size.
     */
    explicit Image(Size size);


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


// Public Accessors & Mutators
public:


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


    /**
     * @brief Set pixel color.
     *
     * @param coord Pixel coordinates.
     * @param color Pixel color.
     */
    void set(const Coordinate& coord, const Color& color) noexcept
    {
        m_colors[coord] = color;
    }


    /**
     * @brief Returns image size.
     *
     * @return
     */
    Size getSize() const noexcept
    {
        return m_colors.getSize();
    }


    /**
     * @brief Returns image data.
     *
     * @return
     */
    const Color* getData() const noexcept
    {
        return m_colors.getData();
    }


// Public Operators
public:


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


// Private Data Members
private:

    /// Buffer for storing texture data.
    Grid<Color> m_colors;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */

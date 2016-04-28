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
#include "cece/render/Buffer.hpp"

/* ************************************************************************ */

namespace cece {
namespace render {

/* ************************************************************************ */

class Context;
class Color;

/* ************************************************************************ */

/**
 * @brief Drawing object for rectangle.
 */
class CECE_EXPORT Rectangle
{

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param context Rendering context.
     */
    explicit Rectangle(Context& context);


// Public Accessors
public:


    /**
     * @brief Returns if object is initialized.
     */
    bool isInitialized() const noexcept
    {
        return m_buffer.isInitialized();
    }


// Public Operators
public:


    /**
     * @brief Render at current position.
     *
     * @param context Rendering context.
     * @param color   Rectangle color.
     */
    void draw(Context& context, const Color& color) noexcept;


// Private Data Members
private:

    /// Buffer for texture rendering.
    Buffer m_buffer;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */

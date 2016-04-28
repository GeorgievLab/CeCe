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
#include "cece/render/GridColor.hpp"
#include "cece/render/Shader.hpp"
#include "cece/render/Program.hpp"

/* ************************************************************************ */

namespace cece {
namespace render {

/* ************************************************************************ */

class Context;

/* ************************************************************************ */

/**
 * @brief Texture-like grid with color interpolation.
 */
class CECE_EXPORT GridColorSmooth : public GridColor
{

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param context Rendering context.
     */
    explicit GridColorSmooth(Context& context);


    /**
     * @brief Constructor.
     *
     * @param context Rendering context.
     * @param size
     */
    GridColorSmooth(Context& context, Size size)
        : GridColorSmooth(context)
    {
        resize(std::move(size));
    }


// Public Operators
public:


    /**
     * @brief Render at current position.
     *
     * @param context Rendering context.
     */
    void draw(Context& context) noexcept;


// Private Data Members
private:


    /// Vertex shader.
    Shader m_vertexShader;

    /// Fragment shader.
    Shader m_fragmentShader;

    /// Shader program.
    Program m_program;

    /// Size variable pointer.
    Program::UniformId m_sizePtr;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */

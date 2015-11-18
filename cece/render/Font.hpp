/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
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
#include "cece/core/String.hpp"
#include "cece/core/UniquePtr.hpp"
#include "cece/core/Vector.hpp"
#include "cece/core/StaticArray.hpp"
#include "cece/render/Color.hpp"
#include "cece/render/Buffer.hpp"
#include "cece/render/Shader.hpp"
#include "cece/render/Program.hpp"
#include "cece/render/Texture.hpp"

/* ************************************************************************ */

namespace cece {
namespace render {

/* ************************************************************************ */

class Context;

/* ************************************************************************ */

/**
 * @brief Class storing information about rendered font.
 */
class Font
{

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param context Rendering context.
     * @param name    Font name.
     */
    Font(Context& context, const String& name);


    /**
     * @brief Constructor.
     *
     * @param context Rendering context.
     * @param data    Font data.
     * @param size    Data size.
     */
    Font(Context& context, const unsigned char* data, unsigned int size);


    /**
     * @brief Constructor.
     *
     * @param context Rendering context.
     * @param data    Font data.
     * @param size    Data size.
     */
    template<std::size_t N>
    Font(Context& context, const StaticArray<unsigned char, N>& data)
        : Font(context, data.data(), N)
    {
        // Nothing to do
    }


    /**
     * @brief Destructor.
     */
    ~Font();


// Public Mutators
public:


    /**
     * @brief Set font size.
     *
     * @param size
     */
    void setSize(unsigned int size) noexcept;


// Public Operators
public:


    /**
     * @brief Render text.
     *
     * @param context Rendering context.
     * @param text    Text to render.
     * @param color   Text color.
     * @parma pos     Text position.
     */
    void draw(Context& context, const String& text, const Color& color, const Vector<float> pos = Zero) noexcept;


// Private Data Members
private:

    /// Implementation.
    struct Impl;
    UniquePtr<Impl> m_impl;

    /// Vertex buffer.
    Buffer m_buffer;

    /// Texture for glyph render.
    Texture m_texture;

    /// Vertex shader.
    Shader m_vertexShader;

    /// Fragment shader.
    Shader m_fragmentShader;

    /// Shader program.
    Program m_program;

    /// Texture pointer.
    Program::UniformId m_texPtr;

    /// Color pointer.
    Program::UniformId m_colorPtr;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */

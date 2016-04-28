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

// Declaration
#include "cece/render/Texture.hpp"

// C++
#include <cassert>

#ifdef _WIN32
#include <windows.h>
#endif

// OpenGL
#ifdef __linux__
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#elif defined(_WIN32)
#include <GL/gl.h>
#include "cece/render/glext.h"
#ifdef _MSC_VER
#pragma comment(lib, "opengl32.lib")
#endif
#elif __APPLE__ && __MACH__
#include <OpenGL/gl.h>
#endif

// CeCe
#include "cece/core/DynamicArray.hpp"
#include "cece/render/errors.hpp"

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

/**
 * @brief Supported component type by OpenGL.
 */
template<typename>
struct ColorComponentType;

/* ************************************************************************ */

template<>
struct ColorComponentType<float>
{
    static constexpr GLenum value = GL_FLOAT;
};

/* ************************************************************************ */

}

/* ************************************************************************ */

namespace cece {
namespace render {

/* ************************************************************************ */

Texture::Texture(Context& context, bool filter)
{
    // Generate texture
    gl(glGenTextures(1, &m_id));
    assert(m_id != 0);

    gl(glBindTexture(GL_TEXTURE_2D, m_id));
    gl(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    gl(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    if (filter)
    {
        gl(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        gl(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    }
    else
    {
        gl(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        gl(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    }
}

/* ************************************************************************ */

Texture::Texture(Context& context, Size size, const Color& color, bool filter)
    : Texture(context, filter)
{
    // Init texture
    resize(std::move(size), color);
}

/* ************************************************************************ */

Texture::~Texture()
{
    assert(isInitialized());
    gl(glDeleteTextures(1, &m_id));
}

/* ************************************************************************ */

void Texture::clear(const Color& color)
{
    const auto width = m_size.getWidth();
    const auto height = m_size.getHeight();

    DynamicArray<Color> colors(width * height, color);

    gl(glBindTexture(GL_TEXTURE_2D, m_id));
    gl(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, ColorComponentType<Color::ComponentType>::value, colors.data()));
}

/* ************************************************************************ */

void Texture::resize(Size size, const Color& color)
{
    assert(isInitialized());

    m_size = std::move(size);

    const auto width = m_size.getWidth();
    const auto height = m_size.getHeight();

    // Create initial buffer
    DynamicArray<Color> colors(width * height, color);

    gl(glBindTexture(GL_TEXTURE_2D, m_id));
    gl(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, ColorComponentType<Color::ComponentType>::value, colors.data()));
}

/* ************************************************************************ */

void Texture::update(const Color* colors)
{
    const auto width = m_size.getWidth();
    const auto height = m_size.getHeight();

    gl(glBindTexture(GL_TEXTURE_2D, m_id));
    gl(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, ColorComponentType<Color::ComponentType>::value, colors));
}

/* ************************************************************************ */

void Texture::create(Size size, const Color* colors)
{
    assert(isInitialized());

    m_size = std::move(size);

    const auto width = m_size.getWidth();
    const auto height = m_size.getHeight();

    gl(glBindTexture(GL_TEXTURE_2D, m_id));
    gl(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, ColorComponentType<Color::ComponentType>::value, colors));
}

/* ************************************************************************ */

void Texture::createGray(Size size, const unsigned char* colors)
{
    assert(isInitialized());

    m_size = std::move(size);

    const auto width = m_size.getWidth();
    const auto height = m_size.getHeight();

    gl(glBindTexture(GL_TEXTURE_2D, m_id));
    gl(glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, colors));
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

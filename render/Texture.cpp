/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* ************************************************************************ */

// Declaration
#include "render/Texture.hpp"

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
#include "render/glext.h"
#ifdef _MSC_VER
#pragma comment(lib, "opengl32.lib")
#endif
#elif __APPLE__ && __MACH__
#include <OpenGL/gl.h>
#endif

// Simulator
#include "core/DynamicArray.hpp"
#include "render/errors.hpp"

/* ************************************************************************ */

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
    gl(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_FLOAT, colors.data()));
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
    gl(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, colors.data()));
}

/* ************************************************************************ */

void Texture::update(const Color* colors)
{
    const auto width = m_size.getWidth();
    const auto height = m_size.getHeight();

    gl(glBindTexture(GL_TEXTURE_2D, m_id));
    gl(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_FLOAT, colors));
}

/* ************************************************************************ */

void Texture::create(Size size, const Color* colors)
{
    assert(isInitialized());

    m_size = std::move(size);

    const auto width = m_size.getWidth();
    const auto height = m_size.getHeight();

    gl(glBindTexture(GL_TEXTURE_2D, m_id));
    gl(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, colors));
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

/* ************************************************************************ */

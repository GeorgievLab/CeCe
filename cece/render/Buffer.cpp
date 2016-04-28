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
#include "cece/render/Buffer.hpp"

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

// Simulator
#include "cece/render/errors.hpp"

/* ************************************************************************ */

#ifdef _WIN32
static PFNGLGENBUFFERSPROC glGenBuffers = nullptr;
static PFNGLDELETEBUFFERSPROC glDeleteBuffers = nullptr;
static PFNGLBINDBUFFERPROC glBindBuffer = nullptr;
static PFNGLBUFFERDATAPROC glBufferData = nullptr;
#endif

/* ************************************************************************ */

namespace cece {
namespace render {

/* ************************************************************************ */

Buffer::Buffer(Context& context)
{
#ifdef _WIN32
    if (!glGenBuffers)
        glGenBuffers = (PFNGLGENBUFFERSPROC) wglGetProcAddress("glGenBuffers");
#endif

    // Generate buffer
    gl(glGenBuffers(1, &m_id));
    assert(m_id != 0);
}

/* ************************************************************************ */

Buffer::Buffer(Context& context, SizeType size, const void* data)
    : Buffer(context)
{
    // Init data
    resize(size, data);
}

/* ************************************************************************ */

Buffer::~Buffer()
{
#ifdef _WIN32
    if (!glDeleteBuffers)
        glDeleteBuffers = (PFNGLDELETEBUFFERSPROC) wglGetProcAddress("glDeleteBuffers");
#endif

    assert(isInitialized());
    gl(glDeleteBuffers(1, &m_id));
}

/* ************************************************************************ */

void Buffer::resize(SizeType size, const void* data)
{
    assert(isInitialized());

#ifdef _WIN32
    if (!glBindBuffer)
        glBindBuffer = (PFNGLBINDBUFFERPROC) wglGetProcAddress("glBindBuffer");
    if (!glBufferData)
        glBufferData = (PFNGLBUFFERDATAPROC) wglGetProcAddress("glBufferData");
#endif

    // Bind buffer and set data
    gl(glBindBuffer(GL_ARRAY_BUFFER, m_id));
    gl(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

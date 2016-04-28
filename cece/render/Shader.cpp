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
#include "cece/render/Shader.hpp"

// C++
#include <cassert>

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
#include "cece/core/String.hpp"
#include "cece/core/Exception.hpp"
#include "cece/render/errors.hpp"

/* ************************************************************************ */

#ifdef _WIN32
static PFNGLCREATESHADERPROC glCreateShader = nullptr;
static PFNGLDELETESHADERPROC glDeleteShader = nullptr;
static PFNGLSHADERSOURCEPROC glShaderSource = nullptr;
static PFNGLCOMPILESHADERPROC glCompileShader = nullptr;
static PFNGLGETSHADERIVPROC glGetShaderiv = nullptr;
static PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = nullptr;
#endif

/* ************************************************************************ */

namespace cece {
namespace render {

/* ************************************************************************ */

Shader::~Shader()
{
#ifdef _WIN32
    if (!glDeleteShader)
        glDeleteShader = (PFNGLDELETESHADERPROC)wglGetProcAddress("glDeleteShader");
#endif

    if (m_id)
        gl(glDeleteShader(m_id));
}

/* ************************************************************************ */

void Shader::init(Type type, const char* source, unsigned length)
{
#ifdef _WIN32
    if (!glCreateShader)
        glCreateShader = (PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader");
#endif

    if (type == Type::VERTEX)
        m_id = glCreateShader(GL_VERTEX_SHADER);
    else if (type == Type::FRAGMENT)
        m_id = glCreateShader(GL_FRAGMENT_SHADER);
    else
        m_id = 0;

    assert(m_id);

#ifdef _WIN32
    if (!glShaderSource)
        glShaderSource = (PFNGLSHADERSOURCEPROC)wglGetProcAddress("glShaderSource");
#endif

    // Set shader source
    const GLchar* src = source;
    const GLint len = length;
    gl(glShaderSource(m_id, 1, &src, &len));

#ifdef _WIN32
    if (!glCompileShader)
        glCompileShader = (PFNGLCOMPILESHADERPROC)wglGetProcAddress("glCompileShader");
#endif

    // Compile shader
    gl(glCompileShader(m_id));

#ifdef _WIN32
    if (!glGetShaderiv)
        glGetShaderiv = (PFNGLGETSHADERIVPROC)wglGetProcAddress("glGetShaderiv");
#endif

    // Check if was compiled
    GLint isCompiled = 0;
    glGetShaderiv(m_id, GL_COMPILE_STATUS, &isCompiled);

    if (isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(m_id, GL_INFO_LOG_LENGTH, &maxLength);

#ifdef _WIN32
        if (!glGetShaderInfoLog)
            glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)wglGetProcAddress("glGetShaderInfoLog");
#endif

        // The maxLength includes the NULL character
        DynamicArray<GLchar> err(maxLength);
        glGetShaderInfoLog(m_id, maxLength, &maxLength, &err[0]);
        throw RuntimeException(String(err.begin(), err.end()));
    }
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

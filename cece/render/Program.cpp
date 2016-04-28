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
#include "cece/render/Program.hpp"

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
static PFNGLDELETEPROGRAMPROC glDeleteProgram = nullptr;
static PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = nullptr;
static PFNGLCREATEPROGRAMPROC glCreateProgram = nullptr;
static PFNGLATTACHSHADERPROC glAttachShader = nullptr;
static PFNGLLINKPROGRAMPROC glLinkProgram = nullptr;
static PFNGLGETPROGRAMIVPROC glGetProgramiv = nullptr;
static PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog = nullptr;
static PFNGLDETACHSHADERPROC glDetachShader = nullptr;
#endif

/* ************************************************************************ */

namespace cece {
namespace render {

/* ************************************************************************ */

Program::~Program()
{
#ifdef _WIN32
    if (!glDeleteProgram)
        glDeleteProgram = (PFNGLDELETEPROGRAMPROC)wglGetProcAddress("glDeleteProgram");
#endif

    if (m_id)
        gl(glDeleteProgram(m_id));
}

/* ************************************************************************ */

Program::UniformId Program::getUniformId(const char* name) const noexcept
{
#ifdef _WIN32
    if (!glGetUniformLocation)
        glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)wglGetProcAddress("glGetUniformLocation");
#endif

    assert(getId());
    Program::UniformId id;
    gl(id = glGetUniformLocation(getId(), name));
    return id;
}

/* ************************************************************************ */

void Program::init(const Shader& vs, const Shader& fs)
{
#ifdef _WIN32
    if (!glCreateProgram)
        glCreateProgram = (PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram");
#endif

    m_id = glCreateProgram();
    assert(m_id);

#ifdef _WIN32
    if (!glAttachShader)
        glAttachShader = (PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader");
#endif

    gl(glAttachShader(m_id, vs.getId()));
    gl(glAttachShader(m_id, fs.getId()));

#ifdef _WIN32
    if (!glLinkProgram)
        glLinkProgram = (PFNGLDELETEPROGRAMPROC)wglGetProcAddress("glLinkProgram");
#endif

    gl(glLinkProgram(m_id));

#ifdef _WIN32
    if (!glGetProgramiv)
        glGetProgramiv = (PFNGLGETPROGRAMIVPROC)wglGetProcAddress("glGetProgramiv");
#endif

    GLint isLinked = 0;
    glGetProgramiv(m_id, GL_LINK_STATUS, (int *)&isLinked);
    if(isLinked == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &maxLength);

#ifdef _WIN32
        if (!glGetProgramInfoLog)
            glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)wglGetProcAddress("glGetProgramInfoLog");
#endif

        // The maxLength includes the NULL character
        DynamicArray<GLchar> err(maxLength);
        glGetProgramInfoLog(m_id, maxLength, &maxLength, &err[0]);
        throw RuntimeException(String(err.begin(), err.end()));
    }

#ifdef _WIN32
    if (!glDetachShader)
        glDetachShader = (PFNGLDETACHSHADERPROC)wglGetProcAddress("glDetachShader");
#endif

    glDetachShader(m_id, vs.getId());
    glDetachShader(m_id, fs.getId());
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

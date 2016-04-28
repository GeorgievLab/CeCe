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
#include "cece/export.hpp"

/* ************************************************************************ */

/**
 * @brief Do OpenGL operation and check for errors.
 *
 * @param op Operation
 */
#if !DEV_RENDER_GL_ERRORS
#define gl(op) op
#else
#define gl(op) op; ::cece::render::checkGlError(# op)
#endif

/* ************************************************************************ */

namespace cece {
namespace render {

/* ************************************************************************ */

/**
 * @brief Check OpenGL error. If no error is set, nothing is happend.
 *
 * @param operation
 * @throw
 */
void CECE_EXPORT checkGlError(const char* operation);

/* ************************************************************************ */

/**
 * @brief Throw OpenGL error as an exception with translated name.
 *
 * @param error Error code.
 * @param operation
 * @throw
 */
void CECE_EXPORT throwGlError(GLenum error, const char* operation);

/* ************************************************************************ */

}
}

/* ************************************************************************ */

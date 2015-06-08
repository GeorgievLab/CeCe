/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
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
#include "render/glext.h"
#pragma comment(lib, "opengl32.lib")
#elif __APPLE__ && __MACH__
#include <OpenGL/gl.h>
#endif

/* ************************************************************************ */

/**
 * @brief Do OpenGL operation and check for errors.
 *
 * @param op Operation
 */
#ifdef NDEBUG
#define gl(op) op
#else
#define gl(op) op; ::render::checkGlError(# op)
#endif

/* ************************************************************************ */

namespace render {

/* ************************************************************************ */

/**
 * @brief Check OpenGL error. If no error is set, nothing is happend.
 *
 * @param operation
 * @throw
 */
void checkGlError(const char* operation);

/* ************************************************************************ */

/**
 * @brief Throw OpenGL error as an exception with translated name.
 *
 * @param error Error code.
 * @param operation
 * @throw
 */
void throwGlError(GLenum error, const char* operation);

/* ************************************************************************ */

}

/* ************************************************************************ */

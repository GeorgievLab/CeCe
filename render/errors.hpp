
#pragma once

/* ************************************************************************ */

// OpenGL
#include <GL/gl.h>

/* ************************************************************************ */

/**
 * @brief Do OpenGL operation and check for errors.
 *
 * @param op Operation
 */
#ifdef NDEBUG
#define gl(op) op
#else
#define gl(op) op; checkGlError(# op)
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

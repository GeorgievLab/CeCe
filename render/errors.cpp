
/* ************************************************************************ */

// Simulator
#include "render/errors.hpp"

// C++
#include <map>
#include <string>
#include <stdexcept>

/* ************************************************************************ */

namespace render {

/* ************************************************************************ */

static const std::map<GLenum, std::string> errors{{
    {GL_INVALID_ENUM, "Invalid enum"},
    {GL_INVALID_VALUE, "Invalid value"},
    {GL_INVALID_OPERATION, "Invalid operation"},
    //{GL_INVALID_FRAMEBUFFER_OPERATION, "Invalid framebuffer operation"},
    {GL_OUT_OF_MEMORY, "Out of memory"},
    {GL_STACK_UNDERFLOW, "Stack underflow"},
    {GL_STACK_OVERFLOW, "Stack overflow"}
}};

/* ************************************************************************ */

void checkGlError(const char* operation)
{
    GLenum error = glGetError();

    // Throw error
    if (error != GL_NO_ERROR)
        throwGlError(error, operation);
}

/* ************************************************************************ */

void throwGlError(GLenum error, const char* operation)
{
    auto it = errors.find(error);

    if (it != errors.end())
        throw std::runtime_error(std::string(operation) + ": " + it->second);
    else
        throw std::runtime_error(std::string(operation) + ": Unknown error");
}

/* ************************************************************************ */

}

/* ************************************************************************ */

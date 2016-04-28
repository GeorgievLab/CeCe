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
#include "cece/render/errors.hpp"

// CeCe
#include "cece/core/Map.hpp"
#include "cece/core/String.hpp"
#include "cece/core/Exception.hpp"

/* ************************************************************************ */

namespace cece {
namespace render {

/* ************************************************************************ */

static const Map<GLenum, String> errors{{
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
        throw RuntimeException(String(operation) + ": " + it->second);
    else
        throw RuntimeException(String(operation) + ": Unknown error");
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

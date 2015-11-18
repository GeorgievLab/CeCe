/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
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

// CeCe
#include "cece/core/String.hpp"

// Plugin
#include "cece/plugins/python/Handle.hpp"
#include "cece/plugins/python/ValueCast.hpp"
#include "cece/plugins/python/Exception.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace python {

/* ************************************************************************ */

/**
 * @brief Create object from basic types types.
 *
 * @param value Source value.
 *
 * @return Python object handle.
 */
template<typename T>
ObjectHandle makeObject(T value) noexcept
{
    // Use ValueCast to create python object from value.
    return cast<T>(value);
}

/* ************************************************************************ */

/**
 * @brief Create object from object handle.
 *
 * @param value Python object handle.
 *
 * @return Python object handle.
 */
inline ObjectHandle& makeObject(ObjectHandle& value) noexcept
{
    return value;
}

/* ************************************************************************ */

/**
 * @brief Create object from const object handle.
 *
 * @param value Python object handle.
 *
 * @return Python object handle.
 */
inline const Handle<PyObject>& makeObject(const ObjectHandle& value) noexcept
{
    return value;
}

/* ************************************************************************ */

/**
 * @brief Create object from handle.
 *
 * @param value Python object handle.
 *
 * @return Python object handle.
 */
inline ObjectHandle makeObject(ObjectHandle&& value) noexcept
{
    return value;
}

/* ************************************************************************ */

/**
 * @brief Call Python function.
 *
 * @tparam Args A list of arguments.
 *
 * @param fn   Function object.
 * @param args Function arguments.
 *
 * @return Result object.
 *
 * @throw In case the function call failed.
 */
template<typename... Args>
ObjectHandle call(const ObjectHandle& fn, Args&&... args)
{
    // Variadic templates expand each argument into makeObject that creates
    // PyObject managed handle and pass plain PyObject pointer to function as argument
    // and after call it release the "temporary" PyObject.

    auto res = makeHandle(PyObject_CallFunctionObjArgs(fn, makeObject(args).get()..., nullptr));

    // Function call failed.
    if (!res)
        throw Exception{};

    return res;
}

/* ************************************************************************ */

/**
 * @brief Create string handle from string.
 *
 * @param str Source string.
 *
 * @return Python string object.
 */
inline ObjectHandle str(const char* s) noexcept
{
    return makeHandle(PyString_FromString(s));
}

/* ************************************************************************ */

/**
 * @brief Create string handle from string.
 *
 * @param str Source string.
 *
 * @return Python string object.
 */
inline ObjectHandle str(const String& s) noexcept
{
    return makeHandle(PyString_FromString(s.c_str()));
}

/* ************************************************************************ */

/**
 * @brief Returns none object.
 *
 * @return.
 */
inline ObjectHandle none() noexcept
{
    Py_RETURN_NONE;
}

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */

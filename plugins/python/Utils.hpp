/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* ************************************************************************ */

#pragma once

/* ************************************************************************ */

// Simulator
#include "core/String.hpp"

// Plugin
#include "wrapper.hpp" // FIXME: not required, but it solve some inclusion issue
#include "Handle.hpp"
#include "ValueCast.hpp"

/* ************************************************************************ */

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

}
}

/* ************************************************************************ */

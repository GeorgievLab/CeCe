
#pragma once

/* ************************************************************************ */

// C++
#include <type_traits>
#include <string>

// Simulator
#include "core/compatibility.hpp"

// Module
#include "Python.hpp"
#include "wrapper.hpp"
#include "Handle.hpp"

/* ************************************************************************ */

namespace module {
namespace python {

/* ************************************************************************ */

/**
 * @brief Create object from basic types types.
 *
 * @param value
 *
 * @return
 */
template<typename T>
Handle<PyObject> makeObject(T value) NOEXCEPT
{
    return TypeConvertor<T>::convert(value);
}

/* ************************************************************************ */

/**
 * @brief Create object from const handle.
 *
 * @param value
 *
 * @return
 */
inline
Handle<PyObject>& makeObject(Handle<PyObject>& value) NOEXCEPT
{
    return value;
}

/* ************************************************************************ */

/**
 * @brief Create object from handle.
 *
 * @param value
 *
 * @return
 */
inline
const Handle<PyObject>& makeObject(const Handle<PyObject>& value) NOEXCEPT
{
    return value;
}

/* ************************************************************************ */

/**
 * @brief Create object from handle.
 *
 * @param value
 *
 * @return
 */
inline
Handle<PyObject> makeObject(Handle<PyObject>&& value) NOEXCEPT
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
 * @return
 */
template<typename... Args>
Handle<PyObject> call(const Handle<PyObject>& fn, Args&&... args) NOEXCEPT
{
    return makeHandle(PyObject_CallFunctionObjArgs(fn, makeObject(args).get()..., nullptr));
}

/* ************************************************************************ */

/**
 * @brief Create string handle from string.
 *
 * @param str String.
 *
 * @return
 */
inline Handle<PyObject> str(const char* s) NOEXCEPT
{
    return makeHandle(PyString_FromString(s));
}

/* ************************************************************************ */

/**
 * @brief Create string handle from string.
 *
 * @param str String.
 *
 * @return
 */
inline Handle<PyObject> str(const std::string& s) NOEXCEPT
{
    return makeHandle(PyString_FromString(s.c_str()));
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

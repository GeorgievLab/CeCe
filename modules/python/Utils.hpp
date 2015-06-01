
#pragma once

/* ************************************************************************ */

// C++
#include <type_traits>
#include <string>

// Python
#include <Python.h>

// Simulator
#include "core/compatibility.hpp"

// Library
#include "wrapper.hpp"
#include "Handle.hpp"

/* ************************************************************************ */

namespace module {
namespace python {

/* ************************************************************************ */

/**
 * @brief Create object from integral types.
 *
 * @param value
 *
 * @return
 */
template<
    typename T,
    typename std::enable_if<std::is_integral<typename std::remove_reference<T>::type>::value>::type* = nullptr
>
Handle<PyObject> makeObject(T value) NOEXCEPT
{
    return makeHandle(PyInt_FromLong(value));
}

/* ************************************************************************ */

/**
 * @brief Create object from floating point types.
 *
 * @param value
 *
 * @return
 */
template<
    typename T,
    typename std::enable_if<std::is_floating_point<typename std::remove_reference<T>::type>::value>::type* = nullptr
>
Handle<PyObject> makeObject(T value) NOEXCEPT
{
    return makeHandle(PyFloat_FromDouble(value));
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
const Handle<PyObject>& makeObject(Handle<PyObject>& value) NOEXCEPT
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
 * @brief Create object from value.
 *
 * @param value
 *
 * @return
 */
template<
    typename T,
    typename std::enable_if<!std::is_scalar<typename std::remove_reference<T>::type>::value>::type* = nullptr
>
Handle<PyObject> makeObject(T value) NOEXCEPT
{
    using TC = typename std::remove_const<T>::type;
    assert(TypeDefinition<TC>::definition.tp_name);
    return TypeDefinition<TC>::wrap(value);
}

/* ************************************************************************ */

/**
 * @brief Create object from pointer.
 *
 * @param value
 *
 * @return
 */
template<typename T>
Handle<PyObject> makeObject(T* value) NOEXCEPT
{
    using TC = typename std::remove_const<T>::type*;
    assert(TypeDefinition<TC>::definition.tp_name);
    return TypeDefinition<TC>::wrap(const_cast<TC>(value));
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


#pragma once

/* ************************************************************************ */

// Python
#include <Python.h>

// Simulator
#include "core/compatibility.hpp"

// Library
#include "Handle.hpp"

/* ************************************************************************ */

namespace module {
namespace python {

/* ************************************************************************ */

/**
 * @brief C++ type wrapper object.
 */
template<typename T>
struct ObjectWrapper
{
    PyObject_HEAD
    T value;
};

/* ************************************************************************ */

/**
 * @brief Object type definition.
 *
 * @tparam T
 */
template<typename T>
struct TypeDefinition
{
    /// Object type.
    using type = ObjectWrapper<T>;


    /// Type definition
    static PyTypeObject definition;


    /**
     * @brief Define type object.
     *
     * @param name
     *
     * @return
     */
    static void init(const char* name) NOEXCEPT
    {
        definition = {
            PyObject_HEAD_INIT(NULL)
            0,                              // ob_size
            name,                           // tp_name
            sizeof(ObjectWrapper<T>),       // tp_basicsize
            0,                              // tp_itemsize
            0,                              // tp_dealloc
            0,                              // tp_print
            0,                              // tp_getattr
            0,                              // tp_setattr
            0,                              // tp_compare
            0,                              // tp_repr
            0,                              // tp_as_number
            0,                              // tp_as_sequence
            0,                              // tp_as_mapping
            0,                              // tp_hash
            0,                              // tp_call
            0,                              // tp_str
            0,                              // tp_getattro
            0,                              // tp_setattro
            0,                              // tp_as_buffer
            Py_TPFLAGS_DEFAULT,             // tp_flags
            nullptr,                        // tp_doc
        };
    }


    /**
     * @brief Ready type.
     *
     * @return
     */
    static bool ready() NOEXCEPT
    {
        // Initialize type
        return (PyType_Ready(&definition) >= 0);
    }


    /**
     * @brief Wrap value.
     */
    static Handle<PyObject> wrap(T value) NOEXCEPT
    {
        // Create new object
        auto obj = PyObject_New(ObjectWrapper<T>, &definition);
        obj->value = std::move(value);

        return Handle<PyObject>(reinterpret_cast<PyObject*>(obj));
    }
};

/* ************************************************************************ */

template<typename T>
PyTypeObject TypeDefinition<T>::definition;

/* ************************************************************************ */

}
}

/* ************************************************************************ */

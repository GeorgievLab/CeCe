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

// This must be first
#include "Python.hpp"

// Simulator
#include "core/ViewPtr.hpp"

/* ************************************************************************ */

/**
 * @brief This macro tell other modules the class type is stored elsewhere.
 *
 * @param name Class name.
 */
#define DECLARE_PYTHON_CLASS(name) \
    extern template class plugin::python::TypePtr<name>

/* ************************************************************************ */

/**
 * @brief This macro tell to module there is type definition.
 *
 * @param name Class name.
 */
#define DEFINE_PYTHON_CLASS(name) \
    template class plugin::python::TypePtr<name>

/* ************************************************************************ */

namespace plugin {
namespace python {

/* ************************************************************************ */

/**
 * @brief Helper class for static access to python object types.
 *
 * @tparam T Object type.
 */
template<typename T>
struct TypePtr
{
    /// Pointer to type.
    static PyTypeObject* ptr;
};

/* ************************************************************************ */

/**
 * @brief Helper class for static access to python object types. Specialization
 * for `const T` that points to same type as `T`.
 *
 * @tparam T Object type.
 */
template<typename T>
struct TypePtr<const T> : public TypePtr<T>
{
    // Nothing
};

/* ************************************************************************ */

/**
 * @brief Helper class for static access to python object types. Specialization
 * for `T&` that points to same type as `T`.
 *
 * @tparam T Object type.
 */
template<typename T>
struct TypePtr<T&> : public TypePtr<T>
{
    // Nothing
};

/* ************************************************************************ */

/**
 * @brief Helper class for static access to python object types. Specialization
 * for `const T&` that points to same type as `T`.
 *
 * @tparam T Object type.
 */
template<typename T>
struct TypePtr<const T&> : public TypePtr<T>
{
    // Nothing
};

/* ************************************************************************ */

/**
 * @brief Helper class for static access to python object types. Specialization
 * for `T*` that points to same type as `T`.
 *
 * @tparam T Object type.
 */
template<typename T>
struct TypePtr<T*> : public TypePtr<T>
{
    // Nothing
};

/* ************************************************************************ */

/**
 * @brief Helper class for static access to python object types. Specialization
 * for `T&` that points to same type as `T`.
 *
 * @tparam T Object type.
 */
template<typename T>
struct TypePtr<const T*> : public TypePtr<T>
{
    // Nothing
};

/* ************************************************************************ */

/**
 * @brief Helper class for static access to python object types. Specialization
 * for `ViewPtr<T>` that points to same type as `T`.
 *
 * @tparam T Object type.
 */
template<typename T>
struct TypePtr<ViewPtr<T>> : public TypePtr<T>
{
    // Nothing
};

/* ************************************************************************ */

/**
 * @brief Helper class for static access to python object types. Specialization
 * for `ViewPtr<const T>` that points to same type as `T`.
 *
 * @tparam T Object type.
 */
template<typename T>
struct TypePtr<ViewPtr<const T>> : public TypePtr<T>
{
    // Nothing
};

/* ************************************************************************ */

}
}

/* ************************************************************************ */

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
#include "cece/plugins/python/Python.hpp"

// C++
#include <typeindex>

// CeCe
#include "cece/core/Assert.hpp"
#include "cece/core/String.hpp"
#include "cece/core/Exception.hpp"

// Plugin
#include "cece/plugins/python/View.hpp"
#include "cece/plugins/python/Handle.hpp"
#include "cece/plugins/python/ObjectWrapper.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace python {

/* ************************************************************************ */

/**
 * @brief Type definition.
 *
 * @tparam T
 */
template<typename T>
class Type : public PyTypeObject
{

// Public Types
public:


    /// Wrapper type.
    using SelfType = ObjectWrapper<T>;


// Ctors & Dtors
public:


    /**
     * @brief Constructor.
     */
    explicit Type(String name)
        : PyTypeObject{PyObject_HEAD_INIT(NULL)}
        , m_name(std::move(name))
    {
        tp_name = m_name.c_str();
        tp_basicsize = sizeof(SelfType);
        tp_flags = Py_TPFLAGS_DEFAULT;
    }


// Public Operations
public:


    /**
     * @brief Add type to module.
     *
     * @param module
     */
    void add(View<PyObject> module)
    {
        // Type is not ready
        if (PyType_Ready(this) < 0)
            throw RuntimeException("Cannot finalize type object");

        auto type = reinterpret_cast<PyObject*>(this);

        // Find dot
        auto dot = m_name.find('.');
        Assert(dot != String::npos);

        Py_INCREF(type);
        PyModule_AddObject(module, &m_name[dot + 1], type);

        // Register dynamic type
        registerType(typeid(typename std::remove_pointer<T>::type), this);
    }


    /**
     * @brief Returns required base type.
     *
     * @return
     */
    static View<PyTypeObject> getBaseType(const String& name) noexcept
    {
        String cname = name;

        // Find dot
        auto dot = cname.find('.');
        Assert(dot != String::npos);

        // Change dot to '\0'
        cname[dot] = '\0';

        auto baseModule = makeHandle(PyImport_ImportModule(cname.c_str()));
        auto baseDict = PyModule_GetDict(baseModule);
        auto baseClass = PyMapping_GetItemString(baseDict, const_cast<char*>(&cname[dot + 1]));

        if (!(baseClass && PyType_Check(baseClass)))
            throw RuntimeException("Unable to find '" + name + "' class");

        // Base class
        return reinterpret_cast<PyTypeObject*>(baseClass);
    }


// Private Data Members
private:

    /// Type name.
    String m_name;

};

/* ************************************************************************ */

/**
 * @brief Register dynamic type.
 *
 * @param info Type info.
 * @param type Python type.
 */
void registerType(const std::type_info& info, PyTypeObject* type);

/* ************************************************************************ */

/**
 * @brief Find dynamic type.
 *
 * @param info Type info.
 *
 * @return Dynamic type or nullptr.
 */
PyTypeObject* findType(const std::type_info& info);

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */

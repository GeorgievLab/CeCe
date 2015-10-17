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

// Must be first
#include "plugins/python/Python.hpp"

// Simulator
#include "core/Assert.hpp"
#include "simulator/Configuration.hpp"

// Plugin
#include "plugins/python/ObjectWrapper.hpp"
#include "plugins/python/Utils.hpp"

/* ************************************************************************ */

namespace plugin {
namespace python {

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

/**
 * @brief Type definition.
 */
class Type : public PyTypeObject
{

// Public Types
public:


    /// Wrapper type.
    using SelfType = ObjectWrapper<simulator::Configuration*>;


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
        tp_methods = m_methods;

        // Type is not ready
        if (PyType_Ready(this) < 0)
            throw RuntimeException("Cannot finalize type object");
    }


// Public Operations
public:


    /**
     * @brief Finalize type definition.
     *
     * @param module
     */
    void define(View<PyObject> module) noexcept
    {
        auto type = reinterpret_cast<PyObject*>(this);

        // Find dot
        auto dot = m_name.find('.');
        Assert(dot != String::npos);

        Py_INCREF(type);
        PyModule_AddObject(module, &m_name[dot + 1], type);

        // Register dynamic type
        registerType(typeid(Type), this);
    }


    /**
     * @brief Returns value from configuration.
     *
     * @param self
     * @param args
     * @param kwds
     *
     * @return
     */
    static PyObject* get(SelfType* self, PyObject* args, PyObject* kwds) noexcept
    {
        char* name;
        PyObject* def = nullptr;

        const char* kwlist[] = {"default", nullptr};

        if (!PyArg_ParseTupleAndKeywords(args, kwds, "s|O", const_cast<char**>(kwlist), &name, &def))
            return nullptr;

        if (def)
        {
            if (PyInt_Check(def))
            {
                return makeObject(self->value->get(name, PyInt_AS_LONG(def))).release();
            }
            else if (PyFloat_Check(def))
            {
                return makeObject(self->value->get(name, PyFloat_AS_DOUBLE(def))).release();
            }
            else if (PyLong_Check(def))
            {
                return makeObject(self->value->get(name, PyLong_AsLong(def))).release();
            }
            else
            {
                PyErr_SetString(PyExc_ValueError, "Cannot handle default value type");
                return nullptr;
            }
        }
        else
        {
            return makeObject(self->value->get(name)).release();
        }
    }


// Private Data Members
private:

    /// Type name.
    String m_name;

    /// Type methods.
    PyMethodDef m_methods[2] = {
        {"get", (PyCFunction) get, METH_VARARGS, nullptr},
        {nullptr}  /* Sentinel */
    };

};

/* ************************************************************************ */

Type g_type("simulator.Configuration");

/* ************************************************************************ */

}

/* ************************************************************************ */

void init_simulator_Configuration(PyObject* module)
{
    g_type.define(module);
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

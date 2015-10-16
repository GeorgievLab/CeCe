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
#include "core/VectorUnits.hpp"

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


    /// Vector type.
    using VectorType = ForceVector;

    /// Vector element type.
    using ValueType = typename VectorType::ValueType;

    /// Wrapper type.
    using SelfType = ObjectWrapper<VectorType>;


// Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param name Type name.
     */
    Type(String name)
        : PyTypeObject {PyObject_HEAD_INIT(NULL)}
        , m_name(std::move(name))
    {
        tp_name = m_name.c_str();
        tp_basicsize = sizeof(SelfType);
        tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE;
        tp_getset = m_properties;
        tp_new = (newfunc) &allocate;
        tp_dealloc = (destructor) &deallocate;
        tp_init = (initproc) &constructor;

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
        registerType(typeid(VectorType), this);
    }


    /**
     * @brief Allocate object.
     *
     * @param type
     * @param args
     * @param kwds
     *
     * @return
     */
    static PyObject* allocate(PyTypeObject* type, PyObject* args, PyObject* kwds)
    {
        SelfType* self = reinterpret_cast<SelfType*>(type->tp_alloc(type, 0));

        if (self != nullptr) {
            // Inplace allocation
            new (&self->value) VectorType{};
        }

        return reinterpret_cast<PyObject*>(self);
    }


    /**
     * @brief Deallocate object.
     *
     * @param self
     */
    static void deallocate(SelfType* self) noexcept
    {
        // Call destructor
        self->value.~VectorType();

        self->ob_type->tp_free(reinterpret_cast<PyObject*>(self));
    }


    /**
     * @brief Construct object.
     *
     * @param self
     * @param args
     * @param kwds
     *
     * @return
     */
    static int constructor(SelfType* self, PyObject* args, PyObject* kwds)
    {
        PyObject* x;
        PyObject* y;

        const char* kwlist[] = {"x", "y", nullptr};

        if (!PyArg_ParseTupleAndKeywords(args, kwds, "|OO", const_cast<char**>(kwlist), &x, &y))
            return -1;

        if (x && y)
        {
            // Construct value
            self->value = VectorType(cast<ValueType>(x), cast<ValueType>(y));
        }
        else if (x || y)
        {
            PyErr_SetString(PyExc_ValueError, "Both 'x' and 'y' must be set");
            return -1;
        }

        return 0;
    }


    /**
     * @brief Returns X value.
     *
     * @param self
     *
     * @return
     */
    static PyObject* getX(SelfType* self) noexcept
    {
        return makeObject(self->value.getX()).release();
    }


    /**
     * @brief Set X value.
     *
     * @param self
     * @param value
     *
     * @return
     */
    static int setX(SelfType* self, PyObject* value) noexcept
    {
        self->value.setX(cast<ValueType>(value));

        return 0;
    }


    /**
     * @brief Returns Y value.
     *
     * @param self
     *
     * @return
     */
    static PyObject* getY(SelfType* self) noexcept
    {
        return makeObject(self->value.getY()).release();
    }


    /**
     * @brief Set Y value.
     *
     * @param self
     * @param value
     *
     * @return
     */
    static int setY(SelfType* self, PyObject* value) noexcept
    {
        self->value.setY(cast<ValueType>(value));

        return 0;
    }


// Private Data Members
private:

    /// Type name.
    String m_name;

    /// Type properties.
    PyGetSetDef m_properties[5] = {
        {const_cast<char*>("x"),      (getter) getX, (setter) setX, nullptr},
        {const_cast<char*>("y"),      (getter) getY, (setter) setY, nullptr},
        {const_cast<char*>("width"),  (getter) getX, (setter) setX, nullptr},
        {const_cast<char*>("height"), (getter) getY, (setter) setY, nullptr},
        {nullptr}  /* Sentinel */
    };

};

/* ************************************************************************ */

static Type g_type("core.VectorForce");

/* ************************************************************************ */

}

/* ************************************************************************ */

void init_core_VectorForce(PyObject* module)
{
    g_type.define(module);
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

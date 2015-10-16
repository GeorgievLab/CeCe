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


    /// Wrapper type.
    using SelfType = ObjectWrapper<VelocityVector>;


// Ctors & Dtors
public:


    /**
     * @brief Constructor.
     */
    Type()
        : PyTypeObject {PyObject_HEAD_INIT(NULL)}
    {
        tp_name = "core.VectorVelocity";
        tp_basicsize = sizeof(SelfType);
        tp_flags = Py_TPFLAGS_DEFAULT;
        tp_getset = m_properties;

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

        Py_INCREF(type);
        PyModule_AddObject(module, "VectorVelocity", type);

        // Register dynamic type
        registerType(typeid(SelfType::ValueType), this);
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
        self->value.setX(cast<units::Velocity>(value));

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
        self->value.setY(cast<units::Velocity>(value));

        return 0;
    }


// Private Data Members
private:

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

static Type g_type;

/* ************************************************************************ */

}

/* ************************************************************************ */

void init_core_VectorVelocity(PyObject* module)
{
    g_type.define(module);
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

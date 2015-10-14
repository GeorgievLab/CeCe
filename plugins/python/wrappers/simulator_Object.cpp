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

// Simulator
#include "simulator/Object.hpp"
#include "simulator/Simulation.hpp"

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
    using SelfType = ObjectWrapper<simulator::Object*>;


// Ctors & Dtors
public:


    /**
     * @brief Constructor.
     */
    Type()
        : PyTypeObject {PyObject_HEAD_INIT(NULL)}
    {
        tp_name = "simulator.Object";
        tp_basicsize = sizeof(SelfType);
        tp_flags = Py_TPFLAGS_DEFAULT;
        tp_getset = m_properties;
        tp_methods = m_methods;


        // Type is not ready
        if (PyType_Ready(this) < 0)
            throw RuntimeException("Cannot finalize type object");
    }


// Public Operations
public:


    /**
     * @brief Returns object ID.
     *
     * @param self
     *
     * @return
     */
    static PyObject* getId(SelfType* self) noexcept
    {
        return makeObject(self->value->getId()).release();
    }


    /**
     * @brief Returns object position.
     *
     * @param self
     *
     * @return
     */
    static PyObject* getPosition(SelfType* self) noexcept
    {
        return makeObject(self->value->getPosition()).release();
    }


    /**
     * @brief Sets object position.
     *
     * @param self
     * @param args
     *
     * @return
     */
    static PyObject* setPosition(SelfType* self, PyObject* args) noexcept
    {
        PyObject* position;

        if(!PyArg_ParseTuple(args, "o", &position))
            return NULL;

        self->value->setPosition(cast<PositionVector>(position));

        return none();
    }


    /**
     * @brief Returns object rotation.
     *
     * @param self
     *
     * @return
     */
    static PyObject* getRotation(SelfType* self) noexcept
    {
        return makeObject(self->value->getRotation()).release();
    }


    /**
     * @brief Change object rotation.
     *
     * @param self
     * @param args
     *
     * @return
     */
    static PyObject* setRotation(SelfType* self, PyObject* args) noexcept
    {
        float rotation;

        if(!PyArg_ParseTuple(args, "f", &rotation))
            return NULL;

        self->value->setRotation(units::Angle(rotation));

        return none();
    }


    /**
     * @brief Returns object velocity.
     *
     * @param self
     *
     * @return
     */
    static PyObject* getVelocity(SelfType* self) noexcept
    {
        return makeObject(self->value->getVelocity()).release();
    }


    /**
     * @brief Change object velocity.
     *
     * @param self
     * @param args
     *
     * @return
     */
    static PyObject* setVelocity(SelfType* self, PyObject* args) noexcept
    {
        PyObject* velocity;

        if(!PyArg_ParseTuple(args, "o", &velocity))
            return NULL;

        self->value->setVelocity(cast<VelocityVector>(velocity));

        return none();
    }


    /**
     * @brief Returns object class name.
     *
     * @param self
     *
     * @return
     */
    static PyObject* getClassName(SelfType* self) noexcept
    {
        return makeObject(self->value->getClassName()).release();
    }


    /**
     * @brief Use program.
     *
     * @param self
     * @param args
     *
     * @return
     */
    static PyObject* useProgram(SelfType* self, PyObject* args, void*) noexcept
    {
        char* name;

        if(!PyArg_ParseTuple(args, "s", &name))
            return NULL;

        // Add program
        self->value->useProgram(name);

        return none();
    }


    /**
     * @brief Destroy object.
     *
     * @param self
     *
     * @return
     */
    static PyObject* destroy(SelfType* self) noexcept
    {
        self->value->destroy();

        return none();
    }


// Private Data Members
private:


    /// Type properties.
    PyGetSetDef m_properties[6] = {
        {const_cast<char*>("id"),        (getter) getId,        NULL,                  NULL},
        {const_cast<char*>("position"),  (getter) getPosition,  (setter) setPosition,  NULL},
        {const_cast<char*>("rotation"),  (getter) getRotation,  (setter) setRotation,  NULL},
        {const_cast<char*>("velocity"),  (getter) getVelocity,  (setter) setVelocity,  NULL},
        {const_cast<char*>("className"), (getter) getClassName, NULL,                  NULL},
        {NULL}  /* Sentinel */
    };

    /// Type methods.
    PyMethodDef m_methods[3] = {
        {"useProgram", (PyCFunction) useProgram, METH_VARARGS, NULL},
        {"destroy",    (PyCFunction) destroy, METH_NOARGS, NULL},
        {NULL}  /* Sentinel */
    };

};

/* ************************************************************************ */

static Type g_type;

/* ************************************************************************ */

}

/* ************************************************************************ */

void init_simulator_Object(PyObject* module)
{
    auto type = reinterpret_cast<PyObject*>(&g_type);

    Py_INCREF(type);
    PyModule_AddObject(module, "Object", type);

    // Register type.
    registerType(typeid(std::remove_pointer<Type::SelfType::ValueType>::type), &g_type);

    // Define constants
    PyModule_AddIntConstant(module, "OBJECT_TYPE_STATIC", static_cast<int>(simulator::Object::Type::Static));
    PyModule_AddIntConstant(module, "OBJECT_TYPE_DYNAMIC", static_cast<int>(simulator::Object::Type::Dynamic));
    PyModule_AddIntConstant(module, "OBJECT_TYPE_PINNED", static_cast<int>(simulator::Object::Type::Pinned));
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

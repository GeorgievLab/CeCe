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

using SelfType = ObjectWrapper<simulator::Object*>;

/* ************************************************************************ */

static PyObject* getId(SelfType* self) noexcept
{
    return makeObject(self->value->getId());
}

/* ************************************************************************ */

static PyObject* getPosition(SelfType* self) noexcept
{
    return makeObject(self->value->getPosition());
}

/* ************************************************************************ */

static PyObject* setPosition(SelfType* self, PyObject* args) noexcept
{
    PyObject* position;

    if(!PyArg_ParseTuple(args, "o", &position))
        return NULL;

    self->value->setPosition(cast<PositionVector>(position));

    return none();
}

/* ************************************************************************ */

static PyObject* getRotation(SelfType* self) noexcept
{
    return makeObject(self->value->getRotation());
}

/* ************************************************************************ */

static PyObject* setRotation(SelfType* self, PyObject* args) noexcept
{
    float rotation;

    if(!PyArg_ParseTuple(args, "f", &rotation))
        return NULL;

    self->value->setRotation(units::Angle(rotation));

    return none();
}

/* ************************************************************************ */

static PyObject* getVelocity(SelfType* self) noexcept
{
    return makeObject(self->value->getVelocity());
}

/* ************************************************************************ */

static PyObject* setVelocity(SelfType* self, PyObject* args) noexcept
{
    PyObject* velocity;

    if(!PyArg_ParseTuple(args, "o", &velocity))
        return NULL;

    self->value->setVelocity(cast<VelocityVector>(velocity));

    return none();
}

/* ************************************************************************ */

static PyObject* getClassName(SelfType* self) noexcept
{
    return makeObject(self->value->getClassName());
}

/* ************************************************************************ */

static PyObject* useProgram(SelfType* self, PyObject* args, void*) noexcept
{
    char* name;

    if(!PyArg_ParseTuple(args, "s", &name))
        return NULL;

    // Add program
    self->value->useProgram(name);

    return none();
}

/* ************************************************************************ */

static PyObject* destroy(SelfType* self) noexcept
{
    self->value->destroy();

    return none();
}

/* ************************************************************************ */

static PyGetSetDef g_properties[] = {
    {const_cast<char*>("id"),        (getter) getId,        NULL,                  NULL},
    {const_cast<char*>("position"),  (getter) getPosition,  (setter) setPosition,  NULL},
    {const_cast<char*>("rotation"),  (getter) getRotation,  (setter) setRotation,  NULL},
    {const_cast<char*>("velocity"),  (getter) getVelocity,  (setter) setVelocity,  NULL},
    {const_cast<char*>("className"), (getter) getClassName, NULL,                  NULL},
    {NULL}  /* Sentinel */
};

/* ************************************************************************ */

static PyMethodDef g_methods[] = {
    {"useProgram", (PyCFunction) useProgram, METH_VARARGS, NULL},
    {"destroy",    (PyCFunction) destroy, METH_NOARGS, NULL},
    {NULL}  /* Sentinel */
};

/* ************************************************************************ */

static PyTypeObject g_type = {
    PyObject_HEAD_INIT(NULL)
    0,                              // ob_size
    "simulator.Object",             // tp_name
    sizeof(SelfType),               // tp_basicsize
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

/* ************************************************************************ */

}

/* ************************************************************************ */

void init_simulator_Object(PyObject* module)
{
    g_type.tp_getset = g_properties;
    g_type.tp_methods = g_methods;

    // Type is not ready
    if (PyType_Ready(&g_type) < 0)
        return;

    auto type = reinterpret_cast<PyObject*>(&g_type);

    Py_INCREF(type);
    PyModule_AddObject(module, "Object", type);

    // Register type.
    registerDynamic(typeid(SelfType::ValueType), &g_type);

    // Define constants
    PyModule_AddIntConstant(module, "OBJECT_TYPE_STATIC", static_cast<int>(simulator::Object::Type::Static));
    PyModule_AddIntConstant(module, "OBJECT_TYPE_DYNAMIC", static_cast<int>(simulator::Object::Type::Dynamic));
    PyModule_AddIntConstant(module, "OBJECT_TYPE_PINNED", static_cast<int>(simulator::Object::Type::Pinned));
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

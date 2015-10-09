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
#include "core/Vector.hpp"

// Plugin
#include "plugins/python/ObjectWrapper.hpp"
#include "plugins/python/Utils.hpp"

/* ************************************************************************ */

namespace plugin {
namespace python {

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

using SelfType = ObjectWrapper<VectorFloat>;

/* ************************************************************************ */

static PyObject* getX(SelfType* self) noexcept
{
    return makeObject(self->value.getX());
}

/* ************************************************************************ */

static PyObject* setX(SelfType* self, PyObject* args) noexcept
{
    float x;

    if(!PyArg_ParseTuple(args, "f", &x))
        return NULL;

    self->value.setX(x);

    return none();
}

/* ************************************************************************ */

static PyObject* getY(SelfType* self) noexcept
{
    return makeObject(self->value.getY());
}

/* ************************************************************************ */

static PyObject* setY(SelfType* self, PyObject* args) noexcept
{
    float y;

    if(!PyArg_ParseTuple(args, "f", &y))
        return NULL;

    self->value.setY(y);

    return none();
}

/* ************************************************************************ */

static PyGetSetDef g_properties[] = {
    {const_cast<char*>("x"),      (getter) getX, (setter) setX, NULL},
    {const_cast<char*>("y"),      (getter) getY, (setter) setY, NULL},
    {const_cast<char*>("width"),  (getter) getX, (setter) setX, NULL},
    {const_cast<char*>("height"), (getter) getY, (setter) setY, NULL},
    {NULL}  /* Sentinel */
};

/* ************************************************************************ */

static PyTypeObject g_type = {
    PyObject_HEAD_INIT(NULL)
    0,                              // ob_size
    "core.VectorFloat",             // tp_name
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

void init_core_VectorFloat(PyObject* module)
{
    g_type.tp_getset = g_properties;

    // Type is not ready
    if (PyType_Ready(&g_type) < 0)
        return;

    auto type = reinterpret_cast<PyObject*>(&g_type);

    Py_INCREF(type);
    PyModule_AddObject(module, "VectorFloat", type);

    // Register dynamic type
    registerDynamic(typeid(SelfType::ValueType), &g_type);
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

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

// Diffusion
#include "plugins/diffusion/Module.hpp"

// Plugin
#include "plugins/python/ObjectWrapper.hpp"
#include "plugins/python/Utils.hpp"

/* ************************************************************************ */

namespace plugin {
namespace diffusion_python {

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

using namespace plugin::python;

/* ************************************************************************ */

using SelfType = ObjectWrapper<plugin::diffusion::Module*>;

/* ************************************************************************ */

static PyObject* getSignalCount(SelfType* self) noexcept
{
    return makeObject(self->value->getSignalCount());
}

/* ************************************************************************ */

static PyObject* getGridSize(SelfType* self) noexcept
{
    return makeObject(self->value->getGridSize());
}

/* ************************************************************************ */

static PyObject* getSignalId(SelfType* self, PyObject* args) noexcept
{
    char* name;

    if(!PyArg_ParseTuple(args, "s", &name))
        return NULL;

    return makeObject(self->value->getSignalId(name));
}

/* ************************************************************************ */

static PyObject* getSignal(SelfType* self, PyObject* args) noexcept
{
    int id;
    int x;
    int y;

    if(!PyArg_ParseTuple(args, "iii", &id, &x, &y))
        return NULL;

    const auto value = self->value->getSignal(id, plugin::diffusion::Module::Coordinate(x, y));

    return Py_BuildValue("f", value.value());
}

/* ************************************************************************ */

static PyObject* setSignal(SelfType* self, PyObject* args) noexcept
{
    int id;
    int x;
    int y;
    RealType value;

    if(!PyArg_ParseTuple(args, "iiif", &id, &x, &y, &value))
        return NULL;

    // Set signal value
    self->value->setSignal(
        id,
        plugin::diffusion::Module::Coordinate(x, y),
        plugin::diffusion::Module::SignalConcentration(value)
    );

    Py_RETURN_NONE;
}

/* ************************************************************************ */

static PyGetSetDef g_properties[] = {
    {const_cast<char*>("signalCount"),  (getter) getSignalCount,    NULL,  NULL},
    {const_cast<char*>("gridSize"),     (getter) getGridSize,       NULL,  NULL},
    {NULL}  /* Sentinel */
};

/* ************************************************************************ */

static PyMethodDef g_methods[] = {
    {"getSignalId", (PyCFunction) getSignalId,  METH_VARARGS, NULL},
    {"getSignal",   (PyCFunction) getSignal,    METH_VARARGS, NULL},
    {"setSignal",   (PyCFunction) setSignal,    METH_VARARGS, NULL},
    {NULL}  /* Sentinel */
};

/* ************************************************************************ */

static PyTypeObject g_type = {
    PyObject_HEAD_INIT(NULL)
    0,                              // ob_size
    "diffusion.Module",             // tp_name
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

void init_Module(PyObject* module)
{
    g_type.tp_getset = g_properties;
    g_type.tp_methods = g_methods;

    auto baseClass = findType(typeid(simulator::Module*));
    if (!baseClass)
        throw RuntimeException("Unable to find 'simulator.Module' class");

    assert(PyType_Check(baseClass));

    // Base class
    g_type.tp_base = (PyTypeObject*) baseClass;

    // Type is not ready
    if (PyType_Ready(&g_type) < 0)
        return;

    auto type = reinterpret_cast<PyObject*>(&g_type);

    Py_INCREF(type);
    PyModule_AddObject(module, "Module", type);

    // Register dynamic type
    registerType(typeid(SelfType::ValueType), &g_type);
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

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

// Cell
#include "plugins/cell/Yeast.hpp"

// Plugin
#include "plugins/python/ObjectWrapper.hpp"
#include "plugins/python/Type.hpp"
#include "plugins/python/Utils.hpp"

/* ************************************************************************ */

namespace plugin {
namespace cell_python {

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

using namespace plugin::python;

/* ************************************************************************ */

using SelfType = ObjectWrapper<plugin::cell::CellBase*>;

/* ************************************************************************ */

PyObject* getVolume(SelfType* self) noexcept
{
    return makeObject(self->value->getVolume()).release();
}

/* ************************************************************************ */

PyObject* setVolume(SelfType* self, PyObject* args) noexcept
{
    float volume;

    if(!PyArg_ParseTuple(args, "f", &volume))
        return NULL;

    self->value->setVolume(units::Volume(volume));

    return none();
}

/* ************************************************************************ */

PyObject* getGrowthRate(SelfType* self) noexcept
{
    return makeObject(self->value->getGrowthRate()).release();
}

/* ************************************************************************ */

PyObject* setGrowthRate(SelfType* self, PyObject* args) noexcept
{
    float growthRate;

    if(!PyArg_ParseTuple(args, "f", &growthRate))
        return NULL;

    self->value->setGrowthRate(plugin::cell::CellBase::GrowthRate(growthRate));

    return none();
}

/* ************************************************************************ */

PyObject* getMoleculeCount(SelfType* self, PyObject* args) noexcept
{
    char* name;

    if(!PyArg_ParseTuple(args, "s", &name))
        return NULL;

    return makeObject(self->value->getMoleculeCount(name)).release();
}

/* ************************************************************************ */

PyObject* kill(SelfType* self) noexcept
{
    self->value->kill();

    return none();
}

/* ************************************************************************ */

PyGetSetDef g_properties[] = {
    {const_cast<char*>("volume"),     (getter) getVolume,     (setter) setVolume,       NULL},
    {const_cast<char*>("growthRate"), (getter) getGrowthRate, (setter) setGrowthRate,   NULL},
    {NULL}  /* Sentinel */
};

/* ************************************************************************ */

PyMethodDef g_methods[] = {
    {"moleculeCount", (PyCFunction) getMoleculeCount, METH_VARARGS, NULL},
    {"kill",          (PyCFunction) kill,             METH_NOARGS,  NULL},
    {NULL}  /* Sentinel */
};

/* ************************************************************************ */

PyTypeObject g_type = {
    PyObject_HEAD_INIT(NULL)
    0,                              // ob_size
    "cell.CellBase",                // tp_name
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

void init_CellBase(PyObject* module)
{
    g_type.tp_getset = g_properties;
    g_type.tp_methods = g_methods;

    auto baseModule = makeHandle(PyImport_ImportModule("simulator"));
    auto baseDict = PyModule_GetDict(baseModule);
    auto baseClass = PyMapping_GetItemString(baseDict, const_cast<char*>("Object"));
    assert(baseClass);
    assert(PyType_Check(baseClass));

    // Base class
    g_type.tp_base = (PyTypeObject*) baseClass;

    // Type is not ready
    if (PyType_Ready(&g_type) < 0)
        return;

    auto type = reinterpret_cast<PyObject*>(&g_type);

    Py_INCREF(type);
    PyModule_AddObject(module, "CellBase", type);

    // Register dynamic type
    registerType(typeid(std::remove_pointer<SelfType::ValueType>::type), &g_type);
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

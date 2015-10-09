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

using SelfType = ObjectWrapper<simulator::Simulation*>;

/* ************************************************************************ */

static PyObject* getWorldSize(SelfType* self) noexcept
{
    return makeObject(self->value->getWorldSize());
}

/* ************************************************************************ */

static PyObject* getIteration(SelfType* self) noexcept
{
    return makeObject(self->value->getIteration());
}

/* ************************************************************************ */

static PyObject* getIterations(SelfType* self) noexcept
{
    return makeObject(self->value->getIterations());
}

/* ************************************************************************ */

static PyObject* setIterations(SelfType* self, PyObject* args) noexcept
{
    int iterations;

    if(!PyArg_ParseTuple(args, "i", &iterations))
        return NULL;

    self->value->setIterations(iterations);

    return none();
}

/* ************************************************************************ */

static PyObject* getTimeStep(SelfType* self) noexcept
{
    return makeObject(self->value->getTimeStep());
}

/* ************************************************************************ */

static PyObject* setTimeStep(SelfType* self, PyObject* args) noexcept
{
    float timeStep;

    if(!PyArg_ParseTuple(args, "f", &timeStep))
        return NULL;

    self->value->setTimeStep(units::Time(timeStep));

    return none();
}

/* ************************************************************************ */

static PyObject* getTotalTime(SelfType* self) noexcept
{
    return makeObject(self->value->getTotalTime());
}

/* ************************************************************************ */

static PyObject* getObjectCount(SelfType* self) noexcept
{
    return makeObject(self->value->getObjectCount());
}

/* ************************************************************************ */

static PyObject* useModule(SelfType* self, PyObject* args, void*) noexcept
{
    char* name;

    if(!PyArg_ParseTuple(args, "s", &name))
        return NULL;

    return makeObject(self->value->useModule(name));
}

/* ************************************************************************ */

static PyObject* buildObject(SelfType* self, PyObject* args, void*) noexcept
{
    char* name;
    int type = static_cast<int>(simulator::Object::Type::Dynamic);

    if(!PyArg_ParseTuple(args, "s|i", &name, &type))
        return NULL;

    return makeObject(self->value->buildObject(name, static_cast<simulator::Object::Type>(type)));
}

/* ************************************************************************ */

static PyObject* objectCountType(SelfType* self, PyObject* args, void*) noexcept
{
    char* name;

    if(!PyArg_ParseTuple(args, "s", &name))
        return NULL;

    return makeObject(self->value->getObjectCountType(name));
}

/* ************************************************************************ */

static PyObject* getParameter(SelfType* self, PyObject* args, void*) noexcept
{
    char* name;
    float def = 0;

    if(!PyArg_ParseTuple(args, "s|f", &name, &def))
        return NULL;

    // Return parameter
    return makeObject(self->value->getParameter(name, def));
}

/* ************************************************************************ */

static PyObject* getObject(SelfType* self, PyObject* args, void*) noexcept
{
    int pos;

    if(!PyArg_ParseTuple(args, "i", &pos))
        return NULL;

    return makeObject(self->value->getObjects()[pos].get());
}

/* ************************************************************************ */

static PyGetSetDef g_properties[] = {
    {const_cast<char*>("worldSize"),   (getter) getWorldSize,   NULL,                   NULL},
    {const_cast<char*>("iteration"),   (getter) getIteration,   NULL,                   NULL},
    {const_cast<char*>("iterations"),  (getter) getIterations,  (setter) setIterations, NULL},
    {const_cast<char*>("timeStep"),    (getter) getTimeStep,    (setter) setTimeStep,   NULL},
    {const_cast<char*>("totalTime"),   (getter) getTotalTime,   NULL,                   NULL},
    {const_cast<char*>("objectCount"), (getter) getObjectCount, NULL,                   NULL},
    {NULL}  /* Sentinel */
};

/* ************************************************************************ */

static PyMethodDef g_methods[] = {
    {"useModule",       (PyCFunction) useModule,        METH_VARARGS, NULL},
    {"buildObject",     (PyCFunction) buildObject,      METH_VARARGS, NULL},
    {"objectCountType", (PyCFunction) objectCountType,  METH_VARARGS, NULL},
    {"getParameter",    (PyCFunction) getParameter,     METH_VARARGS, NULL},
    {"getObject",       (PyCFunction) getObject,        METH_VARARGS, NULL},
    {NULL}  /* Sentinel */
};

/* ************************************************************************ */

static PyTypeObject g_type = {
    PyObject_HEAD_INIT(NULL)
    0,                              // ob_size
    "simulator.Simulation",         // tp_name
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

void init_simulator_Simulation(PyObject* module)
{
    g_type.tp_getset = g_properties;
    g_type.tp_methods = g_methods;

    // Type is not ready
    if (PyType_Ready(&g_type) < 0)
        return;

    auto type = reinterpret_cast<PyObject*>(&g_type);

    Py_INCREF(type);
    PyModule_AddObject(module, "Simulation", type);

    // Register type.
    registerType(typeid(SelfType::ValueType), &g_type);
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

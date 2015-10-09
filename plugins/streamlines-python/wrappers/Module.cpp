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
#include "plugins/streamlines/Module.hpp"

// Plugin
#include "plugins/python/ObjectWrapper.hpp"
#include "plugins/python/Utils.hpp"
#include "plugins/python/wrapper.hpp"

/* ************************************************************************ */

namespace plugin {
namespace streamlines_python {

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

using namespace plugin::python;

/* ************************************************************************ */

using SelfType = ObjectWrapper<plugin::streamlines::Module*>;

/* ************************************************************************ */

static PyObject* setLayout(SelfType* self, PyObject* args) noexcept
{
    using namespace plugin::streamlines;

    PyObject* array;

    if (!PyArg_ParseTuple(args, "O", &array))
        return nullptr;

    // Get array iterator
    auto iter = makeHandle(PyObject_GetIter(array));

    if (!iter)
    {
        PyErr_SetString(PyExc_RuntimeError, "Not an array");
        return nullptr;
    }

    Module::Layout layout;
    StaticArray<Module::LayoutPosition, Module::LayoutPosCount> order{{
        Module::LayoutPosTop,
        Module::LayoutPosRight,
        Module::LayoutPosBottom,
        Module::LayoutPosLeft
    }};

    for (auto pos : order)
    {
        auto next = makeHandle(PyIter_Next(iter));
        if (!next)
        {
            PyErr_SetString(PyExc_RuntimeError, "Missing layout specifier(s)");
            return nullptr;
        }

        InStringStream iss(cast<String>(next));
        iss >> layout[pos];
    }

    // Set layout
    assert(self->value);
    self->value->setLayout(layout);

    // Return None
    return none();
}

/* ************************************************************************ */

static PyObject* initBarriers(SelfType* self, PyObject* args) noexcept
{
    PyObject* simulation;

    if (!PyArg_ParseTuple(args, "O", &simulation))
        return nullptr;

    // Find simulation type.
    auto type = findDynamic(typeid(simulator::Simulation*));

    if (PyObject_TypeCheck(simulation, type) <= 0)
    {
        PyErr_SetString(PyExc_RuntimeError, "Argument is not simulator.Simulation");
        return nullptr;
    }

    auto sim = reinterpret_cast<ObjectWrapper<simulator::Simulation*>*>(simulation);

    // Init barriers
    self->value->initBarriers(*sim->value);

    // Return None
    return none();
}

/* ************************************************************************ */

static PyObject* setInletVelocity(SelfType* self, PyObject* args) noexcept
{
    int position;
    float velocity;

    if(!PyArg_ParseTuple(args, "if", &position, &velocity))
        return NULL;

    self->value->setInletVelocity(
        static_cast<plugin::streamlines::Module::LayoutPosition>(position),
        units::Velocity(velocity)
    );

    return none();
}

/* ************************************************************************ */

static PyMethodDef g_methods[] = {
    {"setLayout",        (PyCFunction) setLayout,        METH_VARARGS, NULL},
    {"initBarriers",     (PyCFunction) initBarriers,     METH_VARARGS, NULL},
    {"setInletVelocity", (PyCFunction) setInletVelocity, METH_VARARGS, NULL},
    {NULL}  /* Sentinel */
};

/* ************************************************************************ */

static PyTypeObject g_type = {
    PyObject_HEAD_INIT(NULL)
    0,                              // ob_size
    "streamlines.Module",           // tp_name
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
    g_type.tp_methods = g_methods;

    auto baseClass = findDynamic(typeid(simulator::Module*));
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
    registerDynamic(typeid(SelfType::ValueType), &g_type);

    // Define constants
    PyModule_AddIntConstant(module, "LEFT", static_cast<int>(plugin::streamlines::Module::LayoutPosLeft));
    PyModule_AddIntConstant(module, "RIGHT", static_cast<int>(plugin::streamlines::Module::LayoutPosRight));
    PyModule_AddIntConstant(module, "TOP", static_cast<int>(plugin::streamlines::Module::LayoutPosTop));
    PyModule_AddIntConstant(module, "BOTTOM", static_cast<int>(plugin::streamlines::Module::LayoutPosBottom));
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

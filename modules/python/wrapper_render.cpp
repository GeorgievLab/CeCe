
/* ************************************************************************ */

// Declaration
#include "wrapper_render.hpp"

// Python
#include <Python.h>

#if ENABLE_RENDER
// Render
#include "render/Color.hpp"
#include "render/Context.hpp"
#include "render/GridColor.hpp"
#endif

// Library
#include "wrapper.hpp"

/* ************************************************************************ */

using namespace module::python;

/* ************************************************************************ */

static void python_wrapper_render_Color(PyObject* module)
{
#if ENABLE_RENDER

#endif
}

/* ************************************************************************ */

static PyObject* Context_matrixPush(ObjectWrapper<render::Context>* self)
{
    assert(self);
    assert(self->ptr);

    // Get value
    self->ptr->matrixPush();

    // Return value
    return Py_BuildValue("");
}

/* ************************************************************************ */

static PyObject* Context_matrixPop(ObjectWrapper<render::Context>* self)
{
    assert(self);
    assert(self->ptr);

    // Get value
    self->ptr->matrixPop();

    // Return value
    return Py_BuildValue("");
}

/* ************************************************************************ */

static PyObject* Context_matrixIdentity(ObjectWrapper<render::Context>* self)
{
    assert(self);
    assert(self->ptr);

    // Get value
    self->ptr->matrixIdentity();

    // Return value
    return Py_BuildValue("");
}

/* ************************************************************************ */

static PyMethodDef g_contextMethods[] = {
    {"matrixPush", (PyCFunction) Context_matrixPush, METH_NOARGS, nullptr},
    {"matrixPop", (PyCFunction) Context_matrixPop, METH_NOARGS, nullptr},
    {"matrixIdentity", (PyCFunction) Context_matrixIdentity, METH_NOARGS, nullptr},
    {nullptr}  /* Sentinel */
};

/* ************************************************************************ */

static void python_wrapper_render_Context(PyObject* module)
{
#if ENABLE_RENDER
    TypeDefinition<render::Context>::init("render.Context");
    TypeDefinition<render::Context>::definition.tp_methods = g_contextMethods;
    TypeDefinition<render::Context>::ready();

    Py_INCREF(&TypeDefinition<render::Context>::definition);
    PyModule_AddObject(module, "Context", reinterpret_cast<PyObject*>(&TypeDefinition<render::Context>::definition));
#endif
}

/* ************************************************************************ */

static void python_wrapper_render_GridColor(PyObject* module)
{
#if ENABLE_RENDER

#endif
}

/* ************************************************************************ */

void python_wrapper_render()
{
    PyObject* module = Py_InitModule3("render", nullptr, nullptr);

    python_wrapper_render_Color(module);
    python_wrapper_render_Context(module);
    python_wrapper_render_GridColor(module);
}

/* ************************************************************************ */

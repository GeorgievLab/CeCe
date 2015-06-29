/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */

// Python requires to be included first because it sets some parameters to stdlib
#include "Python.hpp"

/* ************************************************************************ */

// Declaration
#include "wrapper_render.hpp"

#if ENABLE_RENDER
// Render
#include "render/Color.hpp"
#include "render/Context.hpp"
#include "render/GridColor.hpp"
#endif

// Module
#include "wrapper.hpp"

/* ************************************************************************ */

using namespace plugin::python;

/* ************************************************************************ */

static void python_wrapper_render_Color(PyObject* module)
{
#if ENABLE_RENDER

#endif
}

/* ************************************************************************ */

static void python_wrapper_render_Context(PyObject* module)
{
#if ENABLE_RENDER
    using type = render::Context*;
    using type_def = TypeDefinition<type>;

    static PyMethodDef fns[] = {
        defineMemberFunction<1, type>("matrixPush", &render::Context::matrixPush),
        defineMemberFunction<2, type>("matrixPop", &render::Context::matrixPop),
        defineMemberFunction<3, type>("matrixIdentity", &render::Context::matrixIdentity),
        {NULL}  /* Sentinel */
    };

    type_def::init("render.Context");
    type_def::definition.tp_methods = fns;
    type_def::ready();

    Py_INCREF(&type_def::definition);
    PyModule_AddObject(module, "Context", reinterpret_cast<PyObject*>(&type_def::definition));
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

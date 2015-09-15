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
    type_def::finish(module, "Context");
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

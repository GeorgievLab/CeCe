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

// This must be first
#include "cece/plugins/python/Python.hpp"

// C++
#include <iostream>

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace python {

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

/**
 * @brief Write Python string object to std::cout.
 *
 * @param self
 * @param args
 *
 * @return
 */
PyObject* log_write(PyObject* self, PyObject* args)
{
    const char* what;

    if (!PyArg_ParseTuple(args, "s", &what))
        return nullptr;

    if (!strcmp(what, "\n"))
        std::cout << std::endl;
    else
        std::cout << what;

    // Return None
    Py_RETURN_NONE;
}

/* ************************************************************************ */

static const PyMethodDef log_methods[] = {
    {"write", log_write, METH_VARARGS, "Write string to std::cout."},
    {NULL, NULL, 0, NULL}
};

/* ************************************************************************ */

}

/* ************************************************************************ */

void init_stdout(void)
{
    PyObject* module = Py_InitModule("cppout", const_cast<PyMethodDef*>(log_methods));

    if (module == nullptr)
        return;

    PySys_SetObject(const_cast<char*>("stdout"), module);
}

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */

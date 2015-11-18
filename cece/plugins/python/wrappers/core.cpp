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
#include "cece/plugins/python/Python.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace python {

/* ************************************************************************ */

extern PyMethodDef core_functions[];

/* ************************************************************************ */

void init_core_VectorInt(PyObject* module);
void init_core_VectorUint(PyObject* module);
void init_core_VectorFloat(PyObject* module);
void init_core_VectorPosition(PyObject* module);
void init_core_VectorVelocity(PyObject* module);
void init_core_VectorForce(PyObject* module);
void init_core_VectorAcceleration(PyObject* module);

/* ************************************************************************ */

void init_core(void)
{
    PyObject* module = Py_InitModule("core", core_functions);

    init_core_VectorInt(module);
    init_core_VectorUint(module);
    init_core_VectorFloat(module);
    init_core_VectorPosition(module);
    init_core_VectorVelocity(module);
    init_core_VectorForce(module);
    init_core_VectorAcceleration(module);
}

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */

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
#include "Python.hpp"

// Declaration
#include "wrapper_core.hpp"

// Core
#include "core/Vector.hpp"
#include "core/Units.hpp"

// Module
#include "wrapper.hpp"
#include "Exception.hpp"

/* ************************************************************************ */

using namespace plugin::python;

/* ************************************************************************ */

template<typename T>
static void python_wrapper_core_Vector(const char* fullname, PyObject* module)
{
    const char* name = fullname + 5;

    using type = Vector<T>;
    using type_def = TypeDefinition<type>;

    static PyGetSetDef properties[] = {
        defineProperty<1, type>("x", &type::getX, &type::setX),
        defineProperty<2, type>("y", &type::getY, &type::setY),
        defineProperty<3, type>("width", &type::getWidth),
        defineProperty<4, type>("height", &type::getHeight),
        {NULL}  /* Sentinel */
    };

    type_def::init(fullname);
    type_def::definition.tp_init = Constructor<type, T, T>::to();
    type_def::definition.tp_getset = properties;
    type_def::ready();
    type_def::finish(module, name);
}

/* ************************************************************************ */

void python_wrapper_core()
{
    PyObject* module = Py_InitModule3("core", nullptr, nullptr);

    python_wrapper_core_Vector<float>("core.VectorFloat", module);
    python_wrapper_core_Vector<int>("core.VectorInt", module);
    python_wrapper_core_Vector<unsigned int>("core.VectorUint", module);
    python_wrapper_core_Vector<units::Length>("core.PositionVector", module);
    python_wrapper_core_Vector<units::Velocity>("core.VelocityVector", module);
}

/* ************************************************************************ */

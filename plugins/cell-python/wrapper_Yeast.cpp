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
#include "plugins/python/Python.hpp"

/* ************************************************************************ */

// Declaration
#include "wrapper_Yeast.hpp"

// Python
#include "plugins/python/wrapper.hpp"

// Cell plugin
#include "plugins/cell/Yeast.hpp"

/* ************************************************************************ */

using namespace plugin::python;
using namespace plugin::cell;

/* ************************************************************************ */

void python_wrapper_cell_Yeast()
{
    PyObject* module = Py_InitModule3("cell", nullptr, nullptr);

    using type = Yeast*;
    using type_def = TypeDefinition<type>;

    type_def::init("cell.Yeast", "cell.CellBase");
    type_def::ready();
    type_def::finish(module, "Yeast");

    // Register dynamic type
    registerDynamic(typeid(Yeast), &type_def::definition);
}

/* ************************************************************************ */

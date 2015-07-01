/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
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

    Py_INCREF(&type_def::definition);
    PyModule_AddObject(module, "Yeast", reinterpret_cast<PyObject*>(&type_def::definition));

    // Register dynamic type
    registerDynamic(typeid(Yeast), &type_def::definition);
}

/* ************************************************************************ */

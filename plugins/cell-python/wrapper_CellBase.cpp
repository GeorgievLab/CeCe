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
#include "wrapper_CellBase.hpp"

// Python
#include "plugins/python/wrapper.hpp"

// Cell plugin
#include "plugins/cell/CellBase.hpp"

/* ************************************************************************ */

using namespace plugin::python;
using namespace plugin::cell;

/* ************************************************************************ */

void python_wrapper_cell_CellBase()
{
    PyObject* module = Py_InitModule3("cell", nullptr, nullptr);

    using type = CellBase*;
    using type_def = TypeDefinition<type>;

    static PyGetSetDef properties[] = {
        defineProperty<1, type>("volume", &CellBase::getVolume, &CellBase::setVolume),
        defineProperty<2, type>("growthRate", &CellBase::getGrowthRate, &CellBase::setGrowthRate),
        {NULL}  /* Sentinel */
    };

    static PyMethodDef fns[] = {
        defineMemberFunction<1, type>("moleculeCount", &CellBase::getMoleculeCount),
        defineMemberFunction<2, type>("kill", &CellBase::kill),
        {NULL}  /* Sentinel */
    };

    type_def::init("cell.CellBase", "simulator.Object");
    type_def::definition.tp_getset = properties;
    type_def::definition.tp_methods = fns;
    type_def::ready();
    type_def::finish(module, "CellBase");

    // Register dynamic type
    registerDynamic(typeid(CellBase), &type_def::definition);
}

/* ************************************************************************ */

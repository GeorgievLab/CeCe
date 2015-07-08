/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */

// Python requires to be included first because it sets some parameters to stdlib
#include "../python/Python.hpp"

/* ************************************************************************ */

// Declaration
#include "wrapper_module.hpp"

// Python
#include "plugins/python/wrapper.hpp"

// Streamlines
#include "plugins/diffusion/Module.hpp"

/* ************************************************************************ */

using namespace plugin::python;

/* ************************************************************************ */

void python_wrapper_module()
{
    PyObject* module = Py_InitModule3("diffusion", nullptr, nullptr);

    using type = plugin::diffusion::Module;
    using type_ptr = type*;
    using type_def = TypeDefinition<type_ptr>;

    static PyGetSetDef properties[] = {
        defineProperty<1, type_ptr>("signalCount", &type::getSignalCount),
        //defineProperty<2, type_ptr>("diffusionRate", &type::getDiffusionRate, &type::setDiffusionRate),
        {NULL}  /* Sentinel */
    };

    static PyMethodDef fns[] = {
        defineMemberFunction<1, type_ptr>("getSignalId", &type::getSignalId),
        //defineMemberFunction<2, type_ptr>("getSignal", +[](type_ptr ptr, type::SignalId id, int x, int y) {
        //    return ptr->getSignal(id, type::Coordinate(x, y));
        //}),
        {NULL}  /* Sentinel */
    };

    type_def::init("diffusion.Module", "simulator.Module");
    type_def::definition.tp_getset = properties;
    type_def::definition.tp_methods = fns;
    type_def::ready();

    Py_INCREF(&type_def::definition);
    PyModule_AddObject(module, "Module", reinterpret_cast<PyObject*>(&type_def::definition));

    // Register dynamic type
    registerDynamic(typeid(type), &type_def::definition);
}

/* ************************************************************************ */

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
#include "../python/wrapper.hpp"

// Streamlines
#include "../streamlines/Module.hpp"

/* ************************************************************************ */

using namespace module::python;

/* ************************************************************************ */

void python_wrapper_module()
{
    // Get simulator module
    auto simulatorModule = makeHandle(PyImport_ImportModule("simulator"));
    auto dict = PyModule_GetDict(simulatorModule);
    auto moduleClass = makeHandle(PyMapping_GetItemString(dict, "Module"));
    assert(PyType_Check(moduleClass));

    PyObject* module = Py_InitModule3("streamlines", nullptr, nullptr);

    using type = module::streamlines::Module*;
    using type_def = TypeDefinition<type>;

    static PyGetSetDef properties[] = {
        defineProperty<1, type>("velocityInflow", &module::streamlines::Module::getVelocityInflow, &module::streamlines::Module::setVelocityInflow),
        {NULL}  /* Sentinel */
    };

    type_def::init("streamlines.Module");
    type_def::definition.tp_getset = properties;
    type_def::definition.tp_base = (PyTypeObject*) moduleClass.release();
    type_def::ready();

    Py_INCREF(&type_def::definition);
    PyModule_AddObject(module, "Module", reinterpret_cast<PyObject*>(&type_def::definition));

    // Register dynamic type
    registerDynamic(typeid(module::streamlines::Module), &type_def::definition);
}

/* ************************************************************************ */

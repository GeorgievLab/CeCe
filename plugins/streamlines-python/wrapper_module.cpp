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

using namespace plugin::python;

/* ************************************************************************ */

static PyObject* setLayout(ObjectWrapper<plugin::streamlines::Module*>* self, PyObject* args, void*) noexcept
{
    PyObject* array;

    if (!PyArg_ParseTuple(args, "O", &array))
        return nullptr;

    // Get array iterator
    auto iter = makeHandle(PyObject_GetIter(array));

    if (!iter)
    {
        PyErr_SetString(PyExc_RuntimeError, "Not an array");
        return nullptr;
    }

    plugin::streamlines::Module::Layout layout;

    for (auto& item : layout)
    {
        auto next = makeHandle(PyIter_Next(iter));
        if (!next)
        {
            PyErr_SetString(PyExc_RuntimeError, "Missing layout specifier(s)");
            return nullptr;
        }

        InStringStream iss(cast<String>(next));
        iss >> item;
    }

    // Set layout
    assert(self->value);
    self->value->setLayout(layout);

    // Return None
    Py_RETURN_NONE;
}

/* ************************************************************************ */

void python_wrapper_module()
{
    PyObject* module = Py_InitModule3("streamlines", nullptr, nullptr);

    using type = plugin::streamlines::Module*;
    using type_def = TypeDefinition<type>;
/*
    static PyGetSetDef properties[] = {
        defineProperty<1, type>("velocityInflow", &plugin::streamlines::Module::getVelocityInflow, &plugin::streamlines::Module::setVelocityInflow),
        {NULL}
    };
*/

    static PyMethodDef fns[] = {
        {"setLayout", (PyCFunction) setLayout, METH_VARARGS, "Set streamlines module layout."},
        {NULL}  /* Sentinel */
    };

    type_def::init("streamlines.Module", "simulator.Module");
    //type_def::definition.tp_getset = properties;
    type_def::definition.tp_methods = fns;
    type_def::ready();

    Py_INCREF(&type_def::definition);
    PyModule_AddObject(module, "Module", reinterpret_cast<PyObject*>(&type_def::definition));

    // Register dynamic type
    registerDynamic(typeid(plugin::streamlines::Module), &type_def::definition);
}

/* ************************************************************************ */

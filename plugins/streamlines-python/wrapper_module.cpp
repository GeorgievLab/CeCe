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
    using namespace plugin::streamlines;

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

    Module::Layout layout;
    StaticArray<Module::LayoutPosition, Module::LayoutPosCount> order{{
        Module::LayoutPosTop,
        Module::LayoutPosRight,
        Module::LayoutPosBottom,
        Module::LayoutPosLeft
    }};

    for (auto pos : order)
    {
        auto next = makeHandle(PyIter_Next(iter));
        if (!next)
        {
            PyErr_SetString(PyExc_RuntimeError, "Missing layout specifier(s)");
            return nullptr;
        }

        InStringStream iss(cast<String>(next));
        iss >> layout[pos];
    }

    // Set layout
    assert(self->value);
    self->value->setLayout(layout);

    // Return None
    Py_RETURN_NONE;
}

/* ************************************************************************ */

static PyObject* initBarriers(ObjectWrapper<plugin::streamlines::Module*>* self, PyObject* args, void*) noexcept
{
    PyObject* simulation;

    if (!PyArg_ParseTuple(args, "O", &simulation))
        return nullptr;

    if (PyObject_TypeCheck(simulation, &TypeDefinition<simulator::Simulation*>::definition) <= 0)
    {
        PyErr_SetString(PyExc_RuntimeError, "Argument is not simulator.Simulation");
        return nullptr;
    }

    auto sim = reinterpret_cast<ObjectWrapper<simulator::Simulation*>*>(simulation);

    // Init barriers
    self->value->initBarriers(*sim->value);

    // Return None
    Py_RETURN_NONE;
}

/* ************************************************************************ */

void python_wrapper_module()
{
    PyObject* module = Py_InitModule3("streamlines", nullptr, nullptr);

    using type = plugin::streamlines::Module;
    using type_ptr = type*;
    using type_def = TypeDefinition<type_ptr>;
/*
    static PyGetSetDef properties[] = {
        defineProperty<1, type_ptr>("inletVelocity", &type::getInletVelocity, &type::setInletVelocity),
        {NULL}
    };
*/
    static PyMethodDef fns[] = {
        {"setLayout", (PyCFunction) setLayout, METH_VARARGS, "Set streamlines module layout."},
        {"initBarriers", (PyCFunction) initBarriers, METH_VARARGS, "Reinit barriers."},
        defineMemberFunction<1, type_ptr>("setInletVelocity", &type::setInletVelocity),
        {NULL}  /* Sentinel */
    };

    type_def::init("streamlines.Module", "simulator.Module");
    //type_def::definition.tp_getset = properties;
    type_def::definition.tp_methods = fns;
    type_def::ready();
    type_def::finish(module, "Module");

    // Register dynamic type
    registerDynamic(typeid(plugin::streamlines::Module), &type_def::definition);

    // Define constants
    PyModule_AddIntConstant(module, "LEFT", static_cast<int>(type::LayoutPosLeft));
    PyModule_AddIntConstant(module, "RIGHT", static_cast<int>(type::LayoutPosRight));
    PyModule_AddIntConstant(module, "TOP", static_cast<int>(type::LayoutPosTop));
    PyModule_AddIntConstant(module, "BOTTOM", static_cast<int>(type::LayoutPosBottom));
}

/* ************************************************************************ */

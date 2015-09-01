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
#include "plugins/python/Utils.hpp"

// Streamlines
#include "plugins/diffusion/Module.hpp"

/* ************************************************************************ */

using namespace plugin::python;

/* ************************************************************************ */

static PyObject* get_signal(ObjectWrapper<plugin::diffusion::Module*>* self, PyObject* args, void*) noexcept
{
    int id;
    int x;
    int y;

    if(!PyArg_ParseTuple(args, "iii", &id, &x, &y))
        return NULL;

    const auto value = self->value->getSignal(id, plugin::diffusion::Module::Coordinate(x, y));

    return Py_BuildValue("f", value.value());
}

/* ************************************************************************ */

static PyObject* set_signal(ObjectWrapper<plugin::diffusion::Module*>* self, PyObject* args, void*) noexcept
{
    int id;
    int x;
    int y;
    RealType value;

    if(!PyArg_ParseTuple(args, "iiif", &id, &x, &y, &value))
        return NULL;

    // Set signal value
    self->value->setSignal(
        id,
        plugin::diffusion::Module::Coordinate(x, y),
        plugin::diffusion::Module::SignalConcentration(value)
    );

    Py_RETURN_NONE;
}

/* ************************************************************************ */

void python_wrapper_module()
{
    PyObject* module = Py_InitModule3("diffusion", nullptr, nullptr);

    using type = plugin::diffusion::Module;
    using type_ptr = type*;
    using type_def = TypeDefinition<type_ptr>;

    static PyGetSetDef properties[] = {
        defineProperty<1, type_ptr>("signalCount", &type::getSignalCount),
        defineProperty<2, type_ptr>("gridSize", &type::getGridSize),
        //defineProperty<2, type_ptr>("diffusionRate", &type::getDiffusionRate, &type::setDiffusionRate),
        {NULL}  /* Sentinel */
    };

    static PyMethodDef fns[] = {
        defineMemberFunction<1, type_ptr>("getSignalId", &type::getSignalId),
        {"getSignal", (PyCFunction) get_signal, METH_VARARGS, NULL},
        {"setSignal", (PyCFunction) set_signal, METH_VARARGS, NULL},
        //defineMemberFunction<2, type_ptr>("getSignal", +[](type_ptr ptr, type::SignalId id, int x, int y) {
        //    return ptr->getSignal(id, type::Coordinate(x, y));
        //}),
        {NULL}  /* Sentinel */
    };

    type_def::init("diffusion.Module", "simulator.Module");
    type_def::definition.tp_getset = properties;
    type_def::definition.tp_methods = fns;
    type_def::ready();
    type_def::finish(module, "Module");

    // Register dynamic type
    registerDynamic(typeid(type), &type_def::definition);
}

/* ************************************************************************ */


/* ************************************************************************ */

// Declaration
#include "wrapper_simulator.hpp"

// C++
#include <cassert>

// Python
#include <Python.h>

// Simulation
#include "simulator/Simulation.hpp"
#include "simulator/Configuration.hpp"

// Library
#include "wrapper.hpp"

/* ************************************************************************ */

using namespace module::python;

/* ************************************************************************ */

/**
 * @brief Configuration::getString function.
 *
 * @param self
 * @param args
 * @param kwds
 *
 * @return
 */
static PyObject* Configuration_get(ObjectWrapper<simulator::ConfigurationBase>* self, PyObject* args)
{
    char* name;

    // Get argument
    if (!PyArg_ParseTuple(args, "s", &name))
        return nullptr;

    assert(self);
    assert(self->ptr);
    assert(name);

    // Get value
    auto value = self->ptr->getString(name);

    // Return value
    return Py_BuildValue("s#", value.c_str(), value.length());
}

/* ************************************************************************ */

static PyMethodDef g_configurationMethods[] = {
    {"get", (PyCFunction) Configuration_get, METH_VARARGS, nullptr},
    {nullptr}  /* Sentinel */
};

/* ************************************************************************ */

static void python_wrapper_simulator_Configuration(PyObject* module)
{
    TypeDefinition<simulator::ConfigurationBase>::init("simulator.Configuration");
    TypeDefinition<simulator::ConfigurationBase>::definition.tp_methods = g_configurationMethods;
    TypeDefinition<simulator::ConfigurationBase>::ready();

    Py_INCREF(&TypeDefinition<simulator::ConfigurationBase>::definition);
    PyModule_AddObject(module, "Configuration", reinterpret_cast<PyObject*>(&TypeDefinition<simulator::ConfigurationBase>::definition));
}

/* ************************************************************************ */

static PyObject* simulation_getWorldSize(ObjectWrapper<simulator::Simulation>* self, void* closure)
{
    // TODO: implement
    return nullptr;
}

/* ************************************************************************ */

static PyObject* simulation_getObjectsCount(ObjectWrapper<simulator::Simulation>* self, void* closure)
{
    assert(self);
    assert(self->ptr);
    const unsigned long count = self->ptr->getObjects().size();

    return Py_BuildValue("k", count);
}

/* ************************************************************************ */

static PyGetSetDef g_simulationGettersSeters[] = {
    {"worldSize", (getter)simulation_getWorldSize, nullptr, nullptr, nullptr },
    {"objectsCount", (getter)simulation_getObjectsCount, nullptr, nullptr, nullptr },
    {NULL}  /* Sentinel */
};

/* ************************************************************************ */

static void python_wrapper_simulator_Simulation(PyObject* module)
{
    TypeDefinition<simulator::Simulation>::init("simulator.Simulation");
    TypeDefinition<simulator::Simulation>::definition.tp_getset = g_simulationGettersSeters;
    TypeDefinition<simulator::Simulation>::ready();

    Py_INCREF(&TypeDefinition<simulator::Simulation>::definition);
    PyModule_AddObject(module, "Simulation", reinterpret_cast<PyObject*>(&TypeDefinition<simulator::Simulation>::definition));
}

/* ************************************************************************ */

void python_wrapper_simulator()
{
    PyObject* module = Py_InitModule3("simulator", nullptr, nullptr);

    python_wrapper_simulator_Configuration(module);
    python_wrapper_simulator_Simulation(module);
}

/* ************************************************************************ */

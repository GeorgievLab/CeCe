/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */

// Python requires to be included first because it sets some parameters to stdlib
#include "Python.hpp"

/* ************************************************************************ */

// Declaration
#include "wrapper_simulator.hpp"

// C++
#include <cassert>

// Simulation
#include "simulator/Simulation.hpp"
#include "simulator/Configuration.hpp"

// Module
#include "wrapper.hpp"
#include "Utils.hpp"

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
static PyObject* Configuration_get(ObjectWrapper<simulator::Configuration*>* self, PyObject* args)
{
    char* name;

    // Get argument
    if (!PyArg_ParseTuple(args, "s", &name))
        return nullptr;

    assert(self);
    assert(self->value);
    assert(name);

    // Get value
    auto value = self->value->getString(name);

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
    using type_def = TypeDefinition<simulator::Configuration*>;

    type_def::init("simulator.Configuration");
    type_def::definition.tp_methods = g_configurationMethods;
    type_def::ready();

    Py_INCREF(&type_def::definition);
    PyModule_AddObject(module, "Configuration", reinterpret_cast<PyObject*>(&type_def::definition));
}

/* ************************************************************************ */

static PyObject* simulation_getWorldSize(ObjectWrapper<simulator::Simulation*>* self, void* closure)
{
    assert(self);
    assert(self->value);
    const Vector<float> pos = self->value->getWorldSize();

    return makeObject(pos);
}

/* ************************************************************************ */

static PyObject* simulation_getObjectsCount(ObjectWrapper<simulator::Simulation*>* self, void* closure)
{
    assert(self);
    assert(self->value);
    const unsigned long count = self->value->getObjects().size();

    return Py_BuildValue("k", count);
}

/* ************************************************************************ */

static PyObject* simulation_getIteration(ObjectWrapper<simulator::Simulation*>* self, void* closure)
{
    assert(self);
    assert(self->value);
    const unsigned long step = self->value->getStepNumber();

    return Py_BuildValue("k", step);
}

/* ************************************************************************ */

static PyObject* simulation_getIterations(ObjectWrapper<simulator::Simulation*>* self, void* closure)
{
    assert(self);
    assert(self->value);
    const unsigned long step = self->value->getIterations();

    return Py_BuildValue("k", step);
}

/* ************************************************************************ */

static PyGetSetDef g_simulationGettersSeters[] = {
    {const_cast<char*>("worldSize"), (getter)simulation_getWorldSize, nullptr, nullptr, nullptr },
    {const_cast<char*>("objectsCount"), (getter)simulation_getObjectsCount, nullptr, nullptr, nullptr },
    {const_cast<char*>("iteration"), (getter)simulation_getIteration, nullptr, nullptr, nullptr },
    {const_cast<char*>("iterations"), (getter)simulation_getIterations, nullptr, nullptr, nullptr },
    {NULL}  /* Sentinel */
};

/* ************************************************************************ */

static void python_wrapper_simulator_Simulation(PyObject* module)
{
    using type_def = TypeDefinition<simulator::Simulation*>;

    type_def::init("simulator.Simulation");
    type_def::definition.tp_getset = g_simulationGettersSeters;
    type_def::ready();

    Py_INCREF(&type_def::definition);
    PyModule_AddObject(module, "Simulation", reinterpret_cast<PyObject*>(&type_def::definition));
}

/* ************************************************************************ */

static PyObject* object_getId(ObjectWrapper<simulator::Object*>* self, void* closure)
{
    assert(self);
    assert(self->value);
    const unsigned long count = self->value->getId();

    return Py_BuildValue("k", count);
}

/* ************************************************************************ */

static PyObject* object_getPosition(ObjectWrapper<simulator::Object*>* self, void* closure)
{
    assert(self);
    assert(self->value);
    const Vector<float> pos = self->value->getPosition();

    return makeObject(pos);
}

/* ************************************************************************ */

static int object_setPosition(ObjectWrapper<simulator::Object*>* self, PyObject* value, void* closure)
{
    if (value == nullptr)
    {
        PyErr_SetString(PyExc_TypeError, "Value cannot be NULL");
        return -1;
    }

    if (!PyObject_TypeCheck(value, &TypeDefinition<Vector<float>>::definition))
    {
        PyErr_SetString(PyExc_TypeError, "Position must have VectorFloat type");
        return -1;
    }

    using value_type = TypeDefinition<Vector<float>>::type;
    value_type* val = reinterpret_cast<value_type*>(value);

    assert(self);
    assert(self->value);
    self->value->setPosition(val->value);

    return 0;
}

/* ************************************************************************ */

static PyObject* object_getRotation(ObjectWrapper<simulator::Object*>* self, void* closure)
{
    assert(self);
    assert(self->value);
    const float rotation = self->value->getRotation();

    return Py_BuildValue("f", rotation);
}

/* ************************************************************************ */

static int object_setRotation(ObjectWrapper<simulator::Object*>* self, PyObject* value, void* closure)
{
    if (value == nullptr)
    {
        PyErr_SetString(PyExc_TypeError, "Value cannot be NULL");
        return -1;
    }

    if (!PyFloat_Check(value))
    {
        PyErr_SetString(PyExc_TypeError, "Value must be float");
        return -1;
    }

    assert(self);
    assert(self->value);
    self->value->setRotation(PyFloat_AsDouble(value));

    return 0;
}

/* ************************************************************************ */

static PyObject* object_getVelocity(ObjectWrapper<simulator::Object*>* self, void* closure)
{
    assert(self);
    assert(self->value);
    const Vector<float> velocity = self->value->getVelocity();

    return makeObject(velocity);
}

/* ************************************************************************ */

static int object_setVelocity(ObjectWrapper<simulator::Object*>* self, PyObject* value, void* closure)
{
    if (!PyObject_TypeCheck(value, &TypeDefinition<Vector<float>>::definition))
    {
        PyErr_SetString(PyExc_TypeError, "Position must have VectorFloat type");
        return -1;
    }

    using value_type = TypeDefinition<Vector<float>>::type;
    value_type* val = reinterpret_cast<value_type*>(value);

    assert(self);
    assert(self->value);
    self->value->setVelocity(val->value);

    return 0;
}

/* ************************************************************************ */

static PyGetSetDef g_objectGettersSeters[] = {
    {const_cast<char*>("id"), (getter)object_getId, nullptr, nullptr, nullptr },
    {const_cast<char*>("position"), (getter)object_getPosition, (setter)object_setPosition, nullptr, nullptr },
    {const_cast<char*>("rotation"), (getter)object_getRotation, (setter)object_setRotation, nullptr, nullptr },
    {const_cast<char*>("velocity"), (getter)object_getVelocity, (setter)object_setVelocity, nullptr, nullptr },
    {NULL}  /* Sentinel */
};

/* ************************************************************************ */

static void python_wrapper_simulator_Object(PyObject* module)
{
    using type_def = TypeDefinition<simulator::Object*>;

    type_def::init("simulator.Object");
    type_def::definition.tp_getset = g_objectGettersSeters;
    type_def::ready();

    Py_INCREF(&type_def::definition);
    PyModule_AddObject(module, "Object", reinterpret_cast<PyObject*>(&type_def::definition));
}

/* ************************************************************************ */

void python_wrapper_simulator()
{
    PyObject* module = Py_InitModule3("simulator", nullptr, nullptr);

    python_wrapper_simulator_Configuration(module);
    python_wrapper_simulator_Simulation(module);
    python_wrapper_simulator_Object(module);
}

/* ************************************************************************ */

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

static void python_wrapper_simulator_Configuration(PyObject* module)
{
    using type = const simulator::Configuration*;
    using type_def = TypeDefinition<type>;

    static PyMethodDef fns[] = {
        defineMemberFunction<1, type>("get", &simulator::Configuration::getString),
        {NULL}  /* Sentinel */
    };

    type_def::init("simulator.Configuration");
    type_def::definition.tp_methods = fns;
    type_def::ready();

    Py_INCREF(&type_def::definition);
    PyModule_AddObject(module, "Configuration", reinterpret_cast<PyObject*>(&type_def::definition));
}

/* ************************************************************************ */

static void python_wrapper_simulator_Simulation(PyObject* module)
{
    using type = simulator::Simulation*;
    using type_def = TypeDefinition<type>;

    static PyGetSetDef properties[] = {
        defineProperty<1, type>("worldSize", &simulator::Simulation::getWorldSize),
        defineProperty<2, type>("iteration", &simulator::Simulation::getStepNumber),
        defineProperty<3, type>("iterations", &simulator::Simulation::getIterations),
        defineProperty<4, type>("objectsCount", +[](type obj) {
            return obj->getObjects().size();
        }),
        {NULL}  /* Sentinel */
    };

    type_def::init("simulator.Simulation");
    type_def::definition.tp_getset = properties;
    type_def::ready();

    Py_INCREF(&type_def::definition);
    PyModule_AddObject(module, "Simulation", reinterpret_cast<PyObject*>(&type_def::definition));
}

/* ************************************************************************ */

static void python_wrapper_simulator_Object(PyObject* module)
{
    using type = simulator::Object*;
    using type_def = TypeDefinition<type>;

    static PyGetSetDef properties[] = {
        defineProperty<1, type>("id", &simulator::Object::getId),
        defineProperty<2, type>("position", &simulator::Object::getPosition, &simulator::Object::setPosition),
        defineProperty<3, type>("rotation", &simulator::Object::getRotation, &simulator::Object::setRotation),
        defineProperty<4, type>("velocity", &simulator::Object::getVelocity, &simulator::Object::setVelocity),
        {NULL}  /* Sentinel */
    };

    type_def::init("simulator.Object");
    type_def::definition.tp_getset = properties;
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

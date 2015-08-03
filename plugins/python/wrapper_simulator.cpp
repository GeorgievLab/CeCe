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
#include "simulator/Object.hpp"
#include "simulator/Module.hpp"
#include "simulator/Configuration.hpp"

// Module
#include "wrapper.hpp"
#include "Utils.hpp"

/* ************************************************************************ */

using namespace plugin::python;

/* ************************************************************************ */

static void python_wrapper_simulator_Configuration(PyObject* module)
{
    using type = const simulator::Configuration*;
    using type_def = TypeDefinition<type>;

    static PyMethodDef fns[] = {
        defineMemberFunction<1, type>("get",
            static_cast<String (simulator::Configuration::*)(const StringView&) const>(&simulator::Configuration::get)),
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
        defineProperty<2, type>("iteration", &simulator::Simulation::getIteration),
        defineProperty<3, type>("iterations", &simulator::Simulation::getIterations),
        defineProperty<4, type>("timeStep", &simulator::Simulation::getTimeStep),
        defineProperty<5, type>("totalTime", &simulator::Simulation::getTotalTime),
        defineProperty<6, type>("objectsCount", +[](type obj) {
            return obj->getObjects().size();
        }),
        {NULL}  /* Sentinel */
    };

    static PyMethodDef fns[] = {
        defineMemberFunction<1, type>("useModule",
            static_cast<simulator::Module*(simulator::Simulation::*)(const String&)>(&simulator::Simulation::useModule)
        ),
        defineMemberFunction<2, type>("buildObject", &simulator::Simulation::buildObject),
        {NULL}  /* Sentinel */
    };

    type_def::init("simulator.Simulation");
    type_def::definition.tp_getset = properties;
    type_def::definition.tp_methods = fns;
    type_def::ready();

    Py_INCREF(&type_def::definition);
    PyModule_AddObject(module, "Simulation", reinterpret_cast<PyObject*>(&type_def::definition));
}

/* ************************************************************************ */

static void python_wrapper_simulator_Module(PyObject* module)
{
    using type = simulator::Module*;
    using type_def = TypeDefinition<type>;

    type_def::init("simulator.Module");
    type_def::ready();

    Py_INCREF(&type_def::definition);
    PyModule_AddObject(module, "Module", reinterpret_cast<PyObject*>(&type_def::definition));
}

/* ************************************************************************ */

static PyObject* useProgram(ObjectWrapper<simulator::Object*>* self, PyObject* args, void*) NOEXCEPT
{
    char* name;

    if(!PyArg_ParseTuple(args, "s", &name))
        return NULL;

    // Add program
    self->value->addProgram(self->value->getSimulation().getProgram(name));

    return Py_BuildValue("");
}

/* ************************************************************************ */

static void python_wrapper_simulator_Object(PyObject* module)
{
    using type_base = simulator::Object;
    using type = type_base*;
    using type_def = TypeDefinition<type>;

    static PyGetSetDef properties[] = {
        defineProperty<1, type>("id", &type_base::getId),
        defineProperty<2, type>("position", &type_base::getPosition, &type_base::setPosition),
        defineProperty<3, type>("rotation", &type_base::getRotation, &type_base::setRotation),
        defineProperty<4, type>("velocity", &type_base::getVelocity, &type_base::setVelocity),
        {NULL}  /* Sentinel */
    };

    static PyMethodDef fns[] = {
        {"useProgram", (PyCFunction) useProgram, METH_VARARGS, NULL},
        defineMemberFunction<1, type>("destroy", &type_base::destroy),
        {NULL}  /* Sentinel */
    };

    type_def::init("simulator.Object");
    type_def::definition.tp_getset = properties;
    type_def::definition.tp_methods = fns;
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
    python_wrapper_simulator_Module(module);
    python_wrapper_simulator_Object(module);
}

/* ************************************************************************ */

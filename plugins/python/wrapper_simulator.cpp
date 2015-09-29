/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/*                                                                          */
/* This file is part of CeCe.                                               */
/*                                                                          */
/* CeCe is free software: you can redistribute it and/or modify             */
/* it under the terms of the GNU General Public License as published by     */
/* the Free Software Foundation, either version 3 of the License, or        */
/* (at your option) any later version.                                      */
/*                                                                          */
/* CeCe is distributed in the hope that it will be useful,                  */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of           */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            */
/* GNU General Public License for more details.                             */
/*                                                                          */
/* You should have received a copy of the GNU General Public License        */
/* along with CeCe.  If not, see <http://www.gnu.org/licenses/>.            */
/*                                                                          */
/* ************************************************************************ */

// This must be first
#include "Python.hpp"

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
    type_def::finish(module, "Configuration");
}

/* ************************************************************************ */

static PyObject* getObject(ObjectWrapper<simulator::Simulation*>* self, PyObject* args, void*) noexcept
{
    int i;

    if(!PyArg_ParseTuple(args, "i", &i))
        return NULL;

    // Get object
    auto object = self->value->getObjects()[i].get();

    return plugin::python::makeObject(object);
}

/* ************************************************************************ */

static void python_wrapper_simulator_Simulation(PyObject* module)
{
    using type = simulator::Simulation;
    using type_ptr = simulator::Simulation*;
    using type_def = TypeDefinition<type_ptr>;

    static PyGetSetDef properties[] = {
        defineProperty<1, type_ptr>("worldSize", &type::getWorldSize),
        defineProperty<2, type_ptr>("iteration", &type::getIteration),
        defineProperty<3, type_ptr>("iterations", &type::getIterations),
        defineProperty<4, type_ptr>("timeStep", &type::getTimeStep, &type::setTimeStep),
        defineProperty<5, type_ptr>("totalTime", &type::getTotalTime),
        defineProperty<6, type_ptr>("objectCount", &type::getObjectCount),
        {NULL}  /* Sentinel */
    };

    static PyMethodDef fns[] = {
        defineMemberFunction<1, type_ptr>("useModule",
            static_cast<simulator::Module*(type::*)(const String&)>(&type::useModule)
        ),
        defineMemberFunction<2, type_ptr>("buildObject", &type::buildObject),
        defineMemberFunction<3, type_ptr>("objectCountType", &type::getObjectCountType),
        defineMemberFunction<4, type_ptr>("getParameter",
            static_cast<type::ParameterValueType (type::*)(const String&) const>(&type::getParameter)
        ),
        defineMemberFunction<5, type_ptr>("getParameterDef",
            static_cast<type::ParameterValueType (type::*)(const String&, type::ParameterValueType) const>(&type::getParameter)
        ),
        {"getObject", (PyCFunction) getObject, METH_VARARGS, "Get n-th object"},
        {NULL}  /* Sentinel */
    };

    type_def::init("simulator.Simulation");
    type_def::definition.tp_getset = properties;
    type_def::definition.tp_methods = fns;
    type_def::ready();
    type_def::finish(module, "Simulation");
}

/* ************************************************************************ */

static void python_wrapper_simulator_Module(PyObject* module)
{
    using type = simulator::Module*;
    using type_def = TypeDefinition<type>;

    type_def::init("simulator.Module");
    type_def::ready();
    type_def::finish(module, "Module");
}

/* ************************************************************************ */

static PyObject* useProgram(ObjectWrapper<simulator::Object*>* self, PyObject* args, void*) noexcept
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
        defineProperty<5, type>("className", &type_base::getClassName),
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
    type_def::finish(module, "Object");

    // Define constants
    PyModule_AddIntConstant(module, "OBJECT_TYPE_STATIC", static_cast<int>(simulator::Object::Type::Static));
    PyModule_AddIntConstant(module, "OBJECT_TYPE_DYNAMIC", static_cast<int>(simulator::Object::Type::Dynamic));
    PyModule_AddIntConstant(module, "OBJECT_TYPE_PINNED", static_cast<int>(simulator::Object::Type::Pinned));
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

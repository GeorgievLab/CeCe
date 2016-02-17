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

// Must be first
#include "cece/plugins/python/Python.hpp"

// CeCe
#include "cece/core/Assert.hpp"
#include "cece/simulator/Simulation.hpp"

// Plugin
#include "cece/plugins/python/Type.hpp"
#include "cece/plugins/python/Utils.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace python {

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

/**
 * @brief Type definition.
 */
class SimulationType : public Type<simulator::Simulation*>
{

// Ctors & Dtors
public:


    /**
     * @brief Constructor.
     */
    SimulationType()
        : Type("simulator.Simulation")
    {
        tp_getset = m_properties;
        tp_methods = m_methods;
    }


    /**
     * @brief Returns world size.
     *
     * @param self
     *
     * @return
     */
    static PyObject* getWorldSize(SelfType* self) noexcept
    {
        return makeObject(self->value->getWorldSize()).release();
    }


    /**
     * @brief Returns a number of current iteration.
     *
     * @param self
     *
     * @return
     */
    static PyObject* getIteration(SelfType* self) noexcept
    {
        return makeObject(self->value->getIteration()).release();
    }


    /**
     * @brief Returns a number of iterations.
     *
     * @param self
     *
     * @return
     */
    static PyObject* getIterations(SelfType* self) noexcept
    {
        return makeObject(self->value->getIterations()).release();
    }


    /**
     * @brief Change iterations number.
     *
     * @param self
     * @param value
     *
     * @return
     */
    static int setIterations(SelfType* self, PyObject* value) noexcept
    {
        try
        {
            self->value->setIterations(cast<simulator::IterationCount>(value));
        }
        catch (const cece::Exception& e)
        {
            PyErr_SetString(PyExc_RuntimeError, e.what());
            return -1;
        }

        return 0;
    }


    /**
     * @brief Returns time step.
     *
     * @param self
     *
     * @return
     */
    static PyObject* getTimeStep(SelfType* self) noexcept
    {
        return makeObject(self->value->getTimeStep()).release();
    }


    /**
     * @brief Change time step.
     *
     * @param self
     * @param args
     *
     * @return
     */
    static int setTimeStep(SelfType* self, PyObject* value) noexcept
    {
        try
        {
            self->value->setTimeStep(cast<units::Time>(value));
        }
        catch (const cece::Exception& e)
        {
            PyErr_SetString(PyExc_RuntimeError, e.what());
            return -1;
        }

        return 0;
    }


    /**
     * @brief Returns total simulation time.
     *
     * @param self
     *
     * @return
     */
    static PyObject* getTotalTime(SelfType* self) noexcept
    {
        return makeObject(self->value->getTotalTime()).release();
    }


    /**
     * @brief Returns simulation gravity vector.
     *
     * @param self
     *
     * @return
     */
    static PyObject* getGravity(SelfType* self) noexcept
    {
        return makeObject(self->value->getGravity()).release();
    }


    /**
     * @brief Change gravity vector.
     *
     * @param self
     * @param value
     *
     * @return
     */
    static int setGravity(SelfType* self, PyObject* value) noexcept
    {
        try
        {
            self->value->setGravity(cast<AccelerationVector>(value));
        }
        catch (const cece::Exception& e)
        {
            PyErr_SetString(PyExc_RuntimeError, e.what());
            return -1;
        }

        return 0;
    }


    /**
     * @brief Returns a number of objects.
     *
     * @param self
     *
     * @return
     */
    static PyObject* getObjectCount(SelfType* self) noexcept
    {
        return makeObject(self->value->getObjectCount()).release();
    }


    /**
     * @brief Gets a module.
     *
     * @param self
     * @param args
     *
     * @return
     */
    static PyObject* useModule(SelfType* self, PyObject* args) noexcept
    {
        char* name;

        if (!PyArg_ParseTuple(args, "s", &name))
            return nullptr;

        try
        {
            return makeObject(self->value->useModule(name)).release();
        }
        catch (const cece::Exception& e)
        {
            PyErr_SetString(PyExc_RuntimeError, e.what());
        }

        return nullptr;
    }


    /**
     * @brief Create a new object.
     *
     * @param self
     * @param args
     *
     * @return
     */
    static PyObject* buildObject(SelfType* self, PyObject* args) noexcept
    {
        char* name;
        int type = static_cast<int>(object::Object::Type::Dynamic);

        if (!PyArg_ParseTuple(args, "s|i", &name, &type))
            return nullptr;

        try
        {
            return makeObject(self->value->buildObject(name, static_cast<object::Object::Type>(type))).release();
        }
        catch (const cece::Exception& e)
        {
            PyErr_SetString(PyExc_RuntimeError, e.what());
        }

        return nullptr;
    }


    /**
     * @brief Returns a number of object of given type.
     *
     * @param self
     * @param args
     *
     * @return
     */
    static PyObject* objectCountType(SelfType* self, PyObject* args) noexcept
    {
        char* name;

        if (!PyArg_ParseTuple(args, "s", &name))
            return nullptr;

        try
        {
            return makeObject(self->value->getObjectCountType(name)).release();
        }
        catch (const cece::Exception& e)
        {
            PyErr_SetString(PyExc_RuntimeError, e.what());
        }

        return nullptr;
    }


    /**
     * @brief Returns parameter.
     *
     * @param self
     * @param args
     *
     * @return
     */
    static PyObject* getParameter(SelfType* self, PyObject* args) noexcept
    {
        char* name;
        char* def = 0;

        if (!PyArg_ParseTuple(args, "s|s", &name, &def))
            return nullptr;

        try
        {
            // Return parameter
            return makeObject(self->value->getParameter(name, def)).release();
        }
        catch (const cece::Exception& e)
        {
            PyErr_SetString(PyExc_RuntimeError, e.what());
        }

        return nullptr;
    }


    /**
     * @brief Returns object by position.
     *
     * @param self
     * @param args
     *
     * @return
     */
    static PyObject* getObject(SelfType* self, PyObject* args) noexcept
    {
        int pos;

        if (!PyArg_ParseTuple(args, "i", &pos))
            return nullptr;

        try
        {
            return makeObject(self->value->getObjects()[pos].get()).release();
        }
        catch (const cece::Exception& e)
        {
            PyErr_SetString(PyExc_RuntimeError, e.what());
        }

        return nullptr;
    }


// Private Data Members
private:

    // Type properties.
    PyGetSetDef m_properties[8] = {
        {const_cast<char*>("worldSize"),   (getter) getWorldSize,   nullptr,                nullptr},
        {const_cast<char*>("iteration"),   (getter) getIteration,   nullptr,                nullptr},
        {const_cast<char*>("iterations"),  (getter) getIterations,  (setter) setIterations, nullptr},
        {const_cast<char*>("timeStep"),    (getter) getTimeStep,    (setter) setTimeStep,   nullptr},
        {const_cast<char*>("gravity"),     (getter) getGravity,     (setter) setGravity,    nullptr},
        {const_cast<char*>("totalTime"),   (getter) getTotalTime,   nullptr,                nullptr},
        {const_cast<char*>("objectCount"), (getter) getObjectCount, nullptr,                nullptr},
        {nullptr}  /* Sentinel */
    };


    // Type methods.
    PyMethodDef m_methods[6] = {
        {"useModule",       (PyCFunction) useModule,        METH_VARARGS, nullptr},
        {"buildObject",     (PyCFunction) buildObject,      METH_VARARGS, nullptr},
        {"objectCountType", (PyCFunction) objectCountType,  METH_VARARGS, nullptr},
        {"getParameter",    (PyCFunction) getParameter,     METH_VARARGS, nullptr},
        {"getObject",       (PyCFunction) getObject,        METH_VARARGS, nullptr},
        {nullptr}  /* Sentinel */
    };

};

/* ************************************************************************ */

SimulationType g_type;

/* ************************************************************************ */

}

/* ************************************************************************ */

void init_simulator_Simulation(PyObject* module)
{
    g_type.add(module);
}

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */

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
#include "cece/object/Object.hpp"
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
class ObjectType : public Type<object::Object*>
{


// Ctors & Dtors
public:


    /**
     * @brief Constructor.
     */
    explicit ObjectType()
        : Type("cece.Object")
    {
        tp_getset = m_properties;
        tp_methods = m_methods;
    }


// Public Operations
public:


    /**
     * @brief Returns object ID.
     *
     * @param self
     *
     * @return
     */
    static PyObject* getId(SelfType* self) noexcept
    {
        return makeObject(self->value->getId()).release();
    }


    /**
     * @brief Returns object position.
     *
     * @param self
     *
     * @return
     */
    static PyObject* getPosition(SelfType* self) noexcept
    {
        return makeObject(self->value->getPosition()).release();
    }


    /**
     * @brief Sets object position.
     *
     * @param self
     * @param value
     *
     * @return
     */
    static int setPosition(SelfType* self, PyObject* value) noexcept
    {
        self->value->setPosition(cast<PositionVector>(value));

        return 0;
    }


    /**
     * @brief Returns object rotation.
     *
     * @param self
     *
     * @return
     */
    static PyObject* getRotation(SelfType* self) noexcept
    {
        return makeObject(self->value->getRotation()).release();
    }


    /**
     * @brief Change object rotation.
     *
     * @param self
     * @param value
     *
     * @return
     */
    static int setRotation(SelfType* self, PyObject* value) noexcept
    {
        self->value->setRotation(cast<units::Angle>(value));

        return 0;
    }


    /**
     * @brief Returns object velocity.
     *
     * @param self
     *
     * @return
     */
    static PyObject* getVelocity(SelfType* self) noexcept
    {
        return makeObject(self->value->getVelocity()).release();
    }


    /**
     * @brief Change object velocity.
     *
     * @param self
     * @param value
     *
     * @return
     */
    static int setVelocity(SelfType* self, PyObject* value) noexcept
    {
        self->value->setVelocity(cast<VelocityVector>(value));

        return 0;
    }


    /**
     * @brief Returns object density.
     *
     * @param self
     *
     * @return
     */
    static PyObject* getDensity(SelfType* self) noexcept
    {
        return makeObject(self->value->getDensity()).release();
    }


    /**
     * @brief Sets object position.
     *
     * @param self
     * @param value
     *
     * @return
     */
    static int setDensity(SelfType* self, PyObject* value) noexcept
    {
        self->value->setDensity(cast<units::Density>(value));

        return 0;
    }


    /**
     * @brief Returns object mass.
     *
     * @param self
     *
     * @return
     */
    static PyObject* getMass(SelfType* self) noexcept
    {
        return makeObject(self->value->getMass()).release();
    }


    /**
     * @brief Returns object class name.
     *
     * @param self
     *
     * @return
     */
    static PyObject* getClassName(SelfType* self) noexcept
    {
        return makeObject(self->value->getTypeName()).release();
    }


    /**
     * @brief Returns object type name.
     *
     * @param self
     *
     * @return
     */
    static PyObject* getTypeName(SelfType* self) noexcept
    {
        return makeObject(self->value->getTypeName()).release();
    }


    /**
     * @brief Apply force to object.
     *
     * @param self
     * @param args
     *
     * @return
     */
    static PyObject* applyForce(SelfType* self, PyObject* args) noexcept
    {
        PyObject* force;

        if (!PyArg_ParseTuple(args, "O", &force))
            return nullptr;

        self->value->applyForce(cast<ForceVector>(force));

        return none().release();
    }


    /**
     * @brief Use program.
     *
     * @param self
     * @param args
     *
     * @return
     */
    static PyObject* useProgram(SelfType* self, PyObject* args) noexcept
    {
        char* name;

        if (!PyArg_ParseTuple(args, "s", &name))
            return NULL;

        // Add program
        self->value->useProgram(name);

        return none().release();
    }


    /**
     * @brief Destroy object.
     *
     * @param self
     *
     * @return
     */
    static PyObject* destroy(SelfType* self) noexcept
    {
        self->value->destroy();

        return none().release();
    }


// Private Data Members
private:

    /// Type properties.
    PyGetSetDef m_properties[9] = {
        {const_cast<char*>("id"),        (getter) getId,        nullptr,               nullptr},
        {const_cast<char*>("position"),  (getter) getPosition,  (setter) setPosition,  nullptr},
        {const_cast<char*>("rotation"),  (getter) getRotation,  (setter) setRotation,  nullptr},
        {const_cast<char*>("velocity"),  (getter) getVelocity,  (setter) setVelocity,  nullptr},
        {const_cast<char*>("density"),   (getter) getDensity,   (setter) setDensity,   nullptr},
        {const_cast<char*>("mass"),      (getter) getMass,      nullptr,               nullptr},
        {const_cast<char*>("className"), (getter) getClassName, nullptr,               nullptr},
        {const_cast<char*>("typeName"),  (getter) getTypeName,  nullptr,               nullptr},
        {nullptr}  /* Sentinel */
    };

    /// Type methods.
    PyMethodDef m_methods[4] = {
        {"applyForce", (PyCFunction) applyForce, METH_VARARGS, nullptr},
        {"useProgram", (PyCFunction) useProgram, METH_VARARGS, nullptr},
        {"destroy",    (PyCFunction) destroy,    METH_NOARGS,  nullptr},
        {nullptr}  /* Sentinel */
    };

};

/* ************************************************************************ */

ObjectType g_type;

/* ************************************************************************ */

}

/* ************************************************************************ */

void init_simulator_Object(PyObject* module)
{
    g_type.add(module);

    // Define constants
    PyModule_AddIntConstant(module, "OBJECT_TYPE_STATIC", static_cast<int>(object::Object::Type::Static));
    PyModule_AddIntConstant(module, "OBJECT_TYPE_DYNAMIC", static_cast<int>(object::Object::Type::Dynamic));
    PyModule_AddIntConstant(module, "OBJECT_TYPE_PINNED", static_cast<int>(object::Object::Type::Pinned));
}

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */

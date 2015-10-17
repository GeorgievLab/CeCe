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
#include "plugins/python/Python.hpp"

// Simulator
#include "core/Assert.hpp"
#include "core/Vector.hpp"
#include "core/Exception.hpp"

// Plugin
#include "plugins/python/ObjectWrapper.hpp"
#include "plugins/python/Utils.hpp"

/* ************************************************************************ */

namespace plugin {
namespace python {

/* ************************************************************************ */

/**
 * @brief Vector type definition.
 *
 * @tparam Vector type.
 */
template<typename T>
class VectorType : public PyTypeObject
{

// Public Types
public:


    /// Vector type.
    using Type = T;

    /// Vector element type.
    using ValueType = typename Type::ValueType;

    /// Wrapper type.
    using SelfType = ObjectWrapper<Type>;


// Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param name Type name.
     */
    explicit VectorType(String name)
        : PyTypeObject {PyObject_HEAD_INIT(NULL)}
        , m_name(std::move(name))
    {
        tp_name = m_name.c_str();
        tp_basicsize = sizeof(SelfType);
        tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE;
        tp_getset = m_properties;
        tp_methods = m_methods;
        tp_new = (newfunc) &__new__;
        tp_dealloc = (destructor) &__del__;
        tp_init = (initproc) &__init__;
        tp_richcompare = (richcmpfunc) __cmp__;
        tp_as_number = &m_numberMethods;

        // Type is not ready
        if (PyType_Ready(this) < 0)
            throw RuntimeException("Cannot finalize type object");
    }


// Public Operations
public:


    /**
     * @brief Finalize type definition.
     *
     * @param module
     */
    void define(View<PyObject> module) noexcept
    {
        auto type = reinterpret_cast<PyObject*>(this);

        // Find dot
        auto dot = m_name.find('.');
        Assert(dot != String::npos);

        Py_INCREF(type);
        PyModule_AddObject(module, &m_name[dot + 1], type);

        // Register dynamic type
        registerType(typeid(Type), this);
    }


    /**
     * @brief Allocate object.
     *
     * @param type
     * @param args
     * @param kwds
     *
     * @return
     */
    static PyObject* __new__(PyTypeObject* type, PyObject* args, PyObject* kwds)
    {
        SelfType* self = reinterpret_cast<SelfType*>(type->tp_alloc(type, 0));

        if (self != nullptr) {
            // Inplace allocation
            new (&self->value) Type{};
        }

        return reinterpret_cast<PyObject*>(self);
    }


    /**
     * @brief Deallocate object.
     *
     * @param self
     */
    static void __del__(SelfType* self) noexcept
    {
        // Call destructor
        self->value.~Type();

        self->ob_type->tp_free(reinterpret_cast<PyObject*>(self));
    }


    /**
     * @brief Construct object.
     *
     * @param self
     * @param args
     * @param kwds
     *
     * @return
     */
    static int __init__(SelfType* self, PyObject* args, PyObject* kwds)
    {
        PyObject* x;
        PyObject* y;

        const char* kwlist[] = {"x", "y", nullptr};

        if (!PyArg_ParseTupleAndKeywords(args, kwds, "|OO", const_cast<char**>(kwlist), &x, &y))
            return -1;

        if (x && y)
        {
            // Construct value
            self->value = Type(cast<ValueType>(x), cast<ValueType>(y));
        }
        else if (x || y)
        {
            PyErr_SetString(PyExc_ValueError, "Both 'x' and 'y' must be set");
            return -1;
        }

        return 0;
    }


    /**
     * @brief Compare vectors.
     *
     * @param lhs
     * @param rhs
     * @param op
     *
     * @return
     */
    static PyObject* __cmp__(SelfType* lhs, PyObject* rhs, int op) noexcept
    {
        if (lhs->ob_type != rhs->ob_type)
        {
            PyErr_SetString(PyExc_ValueError, "Operands must have same type");
            return nullptr;
        }

        SelfType* rhsCast = reinterpret_cast<SelfType*>(rhs);

        bool res = false;

        switch (op)
        {
        case Py_LT: res = lhs->value <  rhsCast->value; break;
        case Py_LE: res = lhs->value <= rhsCast->value; break;
        case Py_EQ: res = lhs->value == rhsCast->value; break;
        case Py_NE: res = lhs->value != rhsCast->value; break;
        case Py_GT: res = lhs->value >  rhsCast->value; break;
        case Py_GE: res = lhs->value >= rhsCast->value; break;
        }

        return res ? Py_True : Py_False;
    }


    /**
     * @brief Addition.
     *
     * @param lhs
     * @param rhs
     *
     * @return
     */
    static PyObject* __add__(SelfType* lhs, PyObject* rhs) noexcept
    {
        if (lhs->ob_type != rhs->ob_type)
        {
            PyErr_SetString(PyExc_ValueError, "Operands must have same type");
            return nullptr;
        }

        SelfType* rhsCast = reinterpret_cast<SelfType*>(rhs);

        return makeObject(lhs->value + rhsCast->value).release();
    }


    /**
     * @brief Substraction.
     *
     * @param lhs
     * @param rhs
     *
     * @return
     */
    static PyObject* __sub__(SelfType* lhs, PyObject* rhs) noexcept
    {
        if (lhs->ob_type != rhs->ob_type)
        {
            PyErr_SetString(PyExc_ValueError, "Operands must have same type");
            return nullptr;
        }

        SelfType* rhsCast = reinterpret_cast<SelfType*>(rhs);

        return makeObject(lhs->value - rhsCast->value).release();
    }


    /**
     * @brief Multiplication.
     *
     * @param lhs
     * @param rhs
     *
     * @return
     */
    static PyObject* __mul__(SelfType* lhs, PyObject* rhs) noexcept
    {
        if (PyInt_Check(rhs))
        {
            return makeObject(lhs->value * PyInt_AS_LONG(rhs)).release();
        }
        else if (PyFloat_Check(rhs))
        {
            return makeObject(lhs->value * PyFloat_AS_DOUBLE(rhs)).release();
        }
        else if (PyLong_Check(rhs))
        {
            return makeObject(lhs->value * PyLong_AsLong(rhs)).release();
        }
        else if (lhs->ob_type != rhs->ob_type)
        {
            PyErr_SetString(PyExc_ValueError, "Operands must have same type");
            return nullptr;
        }

        SelfType* rhsCast = reinterpret_cast<SelfType*>(rhs);

        return makeObject(lhs->value * rhsCast->value).release();
    }


    /**
     * @brief Division.
     *
     * @param lhs
     * @param rhs
     *
     * @return
     */
    static PyObject* __div__(SelfType* lhs, PyObject* rhs) noexcept
    {
        if (PyInt_Check(rhs))
        {
            return makeObject(lhs->value / PyInt_AS_LONG(rhs)).release();
        }
        else if (PyFloat_Check(rhs))
        {
            return makeObject(lhs->value / PyFloat_AS_DOUBLE(rhs)).release();
        }
        else if (PyLong_Check(rhs))
        {
            return makeObject(lhs->value / PyLong_AsLong(rhs)).release();
        }
        else if (lhs->ob_type != rhs->ob_type)
        {
            PyErr_SetString(PyExc_ValueError, "Operands must have same type");
            return nullptr;
        }

        SelfType* rhsCast = reinterpret_cast<SelfType*>(rhs);

        return makeObject(lhs->value / rhsCast->value).release();
    }


    /**
     * @brief Negative.
     *
     * @param lhs
     * @param rhs
     *
     * @return
     */
    static PyObject* __neg__(SelfType* rhs) noexcept
    {
        return makeObject(-rhs->value).release();
    }


    /**
     * @brief Positive.
     *
     * @param lhs
     * @param rhs
     *
     * @return
     */
    static PyObject* __pos__(SelfType* rhs) noexcept
    {
        return makeObject(rhs->value).release();
    }


    /**
     * @brief Nonzero test.
     *
     * @return
     */
    static int __nonzero__(SelfType* rhs) noexcept
    {
        return rhs->value == Zero ? 1 : 0;
    }


    /**
     * @brief Addition.
     *
     * @param lhs
     * @param rhs
     *
     * @return
     */
    static PyObject* __iadd__(SelfType* lhs, PyObject* rhs) noexcept
    {
        if (lhs->ob_type != rhs->ob_type)
        {
            PyErr_SetString(PyExc_ValueError, "Operands must have same type");
            return nullptr;
        }

        SelfType* rhsCast = reinterpret_cast<SelfType*>(rhs);

        lhs->value += rhsCast->value;

        return reinterpret_cast<PyObject*>(lhs);
    }


    /**
     * @brief Substraction.
     *
     * @param lhs
     * @param rhs
     *
     * @return
     */
    static PyObject* __isub__(SelfType* lhs, PyObject* rhs) noexcept
    {
        if (lhs->ob_type != rhs->ob_type)
        {
            PyErr_SetString(PyExc_ValueError, "Operands must have same type");
            return nullptr;
        }

        SelfType* rhsCast = reinterpret_cast<SelfType*>(rhs);

        lhs->value -= rhsCast->value;

        return reinterpret_cast<PyObject*>(lhs);
    }


    /**
     * @brief Multiplication.
     *
     * @param lhs
     * @param rhs
     *
     * @return
     */
    static PyObject* __imul__(SelfType* lhs, PyObject* rhs) noexcept
    {
        if (PyInt_Check(rhs))
        {
            lhs->value *= PyInt_AS_LONG(rhs);
        }
        else if (PyFloat_Check(rhs))
        {
            lhs->value *= PyFloat_AS_DOUBLE(rhs);
        }
        else if (PyLong_Check(rhs))
        {
            lhs->value *= PyLong_AsLong(rhs);
        }
        else
        {
            PyErr_SetString(PyExc_ValueError, "Right operand must be scalar");
            return nullptr;
        }

        return reinterpret_cast<PyObject*>(lhs);
    }


    /**
     * @brief Division.
     *
     * @param lhs
     * @param rhs
     *
     * @return
     */
    static PyObject* __idiv__(SelfType* lhs, PyObject* rhs) noexcept
    {
        if (PyInt_Check(rhs))
        {
            lhs->value /= PyInt_AS_LONG(rhs);
        }
        else if (PyFloat_Check(rhs))
        {
            lhs->value /= PyFloat_AS_DOUBLE(rhs);
        }
        else if (PyLong_Check(rhs))
        {
            lhs->value /= PyLong_AsLong(rhs);
        }
        else
        {
            PyErr_SetString(PyExc_ValueError, "Right operand must be scalar");
            return nullptr;
        }

        return reinterpret_cast<PyObject*>(lhs);
    }


    /**
     * @brief Returns X value.
     *
     * @param self
     *
     * @return
     */
    static PyObject* getX(SelfType* self) noexcept
    {
        return makeObject(self->value.getX()).release();
    }


    /**
     * @brief Set X value.
     *
     * @param self
     * @param value
     *
     * @return
     */
    static int setX(SelfType* self, PyObject* value) noexcept
    {
        self->value.setX(cast<ValueType>(value));

        return 0;
    }


    /**
     * @brief Returns Y value.
     *
     * @param self
     *
     * @return
     */
    static PyObject* getY(SelfType* self) noexcept
    {
        return makeObject(self->value.getY()).release();
    }


    /**
     * @brief Set Y value.
     *
     * @param self
     * @param value
     *
     * @return
     */
    static int setY(SelfType* self, PyObject* value) noexcept
    {
        self->value.setY(cast<ValueType>(value));

        return 0;
    }


    /**
     * @brief Returns vector length.
     *
     * @param self
     *
     * @return
     */
    static PyObject* getLength(SelfType* self) noexcept
    {
        return makeObject(self->value.getLength()).release();
    }


    /**
     * @brief Returns vector squared length.
     *
     * @param self
     *
     * @return
     */
    static PyObject* getLengthSquared(SelfType* self) noexcept
    {
        return makeObject(self->value.getLengthSquared()).release();
    }


    /**
     * @brief Returns dot value.
     *
     * @param self
     *
     * @return
     */
    static PyObject* dot(SelfType* self, PyObject* args) noexcept
    {
        PyObject* vec;

        if (!PyArg_ParseTuple(args, "O", &vec))
            return nullptr;

        return makeObject(self->value.dot(cast<Type>(vec))).release();
    }


// Private Data Members
private:

    /// Type name.
    String m_name;

    /// Type properties.
    PyGetSetDef m_properties[7] = {
        {const_cast<char*>("x"),      (getter) getX, (setter) setX, nullptr},
        {const_cast<char*>("y"),      (getter) getY, (setter) setY, nullptr},
        {const_cast<char*>("width"),  (getter) getX, (setter) setX, nullptr},
        {const_cast<char*>("height"), (getter) getY, (setter) setY, nullptr},
        {const_cast<char*>("length"), (getter) getLength, nullptr, nullptr},
        {const_cast<char*>("lengthSquared"), (getter) getLengthSquared, nullptr, nullptr},
        {nullptr}  /* Sentinel */
    };

    /// Type methods.
    PyMethodDef m_methods[2] = {
        {"dot", (PyCFunction) dot, METH_VARARGS, nullptr},
        {nullptr}  /* Sentinel */
    };

    /// Number methods.
    PyNumberMethods m_numberMethods = {
        (binaryfunc) __add__,
        (binaryfunc) __sub__,
        (binaryfunc) __mul__,
        (binaryfunc) __div__,
        nullptr,
        nullptr,
        nullptr,
        (unaryfunc) __neg__,
        (unaryfunc) __pos__,
        nullptr,
        (inquiry) __nonzero__,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        (binaryfunc) __iadd__,
        (binaryfunc) __isub__,
        (binaryfunc) __imul__,
        (binaryfunc) __idiv__,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr
    };

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */

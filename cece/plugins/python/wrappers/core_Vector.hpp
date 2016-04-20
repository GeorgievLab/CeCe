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
#include "cece/core/Vector.hpp"
#include "cece/core/Exception.hpp"

// Plugin
#include "cece/plugins/python/Type.hpp"
#include "cece/plugins/python/Utils.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace python {

/* ************************************************************************ */

/**
 * @brief Vector type definition.
 *
 * @tparam Vector type.
 */
template<typename T>
class VectorType : public Type<T>
{

// Public Types
public:


    // Parent type.
    using ParentType = Type<T>;

    /// Vector element type.
    using ValueType = typename T::ValueType;

    /// Wrapper type.
    using SelfType = typename ParentType::SelfType;


// Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param name Type name.
     */
    explicit VectorType(String name)
        : ParentType(std::move(name))
    {
        ParentType::tp_getset = const_cast<PyGetSetDef*>(m_properties);
        ParentType::tp_methods = const_cast<PyMethodDef*>(m_methods);
        ParentType::tp_new = (newfunc) &__new__;
        ParentType::tp_dealloc = (destructor) &__del__;
        ParentType::tp_init = (initproc) &__init__;
        ParentType::tp_richcompare = (richcmpfunc) __cmp__;
        ParentType::tp_as_number = const_cast<PyNumberMethods*>(&m_numberMethods);
    }


// Public Operations
public:


    /**
     * @brief Allocate object.
     *
     * @param type
     * @param args
     * @param kwds
     *
     * @return
     */
    static PyObject* __new__(PyTypeObject* type, PyObject* args, PyObject* kwds) noexcept
    {
        SelfType* self = reinterpret_cast<SelfType*>(type->tp_alloc(type, 0));

        if (self != nullptr) {
            // Inplace allocation
            new (&self->value) T{};
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
        self->value.~T();

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
            self->value = T(cast<ValueType>(x), cast<ValueType>(y));
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

        return makeObject(self->value.dot(cast<T>(vec))).release();
    }


// Private Data Members
private:

    /// Type properties.
    static PyGetSetDef m_properties[7];

    /// Type methods.
    static PyMethodDef m_methods[2];

    /// Number methods.
    static PyNumberMethods m_numberMethods;

};

/* ************************************************************************ */

template<typename T>
PyGetSetDef VectorType<T>::m_properties[7] = {
    {const_cast<char*>("x"),      (getter) VectorType<T>::getX, (setter) VectorType<T>::setX, nullptr},
    {const_cast<char*>("y"),      (getter) VectorType<T>::getY, (setter) VectorType<T>::setY, nullptr},
    {const_cast<char*>("width"),  (getter) VectorType<T>::getX, (setter) VectorType<T>::setX, nullptr},
    {const_cast<char*>("height"), (getter) VectorType<T>::getY, (setter) VectorType<T>::setY, nullptr},
    {const_cast<char*>("length"), (getter) VectorType<T>::getLength, nullptr, nullptr},
    {const_cast<char*>("lengthSquared"), (getter) VectorType<T>::getLengthSquared, nullptr, nullptr},
    {nullptr}  /* Sentinel */
};

/* ************************************************************************ */

template<typename T>
PyMethodDef VectorType<T>::m_methods[2] = {
    {"dot", (PyCFunction) VectorType<T>::dot, METH_VARARGS, nullptr},
    {nullptr}  /* Sentinel */
};

/* ************************************************************************ */

template<typename T>
PyNumberMethods VectorType<T>::m_numberMethods = {
    (binaryfunc) VectorType<T>::__add__,
    (binaryfunc) VectorType<T>::__sub__,
    (binaryfunc) VectorType<T>::__mul__,
    (binaryfunc) VectorType<T>::__div__,
    nullptr,
    nullptr,
    nullptr,
    (unaryfunc) VectorType<T>::__neg__,
    (unaryfunc) VectorType<T>::__pos__,
    nullptr,
    (inquiry) VectorType<T>::__nonzero__,
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
    (binaryfunc) VectorType<T>::__iadd__,
    (binaryfunc) VectorType<T>::__isub__,
    (binaryfunc) VectorType<T>::__imul__,
    (binaryfunc) VectorType<T>::__idiv__,
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

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */

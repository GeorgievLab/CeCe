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

#pragma once

/* ************************************************************************ */

// C++
#include <typeindex>

// Simulator
#include "core/Exception.hpp"
#include "core/Units.hpp"
#include "core/Tuple.hpp"
#include "core/String.hpp"
#include "core/IntegerSequence.hpp"

// Plugin
#include "Python.hpp"
#include "Handle.hpp"
#include "View.hpp"
#include "ValueCast.hpp"
#include "Exception.hpp"

/* ************************************************************************ */

namespace plugin {
namespace python {

/* ************************************************************************ */

using HashValueType = unsigned long;

/* ************************************************************************ */

/**
 * @brief C++ type wrapper object.
 */
template<typename T>
struct ObjectWrapper
{
    PyObject_HEAD
    T value;
};

/* ************************************************************************ */

/**
 * @brief Register dynamic type.
 *
 * @param info Type info.
 * @param type Python type.
 */
void registerDynamic(const std::type_info& info, PyTypeObject* type);

/* ************************************************************************ */

/**
 * @brief Find dynamic type.
 *
 * @param info Type info.
 *
 * @return Dynamic type or nullptr.
 */
PyTypeObject* findDynamic(const std::type_info& info);

/* ************************************************************************ */

/**
 * @brief Returns reference to object.
 *
 * @param obj Reference to object.
 *
 * @return
 */
template<typename T>
T& ref(T& obj) noexcept
{
    return obj;
}

/* ************************************************************************ */

/**
 * @brief Returns reference to object.
 *
 * @param obj Pointer to object.
 *
 * @return
 */
template<typename T>
T& ref(T* obj) noexcept
{
    return *obj;
}

/* ************************************************************************ */

/**
 * @brief Constructor wrapper.
 *
 * @tparam T    Object type.
 * @tparam Args Argument types.
 */
template<typename T, typename... Args>
struct Constructor
{
    /// Plain object type.
    using ObjectType = typename std::remove_pointer<T>::type;

    /// Remove const reference.
    template<typename Type>
    using RemoveConstRef = typename std::remove_const<typename std::remove_reference<Type>::type>::type;


    /**
     * @brief Convert to PyMethodDef.
     *
     * @return
     */
    static initproc to() noexcept
    {
        return (initproc) &construct;
    }


    /**
     * @brief Calls constructor (helper).
     *
     * @param self
     * @param args
     *
     * @return
     */
    template<int... I>
    static int construct_inner(ObjectWrapper<T>* self, PyObject* args, IntegerSequence<int, I...>) noexcept
    {
        assert(self);
        assert(args);

        // Tuple of args
        const Tuple<RemoveConstRef<Args>...> tupleArgs{
            cast<RemoveConstRef<Args>>(PyTuple_GetItem(args, I))...
        };

        void* ptr = &ref(self->value);
        new (ptr) T(std::get<I>(tupleArgs)...);

        return 0;
    }


    /**
     * @brief Calls constructor.
     *
     * @param self
     * @param args
     * @param kwds
     *
     * @return
     */
    static int construct(ObjectWrapper<T>* self, PyObject* args, PyObject* kwds)
    {
        return construct_inner(self, args, MakeIntegerSequence<int, 0, sizeof...(Args)>{});
    }
};

/* ************************************************************************ */

/**
 * @brief Property wrapper.
 *
 * @tparam T    Object type.
 * @tparam Val  Value type.
 */
template<HashValueType Hash, typename T, typename Val>
struct Property
{
    /// Plain object type.
    using ObjectType = typename std::remove_pointer<T>::type;

    /// Getter function type.
    using Getter = Val(ObjectType::*)() const;

    /// Setter function type.
    using Setter = void(ObjectType::*)(Val);


    /// Property name.
    static const char* name;

    /// Getter function pointer.
    static Getter getFn;

    /// Setter function pointer.
    static Setter setFn;


    /**
     * @brief Convert to PyGetSetDef.
     *
     * @return
     */
    static PyGetSetDef to() noexcept
    {
        return {const_cast<char*>(name), (getter) get, (setter) set, nullptr, nullptr};
    }


    /**
     * @brief Calls object getter.
     *
     * @param self
     * @param closure
     *
     * @return
     */
    static PyObject* get(ObjectWrapper<T>* self, void* closure) noexcept
    {
        assert(self);
        assert(getFn);
        return cast((ref(self->value).*getFn)()).release();
    }


    /**
     * @brief Calls object setter.
     *
     * @param self
     * @param closure
     *
     * @return
     */
    static int set(ObjectWrapper<T>* self, PyObject* value, void* closure)
    {
        if (!check<Val>(value))
            throw Exception("Cannot convert value");

        assert(self);
        assert(setFn);
        (ref(self->value).*setFn)(cast<Val>(value));

        return 0;
    }
};

/* ************************************************************************ */

/**
 * @brief Property wrapper (non-member).
 *
 * @tparam T    Object type.
 * @tparam Val  Value type.
 */
template<HashValueType Hash, typename T, typename Val>
struct PropertyAlt
{
    /// Plain object type.
    using ObjectType = typename std::remove_pointer<T>::type;

    /// Getter function type.
    using Getter = Val(*)(T);

    /// Setter function type.
    using Setter = void(*)(T, Val);


    /// Property name.
    static const char* name;

    /// Getter function pointer.
    static Getter getFn;

    /// Setter function pointer.
    static Setter setFn;


    /**
     * @brief Convert to PyGetSetDef.
     *
     * @return
     */
    static PyGetSetDef to() noexcept
    {
        return {const_cast<char*>(name), (getter) get, (setter) set, nullptr, nullptr};
    }


    /**
     * @brief Calls object getter.
     *
     * @param self
     * @param closure
     *
     * @return
     */
    static PyObject* get(ObjectWrapper<T>* self, void* closure) noexcept
    {
        assert(self);
        assert(getFn);
        return cast<Val>(getFn(self->value)).release();
    }


    /**
     * @brief Calls object setter.
     *
     * @param self
     * @param closure
     *
     * @return
     */
    static int set(ObjectWrapper<T>* self, PyObject* value, void* closure)
    {
        if (!check<Val>(value))
            throw Exception("Cannot convert value");

        assert(self);
        assert(setFn);
        setFn(self->value, cast<Val>(value));

        return 0;
    }
};

/* ************************************************************************ */

/**
 * @brief Define type property.
 *
 * @param name
 * @param getter
 * @param setter
 *
 * @return
 */
template<HashValueType Hash, typename T, typename Val, typename TR = typename std::remove_pointer<T>::type>
PyGetSetDef defineProperty(const char* name, Val(TR::*getter)() const, void(TR::*setter)(Val) = nullptr) noexcept
{
    using PropertyType = Property<Hash, T, Val>;

    PropertyType::name = name;
    PropertyType::getFn = getter;
    PropertyType::setFn = setter;

    return PropertyType::to();
}

/* ************************************************************************ */

/**
 * @brief Define type property (non-member functions).
 *
 * @param name
 * @param getter
 * @param setter
 *
 * @return
 */
template<HashValueType Hash, typename T, typename Val>
PyGetSetDef defineProperty(const char* name, Val(*getter)(T), void(*setter)(T, Val) = nullptr) noexcept
{
    using PropertyType = PropertyAlt<Hash, T, Val>;

    PropertyType::name = name;
    PropertyType::getFn = getter;
    PropertyType::setFn = setter;

    return PropertyType::to();
}

/* ************************************************************************ */

/**
 * @brief Member function wrapper.
 *
 * @tparam T    Object type.
 * @tparam Name Name type.
 * @tparam Ret  Return type.
 * @tparam Args Argument types.
 */
template<HashValueType Hash, typename T, typename Ret, typename... Args>
struct MemberFunction
{
    /// Plain object type.
    using ObjectType = typename std::remove_pointer<T>::type;

    /// Function type.
    using Function = Ret(ObjectType::*)(Args...);

    /// Remove const reference.
    template<typename Type>
    using RemoveConstRef = typename std::remove_const<typename std::remove_reference<Type>::type>::type;

    /// Function name.
    static const char* name;

    /// Function pointer.
    static Function fn;


    /**
     * @brief Convert to PyMethodDef.
     *
     * @return
     */
    static PyMethodDef to() noexcept
    {
        return {name, (PyCFunction) call, METH_VARARGS, nullptr};
    }


    /**
     * @brief Calls object function (helper).
     *
     * @param self
     * @param args
     *
     * @return
     */
    template<int... I>
    static PyObject* call_inner(ObjectWrapper<T>* self, PyObject* args, IntegerSequence<int, I...>) noexcept
    {
        assert(self);
        assert(args);
        assert(fn);

        // Tuple of args
        const Tuple<RemoveConstRef<Args>...> tupleArgs{
            cast<RemoveConstRef<Args>>(PyTuple_GetItem(args, I))...
        };

        if (PyErr_Occurred())
            throw Exception{};

        try
        {
            return cast<Ret>((ref(self->value).*fn)(std::get<I>(tupleArgs)...)).release();
        }
        catch (const Exception& e)
        {
            PyErr_SetString(PyExc_RuntimeError, e.what());
            return nullptr;
        }
    }


    /**
     * @brief Calls object function.
     *
     * @param self
     * @param args
     * @param closure
     *
     * @return
     */
    static PyObject* call(ObjectWrapper<T>* self, PyObject* args, void* closure) noexcept
    {
        if (PyTuple_Size(args) != sizeof...(Args))
        {
            PyErr_SetString(PyExc_RuntimeError, "Arguments mismatch");
            return nullptr;
        }

        return call_inner(self, args, MakeIntegerSequence<int, 0, sizeof...(Args)>{});
    }
};

/* ************************************************************************ */

/**
 * @brief Member function wrapper.
 *
 * @tparam T    Object type.
 * @tparam Name Name type.
 * @tparam Ret  Return type.
 * @tparam Args Argument types.
 */
template<HashValueType Hash, typename T, typename... Args>
struct MemberFunction<Hash, T, void, Args...>
{
    /// Plain object type.
    using ObjectType = typename std::remove_pointer<T>::type;

    /// Function type.
    using Function = void(ObjectType::*)(Args...);

    /// Remove const reference.
    template<typename Type>
    using RemoveConstRef = typename std::remove_const<typename std::remove_reference<Type>::type>::type;

    /// Function name.
    static const char* name;

    /// Function pointer.
    static Function fn;


    /**
     * @brief Convert to PyMethodDef.
     *
     * @return
     */
    static PyMethodDef to() noexcept
    {
        return {name, (PyCFunction) call, METH_VARARGS, nullptr};
    }


    /**
     * @brief Calls object function (helper).
     *
     * @param self
     * @param args
     *
     * @return
     */
    template<int... I>
    static PyObject* call_inner(ObjectWrapper<T>* self, PyObject* args, IntegerSequence<int, I...>) noexcept
    {
        assert(self);
        assert(args);
        assert(fn);

        // Tuple of args
        const Tuple<RemoveConstRef<Args>...> tupleArgs{
            cast<RemoveConstRef<Args>>(PyTuple_GetItem(args, I))...
        };

        try
        {
            (ref(self->value).*fn)(std::get<I>(tupleArgs)...);
        }
        catch (const Exception& e)
        {
            PyErr_SetString(PyExc_RuntimeError, e.what());
            return nullptr;
        }

        return cast().release();
    }


    /**
     * @brief Calls object function (helper).
     *
     * @param self
     * @param args
     *
     * @return
     */
    static PyObject* call_inner(ObjectWrapper<T>* self, PyObject* args, IntegerSequence<int>) noexcept
    {
        assert(self);
        assert(args);
        assert(fn);

        try
        {
            (ref(self->value).*fn)();
        }
        catch (const Exception& e)
        {
            PyErr_SetString(PyExc_RuntimeError, e.what());
            return nullptr;
        }

        return cast().release();
    }


    /**
     * @brief Calls object function.
     *
     * @param self
     * @param args
     * @param closure
     *
     * @return
     */
    static PyObject* call(ObjectWrapper<T>* self, PyObject* args, void* closure) noexcept
    {
        return call_inner(self, args, MakeIntegerSequence<int, 0, sizeof...(Args)>{});
    }
};

/* ************************************************************************ */

/**
 * @brief Member function wrapper.
 *
 * @tparam T    Object type.
 * @tparam Ret  Return type.
 * @tparam Args Argument types.
 */
template<HashValueType Hash, typename T, typename Ret, typename... Args>
struct MemberFunctionConst
{
    /// Plain object type.
    using ObjectType = typename std::remove_pointer<T>::type;

    /// Function type.
    using Function = Ret(ObjectType::*)(Args...) const;

    /// Remove const reference.
    template<typename Type>
    using RemoveConstRef = typename std::remove_const<typename std::remove_reference<Type>::type>::type;


    /// Function name.
    static const char* name;

    /// Function pointer.
    static Function fn;


    /**
     * @brief Convert to PyMethodDef.
     *
     * @return
     */
    static PyMethodDef to() noexcept
    {
        return {name, (PyCFunction) call, METH_VARARGS, nullptr};
    }


    /**
     * @brief Calls object function (helper).
     *
     * @param self
     * @param args
     *
     * @return
     */
    template<int... I>
    static PyObject* call_inner(ObjectWrapper<T>* self, PyObject* args, IntegerSequence<int, I...>) noexcept
    {
        assert(self);
        assert(args);
        assert(fn);

        // Tuple of args
        const Tuple<RemoveConstRef<Args>...> tupleArgs{
            cast<RemoveConstRef<Args>>(PyTuple_GetItem(args, I))...
        };

        try
        {
            return cast<Ret>((ref(self->value).*fn)(std::get<I>(tupleArgs)...)).release();
        }
        catch (const Exception& e)
        {
            PyErr_SetString(PyExc_RuntimeError, e.what());
            return nullptr;
        }
    }


    /**
     * @brief Calls object function.
     *
     * @param self
     * @param args
     * @param closure
     *
     * @return
     */
    static PyObject* call(ObjectWrapper<T>* self, PyObject* args, void* closure) noexcept
    {
        return call_inner(self, args, MakeIntegerSequence<int, 0, sizeof...(Args)>{});
    }
};

/* ************************************************************************ */

/**
 * @brief Define type property.
 *
 * @param name
 * @param fn
 *
 * @return
 */
template<HashValueType Hash, typename T, typename Ret, typename... Args, typename TR = typename std::remove_pointer<T>::type>
PyMethodDef defineMemberFunction(const char* name, Ret(TR::*fn)(Args...)) noexcept
{
    using MemberFunctionType = MemberFunction<Hash, T, Ret, Args...>;

    MemberFunctionType::name = name;
    MemberFunctionType::fn = fn;

    return MemberFunctionType::to();
}

/* ************************************************************************ */

/**
 * @brief Define type property.
 *
 * @param name
 * @param fn
 *
 * @return
 */
template<HashValueType Hash, typename T, typename Ret, typename... Args, typename TR = typename std::remove_pointer<T>::type>
PyMethodDef defineMemberFunction(const char* name, Ret(TR::*fn)(Args...) const) noexcept
{
    using MemberFunctionType = MemberFunctionConst<Hash, T, Ret, Args...>;

    MemberFunctionType::name = name;
    MemberFunctionType::fn = fn;

    return MemberFunctionType::to();
}

/* ************************************************************************ */

/**
 * @brief Object type definition.
 *
 * @tparam T
 */
template<typename T>
struct TypeDefinition
{

    /// Object type.
    using type = ObjectWrapper<T>;


    /// Type definition
    static PyTypeObject definition;


    /**
     * @brief Define type object.
     *
     * @param name
     * @param base
     *
     * @return
     */
    static void init(const char* name, const char* base = nullptr) noexcept
    {
        definition = {
            PyObject_HEAD_INIT(NULL)
            0,                              // ob_size
            name,                           // tp_name
            sizeof(type),                   // tp_basicsize
            0,                              // tp_itemsize
            0,                              // tp_dealloc
            0,                              // tp_print
            0,                              // tp_getattr
            0,                              // tp_setattr
            0,                              // tp_compare
            0,                              // tp_repr
            0,                              // tp_as_number
            0,                              // tp_as_sequence
            0,                              // tp_as_mapping
            0,                              // tp_hash
            0,                              // tp_call
            0,                              // tp_str
            0,                              // tp_getattro
            0,                              // tp_setattro
            0,                              // tp_as_buffer
            Py_TPFLAGS_DEFAULT,             // tp_flags
            nullptr,                        // tp_doc
        };

        if (base)
        {
            const String baseFullName(base);
            const auto sep = baseFullName.find('.');
            String module = baseFullName.substr(0, sep);
            String baseName = baseFullName.substr(sep + 1);

            // Get simulator module
            auto simulatorModule = makeHandle(PyImport_ImportModule(module.c_str()));
            auto dict = PyModule_GetDict(simulatorModule);
            auto moduleClass = makeHandle(PyMapping_GetItemString(dict, const_cast<char*>(baseName.c_str())));
            assert(PyType_Check(moduleClass));

            // Base class
            definition.tp_base = (PyTypeObject*) moduleClass.release();
        }
    }


    /**
     * @brief Finish type definition.
     *
     * @param module
     * @param name
     *
     * @return
     */
    static bool finish(PyObject* module, const char* name) noexcept
    {
        const auto res = ready();

        auto obj = reinterpret_cast<PyObject*>(&definition);

        Py_INCREF(obj);
        PyModule_AddObject(module, name, obj);

        return res;
    }


    /**
     * @brief Ready type.
     *
     * @return
     */
    static bool ready() noexcept
    {
        // Initialize type
        return (PyType_Ready(&definition) >= 0);
    }


    /**
     * @brief Check if definition is valid.
     *
     * @return
     */
    static bool valid() noexcept
    {
        return definition.tp_name != nullptr;
    }


    /**
     * @brief Wrap value.
     *
     * @tparam Source Source type.
     *
     * @param value
     *
     * @return
     */
    template<typename Input>
    static Handle<PyObject> wrap(Input&& value) noexcept
    {
        auto type = findDynamic(typeid(ref(value)));

        if (!type)
            type = &definition;

        if (!type->tp_name)
        {
            PyErr_SetString(PyExc_RuntimeError, "Trying to convert object to undefined type");
            return nullptr;
        }

        // Create new object
        ObjectWrapper<T>* obj = PyObject_New(ObjectWrapper<T>, type);

        // Store value
        obj->value = std::forward<Input>(value);

        return Handle<PyObject>(reinterpret_cast<PyObject*>(obj));
    }


    /**
     * @brief Unwrap value.
     *
     * @param value
     *
     * @return
     */
    template<typename Output>
    static Output unwrap(View<PyObject> value) noexcept
    {
        assert(PyObject_TypeCheck(value, &definition));

        // Cast to wrapper type
        ObjectWrapper<T>* obj = reinterpret_cast<ObjectWrapper<T>*>(value.get());

        return obj->value;
    }
};

/* ************************************************************************ */

template<HashValueType Hash, typename T, typename Val>
const char* Property<Hash, T, Val>::name;

/* ************************************************************************ */

template<HashValueType Hash, typename T, typename Val>
typename Property<Hash, T, Val>::Getter Property<Hash, T, Val>::getFn;

/* ************************************************************************ */

template<HashValueType Hash, typename T, typename Val>
typename Property<Hash, T, Val>::Setter Property<Hash, T, Val>::setFn;

/* ************************************************************************ */

template<HashValueType Hash, typename T, typename Val>
const char* PropertyAlt<Hash, T, Val>::name;

/* ************************************************************************ */

template<HashValueType Hash, typename T, typename Val>
typename PropertyAlt<Hash, T, Val>::Getter PropertyAlt<Hash, T, Val>::getFn;

/* ************************************************************************ */

template<HashValueType Hash, typename T, typename Val>
typename PropertyAlt<Hash, T, Val>::Setter PropertyAlt<Hash, T, Val>::setFn;

/* ************************************************************************ */

template<HashValueType Hash, typename T, typename Ret, typename... Args>
const char* MemberFunction<Hash, T, Ret, Args...>::name;

/* ************************************************************************ */

template<HashValueType Hash, typename T, typename Ret, typename... Args>
typename MemberFunction<Hash, T, Ret, Args...>::Function MemberFunction<Hash, T, Ret, Args...>::fn;

/* ************************************************************************ */

template<HashValueType Hash, typename T, typename... Args>
const char* MemberFunction<Hash, T, void, Args...>::name;

/* ************************************************************************ */

template<HashValueType Hash, typename T, typename... Args>
typename MemberFunction<Hash, T, void, Args...>::Function MemberFunction<Hash, T, void, Args...>::fn;

/* ************************************************************************ */

template<HashValueType Hash, typename T, typename Ret, typename... Args>
const char* MemberFunctionConst<Hash, T, Ret, Args...>::name;

/* ************************************************************************ */

template<HashValueType Hash, typename T, typename Ret, typename... Args>
typename MemberFunctionConst<Hash, T, Ret, Args...>::Function MemberFunctionConst<Hash, T, Ret, Args...>::fn;

/* ************************************************************************ */

template<typename T>
PyTypeObject TypeDefinition<T>::definition;

/* ************************************************************************ */

}
}

/* ************************************************************************ */

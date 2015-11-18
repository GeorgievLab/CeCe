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

// This must be first
#include "cece/plugins/python/Python.hpp"

// CeCe
#include "cece/core/Units.hpp"
#include "cece/core/ViewPtr.hpp"
#include "cece/core/String.hpp"
#include "cece/core/StringView.hpp"

// Plugin
#include "cece/plugins/python/Handle.hpp"
#include "cece/plugins/python/View.hpp"
#include "cece/plugins/python/Type.hpp"
#include "cece/plugins/python/ObjectWrapper.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace python {

/* ************************************************************************ */

/**
 * @brief Return reference to value.
 *
 * @param val
 *
 * @return
 */
template<typename T>
inline T& ref(T& val) noexcept
{
    return val;
}

/* ************************************************************************ */

/**
 * @brief Return reference to value.
 *
 * @param val
 *
 * @return
 */
template<typename T>
inline T& ref(T* val) noexcept
{
    return *val;
}

/* ************************************************************************ */

/**
 * @brief Type cast for scalar types.
 *
 * @tparam Base Base type (one of the base primitive type in Python).
 * @tparam T    Specific type.
 * @tparam Seq  Sequence of characters used to build python object (see Py_BuildValue).
 */
template<typename Base, typename T, char... Seq>
struct ValueCastScalar;

/* ************************************************************************ */

/**
 * @brief Integer base type convertor.
 *
 * @tparam T   Integer type.
 * @tparam Seq Sequence of characters used to build python object (see Py_BuildValue).
 */
template<typename T, char... Seq>
struct ValueCastScalar<long, T, Seq...>
{
    static_assert(std::is_integral<T>::value || std::is_enum<T>::value, "T must be integral or enum");


    /**
     * @brief Check if python object can be converted into integer type.
     *
     * @param value Python object.
     *
     * @return If object can be converted.
     */
    static bool check(View<PyObject> value) noexcept
    {
        return value != nullptr && (PyLong_Check(value) || PyInt_Check(value) || PyFloat_Check(value));
    }


    /**
     * @brief Convert value into python object.
     *
     * @param value Value to convert.
     *
     * @return New python object.
     */
    static Handle<PyObject> convert(T value) noexcept
    {
        static char seq[] = {Seq..., '\0'};
        return Py_BuildValue(seq, static_cast<long>(value));
    }


    /**
     * @brief Convert python object into integer value.
     *
     * @param value Python object view.
     *
     * @return Integer value.
     */
    static T convert(View<PyObject> value) noexcept
    {
        assert(value);
        if (PyLong_Check(value))
            return static_cast<T>(PyLong_AsLong(value));
        else if (PyInt_Check(value))
            return static_cast<T>(PyInt_AsLong(value));
        else if (PyFloat_Check(value))
            return static_cast<T>(PyFloat_AsDouble(value));

        assert(false && "Object is not int or float");
        return T{};
    }
};

/* ************************************************************************ */

/**
 * @brief Double base type convertor.
 *
 * @tparam T   Double type.
 * @tparam Seq Sequence of characters used to build python object (see Py_BuildValue).
 */
template<typename T, char... Seq>
struct ValueCastScalar<double, T, Seq...>
{
    static_assert(std::is_floating_point<T>::value, "T must be floating point");


    /**
     * @brief Check if python object can be converted into floating point type.
     *
     * @param value Python object.
     *
     * @return If object can be converted.
     */
    static bool check(View<PyObject> value) noexcept
    {
        return value != nullptr && (PyFloat_Check(value) || PyLong_Check(value) || PyInt_Check(value));
    }


    /**
     * @brief Convert value into python object.
     *
     * @param value Value to convert.
     *
     * @return New python object.
     */
    static Handle<PyObject> convert(T value) noexcept
    {
        static char seq[] = {Seq..., '\0'};
        return Py_BuildValue(seq, value);
    }


    /**
     * @brief Convert python object into floating point value.
     *
     * @param value Python object view.
     *
     * @return Floating point value.
     */
    static T convert(View<PyObject> value) noexcept
    {
        assert(value);
        if (PyFloat_Check(value))
            return PyFloat_AsDouble(value);
        else if (PyInt_Check(value))
            return PyInt_AsLong(value);
        else if (PyLong_Check(value))
            return PyLong_AsLong(value);

        assert(false && "Object is not int or float");
        return T{};
    }
};

/* ************************************************************************ */

/**
 * @brief Value cast.
 *
 * @tparam T Value type.
 */
template<typename T, typename = void>
struct ValueCast
{

    /**
     * @brief Type.
     */
    using type = T;


    /**
     * @brief Check if python object can be converted.
     *
     * @param value Python object.
     *
     * @return If object can be converted.
     */
    static bool check(ObjectView value)
    {
        auto type = findType(typeid(T));

        return (value != nullptr) && PyObject_TypeCheck(value, type);
    }


    /**
     * @brief Convert value into python object.
     *
     * @param value Value to convert.
     *
     * @return New python object.
     */
    static ObjectHandle convert(T value)
    {
        using BaseType = typename std::remove_pointer<T>::type;

        auto type = findType(typeid(ref(value)));

        if (!std::is_same<BaseType, T>::value && !type)
            type = findType(typeid(BaseType));

        if (!type)
            throw RuntimeException("Trying to convert object to undefined type");

        // Create new object
        ObjectWrapper<T>* obj = PyObject_New(ObjectWrapper<T>, type);

        // Store value
        obj->value = std::move(value);

        return ObjectHandle(reinterpret_cast<PyObject*>(obj));
    }


    /**
     * @brief Convert python object into value.
     *
     * @param value Python object view.
     *
     * @return Value.
     */
    static T convert(ObjectView value)
    {
        if (!value)
            throw InvalidArgumentException("NULL PyObject");

        assert(PyObject_TypeCheck(value, findType(typeid(T))));

        // Cast to wrapper type
        ObjectWrapper<T>* obj = reinterpret_cast<ObjectWrapper<T>*>(value.get());

        return obj->value;
    }

};

/* ************************************************************************ */

/**
 * @brief Value cast for enums.
 *
 * @tparam T Enum type.
 */
template<typename T>
struct ValueCast<T, typename std::enable_if<std::is_enum<T>::value>::type>
    : public ValueCastScalar<long, T, 'i'>
{
    // Nothing
};

/* ************************************************************************ */

/**
 * @brief Integer type convertor.
 */
template<>
struct ValueCast<int> : public ValueCastScalar<long, int, 'i'> {};

/* ************************************************************************ */

/**
 * @brief Unsigned integer type convertor.
 */
template<>
struct ValueCast<unsigned int> : public ValueCastScalar<long, unsigned int, 'I'> {};

/* ************************************************************************ */

/**
 * @brief Unsigned long type convertor.
 */
template<>
struct ValueCast<unsigned long> : public ValueCastScalar<long, unsigned long, 'k'> {};

/* ************************************************************************ */

/**
 * @brief Long long type convertor.
 */
template<>
struct ValueCast<long long> : public ValueCastScalar<long, long long, 'L'> {};

/* ************************************************************************ */

/**
 * @brief Unsigned long long type convertor.
 */
template<>
struct ValueCast<unsigned long long> : public ValueCastScalar<long, unsigned long long, 'K'> {};

/* ************************************************************************ */

/**
 * @brief Float type convertor.
 */
template<>
struct ValueCast<float> : public ValueCastScalar<double, float, 'f'> {};

/* ************************************************************************ */

/**
 * @brief Boolean type convertor.
 */
template<>
struct ValueCast<bool>
{

    /**
     * @brief Check if python object can be converted into boolean type.
     *
     * @param value Python object.
     *
     * @return If object can be converted.
     */
    static bool check(View<PyObject> value) noexcept
    {
        return (value != nullptr) && PyBool_Check(value);
    }


    /**
     * @brief Convert value into python object.
     *
     * @param value Value to convert.
     *
     * @return New python object.
     */
    static Handle<PyObject> convert(bool value) noexcept
    {
        return value ? Py_True : Py_False;
    }


    /**
     * @brief Convert python object into string value.
     *
     * @param value Python object view.
     *
     * @return Boolean value.
     */
    static bool convert(View<PyObject> value) noexcept
    {
        assert(PyBool_Check(value));
        return value == Py_True;
    }
};

/* ************************************************************************ */

/**
 * @brief String type convertor.
 */
template<>
struct ValueCast<String>
{

    /**
     * @brief Check if python object can be converted into string type.
     *
     * @param value Python object.
     *
     * @return If object can be converted.
     */
    static bool check(View<PyObject> value) noexcept
    {
        return (value != nullptr) && PyString_Check(value);
    }


    /**
     * @brief Convert value into python object.
     *
     * @param value Value to convert.
     *
     * @return New python object.
     */
    static Handle<PyObject> convert(const String& value) noexcept
    {
        return PyString_FromString(value.c_str());
    }


    /**
     * @brief Convert python object into string value.
     *
     * @param value Python object view.
     *
     * @return String value.
     */
    static String convert(View<PyObject> value) noexcept
    {
        assert(PyString_Check(value));
        return PyString_AsString(value);
    }
};

/* ************************************************************************ */

/**
 * @brief String type convertor.
 */
template<>
struct ValueCast<StringView>
{

    /**
     * @brief Check if python object can be converted into string type.
     *
     * @param value Python object.
     *
     * @return If object can be converted.
     */
    static bool check(View<PyObject> value) noexcept
    {
        return (value != nullptr) && PyString_Check(value);
    }


    /**
     * @brief Convert value into python object.
     *
     * @param value Value to convert.
     *
     * @return New python object.
     */
    static Handle<PyObject> convert(const StringView& value) noexcept
    {
        return PyString_FromStringAndSize(value.getData(), value.getLength());
    }


    /**
     * @brief Convert python object into string value.
     *
     * @param value Python object view.
     *
     * @return String value.
     */
    static String convert(View<PyObject> value) noexcept
    {
        assert(PyString_Check(value));
        return PyString_AsString(value);
    }
};

/* ************************************************************************ */

/**
 * @brief Units type convertor.
 *
 * @tparam Nom
 * @tparam Denom
 */
template<typename Nom, typename Denom>
struct ValueCast<units::Unit<Nom, Denom>> : public ValueCast<units::Value>
{

    /**
     * @brief Convert value into python object.
     *
     * @param value Value to convert.
     *
     * @return New python object.
     */
    static Handle<PyObject> convert(units::Unit<Nom, Denom> value) noexcept
    {
        return ValueCast<units::Value>::convert(value.value());
    }


    /**
     * @brief Convert python object into units value.
     *
     * @param value Python object view.
     *
     * @return Units value.
     */
    static units::Unit<Nom, Denom> convert(View<PyObject> value) noexcept
    {
        return units::Unit<Nom, Denom>(ValueCast<units::Value>::convert(value));
    }
};

/* ************************************************************************ */

/**
 * @brief Value cast for enums.
 *
 * @tparam T Enum type.
 */
template<typename T>
struct ValueCast<ViewPtr<T>>
{

    /**
     * @brief Check if python object can be converted into boolean type.
     *
     * @param value Python object.
     *
     * @return If object can be converted.
     */
    static bool check(View<PyObject> value) noexcept
    {
        return ValueCast<T*>::check(value);
    }


    /**
     * @brief Convert value into python object.
     *
     * @param value Value to convert.
     *
     * @return New python object.
     */
    static Handle<PyObject> convert(ViewPtr<T> value) noexcept
    {
        return ValueCast<T*>::convert(value.get());
    }


    /**
     * @brief Convert python object into string value.
     *
     * @param value Python object view.
     *
     * @return Boolean value.
     */
    static ViewPtr<T> convert(View<PyObject> value) noexcept
    {
        return ValueCast<T*>::convert(value);
    }
};

/* ************************************************************************ */

/**
 * @brief Cast python object into value.
 *
 * @param object Object to cast.
 *
 * @return Result value.
 */
template<typename T>
inline bool check(View<PyObject> object)
{
    return ValueCast<T>::check(object);
}

/* ************************************************************************ */

/**
 * @brief Cast python object into value.
 *
 * @param object Object to cast.
 *
 * @return Result value.
 */
template<typename T>
inline auto cast(PyObject* object) -> decltype(ValueCast<T>::convert(View<PyObject>(object)))
{
    return ValueCast<T>::convert(View<PyObject>(object));
}

/* ************************************************************************ */

/**
 * @brief Cast python object into value.
 *
 * @param object Object to cast.
 *
 * @return Result value.
 */
template<typename T>
inline auto cast(View<PyObject> object) -> decltype(ValueCast<T>::convert(object))
{
    return ValueCast<T>::convert(object);
}

/* ************************************************************************ */

/**
 * @brief Cast value into python object.
 *
 * @param value Source value.
 *
 * @return New python object.
 */
template<typename T>
inline Handle<PyObject> cast(T value)
{
    return ValueCast<T>::convert(value);
}

/* ************************************************************************ */

/**
 * @brief Create empty return object.
 *
 * @return New python object.
 */
inline Handle<PyObject> cast() noexcept
{
    Py_RETURN_NONE;
}

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */

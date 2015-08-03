/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */

#pragma once

/* ************************************************************************ */

// Simulator
#include "core/compatibility.hpp"
#include "core/Units.hpp"
#include "core/String.hpp"

// Module
#include "Python.hpp"
#include "Handle.hpp"
#include "View.hpp"

/* ************************************************************************ */

namespace plugin {
namespace python {

/* ************************************************************************ */

template<typename T>
struct TypeDefinition;

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
    static_assert(std::is_integral<T>::value, "T must be integral");


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
        return Py_BuildValue(seq, value);
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
            return PyLong_AsLong(value);
        else if (PyInt_Check(value))
            return PyInt_AsLong(value);
        else if (PyFloat_Check(value))
            return PyFloat_AsDouble(value);
        else
            assert(false && "Object is not int or float");
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
        else
            assert(false && "Object is not int or float");
    }
};

/* ************************************************************************ */

/**
 * @brief Value cast.
 *
 * @tparam T Value type.
 */
template<typename T>
struct ValueCast
{

    /**
     * @brief Type.
     */
    using type = T;


    /**
     * @brief Type without const and reference.
     */
    using plain_type = typename std::remove_const<typename std::remove_reference<T>::type>::type;


    /**
     * @brief Type definiton.
     */
    using definition = TypeDefinition<plain_type>;


    /**
     * @brief Check if python object can be converted.
     *
     * @param value Python object.
     *
     * @return If object can be converted.
     */
    static bool check(View<PyObject> value)
    {
        return (value != nullptr) && PyObject_TypeCheck(value, &definition::definition);
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
        assert(definition::valid);
        return definition::wrap(value);
    }


    /**
     * @brief Convert python object into value.
     *
     * @param value Python object view.
     *
     * @return Value.
     */
    static plain_type convert(View<PyObject> value) noexcept
    {
        assert(definition::valid);
        return definition::unwrap(value);
    }
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
 * @brief Cast python object into value.
 *
 * @param object Object to cast.
 *
 * @return Result value.
 */
template<typename T>
inline bool check(View<PyObject> object) noexcept
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
inline auto cast(PyObject* object) noexcept -> decltype(ValueCast<T>::convert(View<PyObject>(object)))
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
inline auto cast(View<PyObject> object) noexcept -> decltype(ValueCast<T>::convert(object))
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
inline Handle<PyObject> cast(T value) noexcept
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

/* ************************************************************************ */

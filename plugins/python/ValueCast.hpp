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
    static bool check(View<PyObject> value) NOEXCEPT
    {
        return value != nullptr && PyLong_Check(value);
    }


    /**
     * @brief Convert value into python object.
     *
     * @param value Value to convert.
     *
     * @return New python object.
     */
    static Handle<PyObject> convert(T value) NOEXCEPT
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
    static T convert(View<PyObject> value) NOEXCEPT
    {
        assert(value);
        assert(PyLong_Check(value));
        return PyLong_AsLong(value);
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
    static bool check(View<PyObject> value) NOEXCEPT
    {
        return value != nullptr && PyFloat_Check(value);
    }


    /**
     * @brief Convert value into python object.
     *
     * @param value Value to convert.
     *
     * @return New python object.
     */
    static Handle<PyObject> convert(T value) NOEXCEPT
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
    static T convert(View<PyObject> value) NOEXCEPT
    {
        assert(value);
        assert(PyFloat_Check(value));
        return PyFloat_AsDouble(value);
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
    static Handle<PyObject> convert(T value) NOEXCEPT
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
    static plain_type convert(View<PyObject> value) NOEXCEPT
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
    static bool check(View<PyObject> value) NOEXCEPT
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
    static Handle<PyObject> convert(const String& value) NOEXCEPT
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
    static String convert(View<PyObject> value) NOEXCEPT
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
    static Handle<PyObject> convert(units::Unit<Nom, Denom> value) NOEXCEPT
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
    static units::Unit<Nom, Denom> convert(View<PyObject> value) NOEXCEPT
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
inline bool check(View<PyObject> object) NOEXCEPT
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
inline auto cast(View<PyObject> object) NOEXCEPT -> decltype(ValueCast<T>::convert(object))
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
inline Handle<PyObject> cast(T value) NOEXCEPT
{
    return ValueCast<T>::convert(value);
}

/* ************************************************************************ */

/**
 * @brief Create empty return object.
 *
 * @return New python object.
 */
inline Handle<PyObject> cast() NOEXCEPT
{
    Py_RETURN_NONE;
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

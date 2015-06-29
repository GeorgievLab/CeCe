/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */

// Python requires to be included first because it sets some parameters to stdlib
#include "Python.hpp"

/* ************************************************************************ */

// Declaration
#include "wrapper_core.hpp"

// Core
#include "core/Vector.hpp"
#include "core/Units.hpp"

// Module
#include "wrapper.hpp"
#include "Exception.hpp"

/* ************************************************************************ */

using namespace module::python;

/* ************************************************************************ */

template<typename T>
static void python_wrapper_core_Vector(const char* fullname, PyObject* module)
{
    const char* name = fullname + 5;

    using type = Vector<T>;
    using type_def = TypeDefinition<type>;

    static PyGetSetDef properties[] = {
        defineProperty<1, type>("x", &type::getX, &type::setX),
        defineProperty<2, type>("y", &type::getY, &type::setY),
        defineProperty<3, type>("width", &type::getWidth),
        defineProperty<4, type>("height", &type::getHeight),
        {NULL}  /* Sentinel */
    };

    type_def::init(fullname);
    type_def::definition.tp_init = Constructor<type, T, T>::to();
    type_def::definition.tp_getset = properties;
    type_def::ready();

    Py_INCREF(&type_def::definition);
    PyModule_AddObject(module, name, reinterpret_cast<PyObject*>(&type_def::definition));
}

/* ************************************************************************ */

void python_wrapper_core()
{
    PyObject* module = Py_InitModule3("core", nullptr, nullptr);

    python_wrapper_core_Vector<float>("core.VectorFloat", module);
    python_wrapper_core_Vector<int>("core.VectorInt", module);
    python_wrapper_core_Vector<unsigned int>("core.VectorUint", module);
    python_wrapper_core_Vector<units::Length>("core.PositionVector", module);
    python_wrapper_core_Vector<units::Velocity>("core.VelocityVector", module);
}

/* ************************************************************************ */


/* ************************************************************************ */

// Declaration
#include "wrapper_core.hpp"

// Boost
#include <boost/python/class.hpp>

// Core
#include "core/Vector.hpp"

/* ************************************************************************ */

void python_wrapper_core()
{
    namespace py = boost::python;

    // Vector<float>
    py::class_<core::Vector<float>>("VectorFloat")
        .def(py::init<float, float>())
        .def(py::init<float>())
        .def("getX", &core::Vector<float>::getX)
        .def("getY", &core::Vector<float>::getY)
        .def("setX", &core::Vector<float>::setX)
        .def("setY", &core::Vector<float>::setY)
    ;

    // Vector<unsigned>
    py::class_<core::Vector<unsigned>>("VectorUint")
        .def(py::init<unsigned, unsigned>())
        .def(py::init<unsigned>())
        .def("getX", &core::Vector<unsigned>::getX)
        .def("getY", &core::Vector<unsigned>::getY)
        .def("setX", &core::Vector<unsigned>::setX)
        .def("setY", &core::Vector<unsigned>::setY)
    ;
}

/* ************************************************************************ */

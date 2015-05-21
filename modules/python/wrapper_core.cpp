
/* ************************************************************************ */

// Declaration
#include "wrapper_core.hpp"

// Boost
#include <boost/python/class.hpp>
#include <boost/python/operators.hpp>

// Core
#include "core/Vector.hpp"

/* ************************************************************************ */

/**
 * @brief Define Vector for different type.
 *
 * @param name Class name.
 */
template<typename T>
static void define_vector(const char* name) noexcept
{
    using namespace boost::python;

    bool (Vector<T>::*inRange)(const Vector<T>&, const Vector<T>&) const = &core::Vector<T>::inRange;

    class_<core::Vector<T>>(name)
        .def(init<T, T>())
        .def(init<T>())
        .add_property("x", &core::Vector<T>::getX, &core::Vector<T>::setX)
        .add_property("y", &core::Vector<T>::getY, &core::Vector<T>::setY)
        // Add
        .def(self + self)
        .def(self + T())
        .def(T() + self)
        .def(self += self)
        // Substract
        .def(self - self)
        .def(self - T())
        .def(T() - self)
        .def(self -= self)
        // Multiple
        .def(self * self)
        .def(self * T())
        .def(T() * self)
        .def(self *= self)
        // Divide
        .def(self / self)
        .def(self / T())
        .def(T() / self)
        .def(self /= self)
        // Relation
        .def(self == self)
        .def(self != self)
        .def(self < self)
        .def(self <= self)
        .def(self > self)
        .def(self >= self)

        .def("inRange", inRange)
        .def("length", &core::Vector<T>::getLength)
        .def("lengthSquared", &core::Vector<T>::getLengthSquared)
        .def("dot", &core::Vector<T>::dot)
        .def("rotated", &core::Vector<T>::rotated)
    ;
}

/* ************************************************************************ */

void python_wrapper_core()
{
    // Predefined types
    define_vector<float>("VectorFloat");
    define_vector<int>("VectorInt");
    define_vector<unsigned int>("VectorUint");
}

/* ************************************************************************ */

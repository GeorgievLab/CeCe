
/* ************************************************************************ */

// Declaration
#include "wrapper_core.hpp"

// Python
#include <Python.h>

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

}

/* ************************************************************************ */

static void python_wrapper_core_Vector()
{
    // Predefined types
    define_vector<float>("VectorFloat");
    define_vector<int>("VectorInt");
    define_vector<unsigned int>("VectorUint");
}

/* ************************************************************************ */

void python_wrapper_core()
{
    PyObject* module = Py_InitModule3("render", nullptr, nullptr);


    python_wrapper_core_Vector();
}

/* ************************************************************************ */

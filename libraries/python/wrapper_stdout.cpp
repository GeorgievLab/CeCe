/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */

// Python requires to be included first because it sets some parameters to stdlib
#include "Python.hpp"

/* ************************************************************************ */

// Declaration
#include "wrapper_stdout.hpp"

// C++
#include <iostream>

/* ************************************************************************ */

/**
 * @brief Write Python string object to std::cout.
 *
 * @param self
 * @param args
 *
 * @return
 */
static PyObject* log_write(PyObject* self, PyObject* args)
{
    const char* what;

    if (!PyArg_ParseTuple(args, "s", &what))
        return nullptr;

    if (!strcmp(what, "\n"))
        std::cout << std::endl;
    else
        std::cout << what;

    // Return None
    Py_RETURN_NONE;
}

/* ************************************************************************ */

static const PyMethodDef log_methods[] = {
    {"write", log_write, METH_VARARGS, "Write string to std::cout."},
    {NULL, NULL, 0, NULL}
};

/* ************************************************************************ */

void python_wrapper_stdout()
{
    PyObject* m = Py_InitModule("cppout", const_cast<PyMethodDef*>(log_methods));

    if (m == nullptr)
        return;

    PySys_SetObject(const_cast<char*>("stdout"), m);
}

/* ************************************************************************ */

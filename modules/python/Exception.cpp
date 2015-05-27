
// Declaration
#include "Exception.hpp"

// Python
#include <Python.h>

// Module
#include "Handle.hpp"

/* ************************************************************************ */

namespace module {
namespace python {

/* ************************************************************************ */

Exception::Exception()
{
    Handle<PyObject> exc;
    Handle<PyObject> val;
    Handle<PyObject> tb;

    PyErr_Fetch(&exc.getRef(), &val.getRef(), &tb.getRef());
    PyErr_NormalizeException(&exc.getRef(), &val.getRef(), &tb.getRef());

    if(val)
    {
        auto traceback = makeHandle(PyImport_ImportModule("traceback"));
        auto format_exception = makeHandle(PyObject_GetAttrString(traceback, "format_exception"));
        auto formatted_list = makeHandle(PyObject_CallFunctionObjArgs(format_exception, exc.get(), val.get(), tb.get(), NULL));
        auto formatted = makeHandle(PyUnicode_Join(makeString(""), formatted_list));

        m_message = PyString_AsString(formatted);
    }
    else
    {
        m_message = PyString_AsString(exc);
    }
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

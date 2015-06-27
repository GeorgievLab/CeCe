/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */

// Python requires to be included first because it sets some parameters to stdlib
#include "Python.hpp"

/* ************************************************************************ */

// Declaration
#include "Exception.hpp"

// C++
#include <cassert>

// Module
#include "Handle.hpp"
#include "Utils.hpp"

/* ************************************************************************ */

namespace module {
namespace python {

/* ************************************************************************ */

Exception::Exception()
{
    assert(PyErr_Occurred());

#ifdef _MSC_VER
    PyObject* exc;
    PyObject* val;
    PyObject* tb;

    PyErr_Fetch(&exc, &val, &tb);
    PyErr_NormalizeException(&exc, &val, &tb);

    Handle<PyObject> hexc(exc);
    Handle<PyObject> hval(val);
    Handle<PyObject> htb(tb);
#else
    Handle<PyObject> hexc;
    Handle<PyObject> hval;
    Handle<PyObject> htb;

    PyErr_Fetch(&hexc.getRef(), &hval.getRef(), &htb.getRef());
    PyErr_NormalizeException(&hexc.getRef(), &hval.getRef(), &htb.getRef());
#endif

    assert(hexc);

    if (hval)
    {
        if (htb)
        {
            auto traceback = makeHandle(PyImport_ImportModule("traceback"));
            auto format_exception = makeHandle(PyObject_GetAttrString(traceback, "format_exception"));
            auto formatted_list = call(format_exception, hexc, hval, htb);
            auto formatted = makeHandle(PyUnicode_Join(str(""), formatted_list));
            m_message = PyString_AsString(formatted);
        }
        else
        {
            auto str = makeHandle(PyObject_Str(hval));
            m_message = PyString_AsString(str);
        }
    }
    else
    {
        auto str = makeHandle(PyObject_Str(hexc));
        m_message = PyString_AsString(str);
    }
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

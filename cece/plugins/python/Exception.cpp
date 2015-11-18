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

// This must be first
#include "cece/plugins/python/Python.hpp"

// Declaration
#include "cece/plugins/python/Exception.hpp"

// CeCe
#include "cece/core/Assert.hpp"

// Plugin
#include "cece/plugins/python/Handle.hpp"
#include "cece/plugins/python/Utils.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace python {

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

/**
 * @brief Returns python formatted error message.
 *
 * @return Formatted error message.
 */
String get_message() noexcept
{
    Assert(PyErr_Occurred());

    Handle<PyObject> hexc;
    Handle<PyObject> hval;
    Handle<PyObject> htb;

    PyErr_Fetch(&hexc.getRef(), &hval.getRef(), &htb.getRef());
    PyErr_NormalizeException(&hexc.getRef(), &hval.getRef(), &htb.getRef());

    Assert(hexc);

    if (hval)
    {
        if (htb)
        {
            auto traceback = makeHandle(PyImport_ImportModule("traceback"));
            auto format_exception = makeHandle(PyObject_GetAttrString(traceback, "format_exception"));
            auto formatted_list = call(format_exception, hexc, hval, htb);
            auto formatted = makeHandle(PyUnicode_Join(str(""), formatted_list));
            return PyString_AsString(formatted);
        }
        else
        {
            auto str = makeHandle(PyObject_Str(hval));
            return PyString_AsString(str);
        }
    }
    else
    {
        auto str = makeHandle(PyObject_Str(hexc));
        return PyString_AsString(str);
    }
}

/* ************************************************************************ */

}

/* ************************************************************************ */

Exception::Exception()
{
    m_message = get_message();
}

/* ************************************************************************ */

Exception::Exception(const char* error)
{
    PyErr_SetString(PyExc_TypeError, error);
    m_message = get_message();
}

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */

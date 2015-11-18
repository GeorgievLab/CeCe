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
#include "cece/plugins/python/Source.hpp"

// Plugin
#include "cece/plugins/python/Exception.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace python {

/* ************************************************************************ */

Source::Source()
    : m_dictionary(PyDict_New())
{
    auto main = makeView(PyImport_AddModule("__main__"));
    assert(main);

    auto main_dict = makeHandle(PyObject_GetAttrString(main, "__dict__"));
    assert(main_dict);

    auto main_builtins = makeView(PyDict_GetItemString(main_dict, "__builtins__"));
    assert(main_builtins);

    // Set buildins
    PyDict_SetItemString(m_dictionary, "__builtins__", main_builtins);
}

/* ************************************************************************ */

Handle<PyObject> Source::getFunction(const char* name) const noexcept
{
    // Attribute not found
    if (!PyMapping_HasKeyString(m_dictionary, const_cast<char*>(name)))
        return nullptr;

    return PyMapping_GetItemString(m_dictionary, const_cast<char*>(name));
}

/* ************************************************************************ */

void Source::initSource(const String& source)
{
    makeHandle(PyImport_ImportModule("cppout"));
    makeHandle(PyImport_ImportModule("core"));
    makeHandle(PyImport_ImportModule("simulator"));

    // Execute given module file
    if (!makeHandle(PyRun_String(source.c_str(), Py_file_input, m_dictionary, nullptr)))
        throw Exception();

    m_initialized = true;
}

/* ************************************************************************ */

void Source::initFile(const FilePath& filename)
{
    makeHandle(PyImport_ImportModule("cppout"));

    const auto path = filename.string();

    // Execute given module file
    if (!makeHandle(PyRun_FileEx(fopen(path.c_str(), "r"), path.c_str(), Py_file_input, m_dictionary, nullptr, 1)))
        throw Exception();

    m_initialized = true;
}

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */

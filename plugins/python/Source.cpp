/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */

// Python requires to be included first because it sets some parameters to stdlib
#include "Python.hpp"

/* ************************************************************************ */

// Declaration
#include "Source.hpp"

// Module
#include "Exception.hpp"

/* ************************************************************************ */

namespace plugin {
namespace python {

/* ************************************************************************ */

Source::Source()
    : m_dictionary(PyDict_New())
{
    // Nothing to do
}

/* ************************************************************************ */

Handle<PyObject> Source::getFunction(const char* name) const NOEXCEPT
{
    // Attribute not found
    if (!PyMapping_HasKeyString(m_dictionary.get(), const_cast<char*>(name)))
        return nullptr;

    return PyMapping_GetItemString(m_dictionary.get(), const_cast<char*>(name));
}

/* ************************************************************************ */

void Source::initSource(const std::string& source)
{
    makeHandle(PyImport_ImportModule("cppout"));
    makeHandle(PyImport_ImportModule("core"));
    makeHandle(PyImport_ImportModule("render"));
    makeHandle(PyImport_ImportModule("simulator"));

    // Execute given module file
    if (!makeHandle(PyRun_String(source.c_str(), Py_file_input, m_dictionary.get(), nullptr)))
        throw Exception();

    m_initialized = true;
}

/* ************************************************************************ */

void Source::initFile(const std::string& filename)
{
    makeHandle(PyImport_ImportModule("cppout"));

    // Execute given module file
    if (!makeHandle(PyRun_FileEx(fopen(filename.c_str(), "r"), filename.c_str(), Py_file_input, m_dictionary, nullptr, 1)))
        throw Exception();

    m_initialized = true;
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

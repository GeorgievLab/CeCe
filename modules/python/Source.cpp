
// Declaration
#include "Source.hpp"

// Module
#include "Exception.hpp"

/* ************************************************************************ */

namespace module {
namespace python {

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
    // Import module
    m_module = PyImport_ImportModule("__main__");

    // Get dictionary with list of objects
    m_dictionary = PyObject_GetAttrString(m_module.get(), "__dict__");

    makeHandle(PyImport_ImportModule("cppout"));
    //makeHandle(PyImport_ImportModule("core"));
    //makeHandle(PyImport_ImportModule("simulator"));

    // Execute given module file
    if (!makeHandle(PyRun_String(source.c_str(), Py_single_input, m_dictionary.get(), nullptr)))
        throw Exception();
}

/* ************************************************************************ */

void Source::initFile(const std::string& filename)
{
    // Import module
    m_module = PyImport_ImportModule("__main__");

    // Get dictionary with list of objects
    m_dictionary = PyObject_GetAttrString(m_module, "__dict__");

    makeHandle(PyImport_ImportModule("cppout"));

    // Execute given module file
    if (!makeHandle(PyRun_FileEx(fopen(filename.c_str(), "r"), filename.c_str(), Py_file_input, m_dictionary, nullptr, 1)))
        throw Exception();
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

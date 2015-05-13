
// Declaration
#include "Module.hpp"

// C++
#include <cassert>
#include <stdexcept>

// Simulation
#include "core/Log.hpp"
#include "simulator/Library.hpp"
#include "simulator/Simulation.hpp"

// Boost
#include <boost/filesystem.hpp>
#include <boost/python.hpp>

/* ************************************************************************ */

namespace module {
namespace python {

/* ************************************************************************ */

Module::Module(simulator::Simulation& simulation, const boost::filesystem::path& filename)
{
    // Initialize python
    Py_Initialize();
/*
    boost::filesystem::path foundPath;

    // Foreach possible paths
    for (auto p : simulator::Library::getLibraryPaths())
    {
        if (boost::filesystem::exists(p / filename))
        {
            foundPath = p / filename;
            break;
        }
    }

    if (foundPath.empty())
        throw std::invalid_argument("Unable to find: " + filename.string());

    FILE* fp = fopen(foundPath.c_str(), "r");
    assert(fp != NULL);
*/
    if (filename.empty())
    {
        Log::warning("Python wrapper module filename is empty!");
        return;
    }

    // Build the name object
    PyObject* name = PyString_FromString(filename.stem().c_str());

    // Load the module object
    m_module = PyImport_Import(name);

    // Don't need name anymore
    Py_DECREF(name);

    if (!m_module)
    {
        PyErr_Print();
        return;
    }

    // Get dictionary with list of objects
    PyObject* dict = PyModule_GetDict(m_module);

    // Get function pointers
    m_configureFn = PyDict_GetItemString(dict, "configure");

    // Check if is a function
    if (m_configureFn && !PyCallable_Check(m_configureFn))
        PyErr_Print();

    m_updateFn = PyDict_GetItemString(dict, "update");

    // Check if is a function
    if (m_updateFn && !PyCallable_Check(m_updateFn))
        PyErr_Print();

}

/* ************************************************************************ */

Module::~Module()
{
    // Free module
    Py_XDECREF(m_module);

    // Finalize python
    Py_Finalize();
}

/* ************************************************************************ */

void Module::update(units::Duration dt, simulator::Simulation& simulation)
{
    if (!m_updateFn)
        return;

    PyObject_CallObject(m_updateFn, NULL);
}

/* ************************************************************************ */

void Module::configure(const simulator::ConfigurationBase& config)
{
    if (!m_configureFn)
        return;

    PyObject_CallObject(m_configureFn, NULL);
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

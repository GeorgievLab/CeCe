
// Declaration
#include "Module.hpp"

// C++
#include <stdexcept>

// Simulation
#include "simulator/Simulation.hpp"

// Boost
#include <boost/python.hpp>

/* ************************************************************************ */

namespace module {
namespace python {

/* ************************************************************************ */

Module::Module(simulator::Simulation& simulation, const boost::filesystem::path& filename)
{
    // Initialize python
    Py_Initialize();

    FILE* fp = fopen(filename.c_str(), "r");

    if (fp == NULL)
        throw std::invalid_argument("Unable to open: " + filename.string());

    // Compile source code
    m_compiledCode = PyNode_Compile(
        PyParser_SimpleParseFile(fp, filename.c_str(), Py_file_input), filename.c_str()
    );

    fclose(fp);
}

/* ************************************************************************ */

Module::~Module()
{
    // Free code
    Py_XDECREF(m_compiledCode);

    // Finalize python
    Py_Finalize();
}

/* ************************************************************************ */

void Module::update(units::Duration dt, simulator::Simulation& simulation)
{
    PyObject* globals = PyDict_New();
    PyObject* locals = PyDict_New();

    // Call update function
    PyObject* res = PyEval_EvalCode(m_compiledCode, globals, locals);
    Py_XDECREF(res);

    Py_XDECREF(locals);
    Py_XDECREF(globals);
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

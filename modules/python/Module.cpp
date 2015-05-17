
// Declaration
#include "Module.hpp"

// C++
#include <cassert>
#include <stdexcept>

// Simulation
#include "core/Log.hpp"
#include "simulator/Simulation.hpp"
#include "simulator/Configuration.hpp"
#include "simulator/Library.hpp"

// Boost
#include <boost/filesystem.hpp>

// Wrappers
#include "wrapper_core.hpp"
#include "wrapper_render.hpp"
#include "wrapper_parser.hpp"
#include "wrapper_simulator.hpp"

/* ************************************************************************ */

BOOST_PYTHON_MODULE(core) {
    python_wrapper_core();
}

/* ************************************************************************ */

BOOST_PYTHON_MODULE(simulator) {
    python_wrapper_simulator();
}

/* ************************************************************************ */

BOOST_PYTHON_MODULE(parser) {
    python_wrapper_parser();
}

/* ************************************************************************ */

BOOST_PYTHON_MODULE(render) {
    python_wrapper_render();
}

/* ************************************************************************ */

namespace module {
namespace python {

/* ************************************************************************ */

/**
 * @brief Parse current python exception.
 *
 * @return Error string.
 *
 * @see https://wiki.python.org/moin/boost.python/EmbeddingPython
 */
static std::string parse_python_exception() noexcept
{
    namespace py = boost::python;

    PyObject* exc;
    PyObject* val;
    PyObject* tb;

    PyErr_Fetch(&exc, &val, &tb);
    PyErr_NormalizeException(&exc, &val, &tb);
    py::handle<> hexc(exc);
    py::handle<> hval(py::allow_null(val));
    py::handle<> htb(py::allow_null(tb));

    if(!hval)
    {
        return py::extract<std::string>(py::str(hexc));
    }
    else
    {
        py::object traceback(py::import("traceback"));
        py::object format_exception(traceback.attr("format_exception"));
        py::object formatted_list(format_exception(hexc, hval, htb));
        py::object formatted(py::str("").join(formatted_list));
        return py::extract<std::string>(formatted);
    }
}

/* ************************************************************************ */

Module::Module(simulator::Simulation& simulation, const boost::filesystem::path& filename)
{
    namespace py = boost::python;
    namespace fs = boost::filesystem;

    PyImport_AppendInittab("core", &initcore);
    PyImport_AppendInittab("simulator", &initsimulator);
    PyImport_AppendInittab("render", &initrender);
    PyImport_AppendInittab("parser", &initparser);

    // Initialize python
    Py_Initialize();

    fs::path foundPath;

    // Foreach possible paths
    for (auto p : simulator::Library::getLibraryPaths())
    {
        if (fs::exists(p / filename))
        {
            foundPath = p / filename;
            break;
        }
    }

    if (foundPath.empty())
        throw std::invalid_argument("Unable to find: " + filename.string());

    if (filename.empty())
    {
        Log::warning("Python wrapper module filename is empty!");
        return;
    }

    try
    {
        // Load the module object
        m_module = py::import("__main__");

        // Get dictionary with list of objects
        auto dict = m_module.attr("__dict__");

        // Execute given module file
        py::exec_file(foundPath.c_str(), dict);

        // Get function pointers
        m_configureFn = dict["configure"];
        m_updateFn = dict["update"];

#if ENABLE_RENDER
        m_drawInitFn = dict["drawInit"];
        m_drawFn = dict["draw"];
#endif

    }
    catch (const py::error_already_set&)
    {
        throw std::runtime_error(parse_python_exception());
    }
}

/* ************************************************************************ */

Module::~Module()
{
    // Finalize python
    //Py_Finalize();
}

/* ************************************************************************ */

void Module::update(units::Duration dt, simulator::Simulation& simulation)
{
    if (!m_updateFn)
        return;

    try
    {
        m_updateFn(boost::python::object(dt), boost::python::ptr(&simulation));
    }
    catch (const boost::python::error_already_set&)
    {
        throw std::runtime_error(parse_python_exception());
    }
}

/* ************************************************************************ */

void Module::configure(const simulator::ConfigurationBase& config)
{
    if (!m_configureFn)
        return;

    try
    {
        m_configureFn(boost::python::ptr(&config));
    }
    catch (const boost::python::error_already_set&)
    {
        throw std::runtime_error(parse_python_exception());
    }
}

/* ************************************************************************ */

#ifdef ENABLE_RENDER
void Module::drawInit(render::Context& context)
{
    if (!m_drawInitFn)
        return;

    try
    {
        m_drawInitFn(boost::python::ptr(&context));
    }
    catch (const boost::python::error_already_set&)
    {
        throw std::runtime_error(parse_python_exception());
    }
}
#endif

/* ************************************************************************ */

#ifdef ENABLE_RENDER
void Module::draw(render::Context& context, const simulator::Simulation& simulation)
{
    if (!m_drawFn)
        return;

    try
    {
        m_drawFn(boost::python::ptr(&context), boost::python::ptr(&simulation));
    }
    catch (const boost::python::error_already_set&)
    {
        throw std::runtime_error(parse_python_exception());
    }
}
#endif

/* ************************************************************************ */

}
}

/* ************************************************************************ */

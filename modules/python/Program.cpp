
// Declaration
#include "Program.hpp"

// C++
#include <cassert>
#include <stdexcept>

// Simulation
#include "core/Log.hpp"

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

Module::Module(const boost::filesystem::path& filename)
{
    namespace py = boost::python;
    namespace fs = boost::filesystem;

    PyImport_AppendInittab("core", &initcore);
    PyImport_AppendInittab("simulator", &initsimulator);
    PyImport_AppendInittab("render", &initrender);
    PyImport_AppendInittab("parser", &initparser);

    // Initialize python
    Py_Initialize();

    // External file
    auto ext = filename.extension();
    if (filename.has_extension() && ext == ".py")
    {
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
        {
            core::Log::warning("Unable to find: " + filename.string());
        }
        else
        {
            try
            {
                // Load the module object
                m_module = py::import("__main__");

                // Get dictionary with list of objects
                auto dict = m_module.attr("__dict__");

                // Execute given module file
                py::exec_file(foundPath.c_str(), dict);

                // Get function pointers
                if (py::dict(dict).has_key("configure")) {
                    m_configureFn = dict["configure"];
                }

                if (py::dict(dict).has_key("update")) {
                    m_updateFn = dict["update"];
                }

#if ENABLE_RENDER
                if (py::dict(dict).has_key("draw")) {
                    m_drawFn = dict["draw"];
                }
#endif
            }
            catch (const py::error_already_set&)
            {
                throw std::runtime_error(parse_python_exception());
            }
        }
    }
}

/* ************************************************************************ */

Module::~Module()
{
    // Finalize python
    //Py_Finalize();
}

/* ************************************************************************ */

void Module::update(core::units::Duration dt, simulator::Simulation& simulation)
{
    namespace py = boost::python;

    if (!m_updateFn)
        return;

    try
    {
        m_updateFn(py::object(dt), py::ptr(&simulation));
    }
    catch (const py::error_already_set&)
    {
        throw std::runtime_error(parse_python_exception());
    }
}

/* ************************************************************************ */

void Module::configure(const simulator::ConfigurationBase& config)
{
    namespace py = boost::python;

    // Check if configuration contains code
    if (config.hasText())
    {
        if (m_module)
            core::Log::warning("Overriding external script by internal code");

        try
        {
            // Load the module object
            m_module = py::import("__main__");

            // Get dictionary with list of objects
            auto dict = m_module.attr("__dict__");

            // Execute given module file
            py::exec(py::str(config.getText()), dict);

            // Get function pointers
            if (py::dict(dict).has_key("configure")) {
                m_configureFn = dict["configure"];
            }

            if (py::dict(dict).has_key("update")) {
                m_updateFn = dict["update"];
            }

#if ENABLE_RENDER
            if (py::dict(dict).has_key("draw")) {
                m_drawFn = dict["draw"];
            }
#endif
        }
        catch (const py::error_already_set&)
        {
            throw std::runtime_error(parse_python_exception());
        }
    }

    if (!m_configureFn)
        return;

    try
    {
        m_configureFn(py::ptr(&config));
    }
    catch (const py::error_already_set&)
    {
        throw std::runtime_error(parse_python_exception());
    }
}

/* ************************************************************************ */

#if ENABLE_RENDER
void Module::draw(render::Context& context, const simulator::Simulation& simulation)
{
    namespace py = boost::python;

    if (!m_drawFn)
        return;

    try
    {
        m_drawFn(py::ptr(&context), py::ptr(&simulation));
    }
    catch (const py::error_already_set&)
    {
        throw std::runtime_error(parse_python_exception());
    }
}
#endif

/* ************************************************************************ */

}
}

/* ************************************************************************ */

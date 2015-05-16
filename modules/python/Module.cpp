
// Declaration
#include "Module.hpp"

// C++
#include <cassert>
#include <stdexcept>

// Simulation
#include "core/Log.hpp"
#include "simulator/Library.hpp"
#include "simulator/Simulation.hpp"
#include "simulator/Configuration.hpp"

#if ENABLE_RENDER
// Render
#include "render/Color.hpp"
#include "render/Context.hpp"
#include "render/GridColor.hpp"
#endif

// Boost
#include <boost/filesystem.hpp>

/* ************************************************************************ */

BOOST_PYTHON_MODULE(core) {
    namespace py = boost::python;

    // Vector<float>
    py::class_<core::Vector<float>>("VectorFloat")
        .def(py::init<float, float>())
        .def(py::init<float>())
        .def("getX", &core::Vector<float>::getX)
        .def("getY", &core::Vector<float>::getY)
        .def("setX", &core::Vector<float>::setX)
        .def("setY", &core::Vector<float>::setY)
    ;

    // Vector<unsigned>
    py::class_<core::Vector<unsigned>>("VectorUint")
        .def(py::init<unsigned, unsigned>())
        .def(py::init<unsigned>())
        .def("getX", &core::Vector<unsigned>::getX)
        .def("getY", &core::Vector<unsigned>::getY)
        .def("setX", &core::Vector<unsigned>::setX)
        .def("setY", &core::Vector<unsigned>::setY)
    ;
}

/* ************************************************************************ */

BOOST_PYTHON_MODULE(simulator) {
    namespace py = boost::python;

    // Configuration
    py::class_<simulator::ConfigurationBase, boost::noncopyable>("Configuration", py::no_init)
        .def("getString", &simulator::ConfigurationBase::getString)
        .def("getFloat", &simulator::ConfigurationBase::getFloat)
        .def("getInteger", &simulator::ConfigurationBase::getInteger)
    ;

    // Simulation
    py::class_<simulator::Simulation, boost::noncopyable>("Simulation", py::no_init)
        //.add_property("worldSize", &simulator::Simulation::getWorldSize, py::return_internal_reference<>())
    ;
}

/* ************************************************************************ */

#if ENABLE_RENDER
BOOST_PYTHON_MODULE(render) {
    namespace py = boost::python;

    py::class_<render::Color>("Color")
        .def(py::init<render::Color::ComponentType>())
        .def(py::init<render::Color::ComponentType, render::Color::ComponentType, render::Color::ComponentType, render::Color::ComponentType>())
        .add_property("red", &render::Color::getRed, &render::Color::setRed)
        .add_property("green", &render::Color::getGreen, &render::Color::setGreen)
        .add_property("blue", &render::Color::getBlue, &render::Color::setBlue)
        .add_property("alpha", &render::Color::getAlpha, &render::Color::setAlpha)
    ;

    void (render::Context::*matrixScale1)(float) = &render::Context::matrixScale;
    void (render::Context::*matrixScale2)(const Vector<float>&) = &render::Context::matrixScale;

    // render::Context
    py::class_<render::Context, boost::noncopyable>("Context", py::no_init)
        .def("matrixPush", &render::Context::matrixPush)
        .def("matrixPop", &render::Context::matrixPop)
        .def("matrixIdentity", &render::Context::matrixIdentity)
        .def("matrixScale", matrixScale1)
        .def("matrixScale", matrixScale2)
    ;

    void (render::GridColor::*init_1)(render::Context&) = &render::GridColor::init;
    void (render::GridColor::*init_2)(render::Context&, Vector<render::GridColor::PositionType>) = &render::GridColor::init;

    py::class_<render::GridColor, boost::noncopyable>("GridColor")
        .def("init", init_1)
        .def("init", init_2)
        .def("draw", &render::GridColor::draw)
        //.def("set", &render::GridColor::set)
        //.def("get", &render::GridColor::get)
    ;
}
#endif

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

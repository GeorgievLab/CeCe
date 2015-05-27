
// Declaration
#include "Module.hpp"

// C++
#include <cassert>
#include <string>
#include <fstream>

// Simulation
#include "core/Log.hpp"
#include "simulator/Simulation.hpp"
#include "simulator/Configuration.hpp"
#include "simulator/Library.hpp"

// Module
#include "Exception.hpp"

/* ************************************************************************ */

namespace module {
namespace python {

/* ************************************************************************ */

Module::Module(const std::string& name)
{
    auto ends_with = [](const std::string& value, const std::string& ending) {
        if (ending.size() > value.size()) return false;
        return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
    };

    auto file_exists = [](const std::string& path) {
        std::ifstream f(path.c_str());
        return f.good();
    };

    // External file
    if (ends_with(name, ".py"))
    {
        std::string foundPath;

        // Foreach possible paths
        for (const auto& p : simulator::Library::getLibraryPaths())
        {
            auto path = p + "/" + name;

            if (file_exists(path))
            {
                foundPath = path;
                break;
            }
        }

        if (foundPath.empty())
        {
            core::Log::warning("Unable to find: " + name);
        }
        else
        {
            m_source.initFile(name);
        }
    }
}

/* ************************************************************************ */

void Module::configure(const simulator::ConfigurationBase& config)
{
    // Check if configuration contains code
    if (config.hasText())
    {
        if (m_source.isInitialized())
            core::Log::warning("Overriding external script by internal code");

        // Initialize from source text
        m_source.initSource(config.getText());
    }

    // Get function pointers
    m_configureFn = m_source.getFunction("configure");
    m_updateFn = m_source.getFunction("update");

#if ENABLE_RENDER
    m_drawFn = m_source.getFunction("draw");
#endif

    if (!m_configureFn)
        return;

    // m_configureFn(py::ptr(&config));
    if (!makeHandle(PyObject_CallFunctionObjArgs(m_configureFn.get(), NULL)))
        throw Exception();
}

/* ************************************************************************ */

void Module::update(core::units::Duration dt, simulator::Simulation& simulation)
{
    if (!m_updateFn)
        return;

    // m_updateFn(py::object(dt), py::ptr(&simulation));
    if (!makeHandle(PyObject_CallFunctionObjArgs(m_updateFn.get(), NULL)))
        throw Exception();
}

/* ************************************************************************ */

#if ENABLE_RENDER
void Module::draw(render::Context& context, const simulator::Simulation& simulation)
{
    if (!m_drawFn)
        return;

    // m_drawFn(py::ptr(&context), py::ptr(&simulation));
    if (!makeHandle(PyObject_CallFunctionObjArgs(m_drawFn.get(), NULL)))
        throw Exception();
}
#endif

/* ************************************************************************ */

}
}

/* ************************************************************************ */

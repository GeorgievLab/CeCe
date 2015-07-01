/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */

// Python requires to be included first because it sets some parameters to stdlib
#include "Python.hpp"

/* ************************************************************************ */

// Declaration
#include "Object.hpp"

// C++
#include <cassert>
#include <string>

// Simulation
#include "core/Log.hpp"
#include "core/FilePath.hpp"
#include "simulator/Simulation.hpp"
#include "simulator/Configuration.hpp"
#include "simulator/Plugin.hpp"

// Module
#include "Utils.hpp"
#include "Exception.hpp"

/* ************************************************************************ */

namespace plugin {
namespace python {

/* ************************************************************************ */

Object::Object(simulator::Simulation& simulation, const std::string& name, Type type)
    : simulator::Object(simulation, type)
{
    auto ends_with = [](const std::string& value, const std::string& ending) {
        if (ending.size() > value.size()) return false;
        return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
    };

    // External file
    if (ends_with(name, ".py"))
    {
        std::string foundPath;

        // Foreach possible paths
        for (const auto& p : simulator::Plugin::getLibraryPaths())
        {
            auto path = p + "/" + name;

            if (fileExists(path))
            {
                foundPath = path;
                break;
            }
        }

        if (foundPath.empty())
        {
            Log::warning("Unable to find: " + name);
        }
        else
        {
            m_source.initFile(name);
        }
    }
}

/* ************************************************************************ */

void Object::configure(const simulator::Configuration& config, simulator::Simulation& simulation)
{
    // Check if configuration contains code
    if (config.hasText())
    {
        if (m_source.isInitialized())
            Log::warning("Overriding external script by internal code");

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

    // Call configure
    if (!call(m_configureFn, static_cast<simulator::Object*>(this), &config))
        throw Exception();
}

/* ************************************************************************ */

void Object::update(units::Duration dt)
{
    if (!m_updateFn)
        return;

    // Call update
    if (!call(m_updateFn, static_cast<simulator::Object*>(this), dt.value()))
        throw Exception();
}

/* ************************************************************************ */

#if ENABLE_RENDER
void Object::draw(render::Context& context)
{
    if (!m_drawFn)
        return;

    // Call draw
    if (!call(m_drawFn, static_cast<simulator::Object*>(this), &context))
        throw Exception();
}
#endif

/* ************************************************************************ */

}
}

/* ************************************************************************ */

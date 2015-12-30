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
#include "cece/plugins/python/Object.hpp"

// CeCe
#include "cece/core/Log.hpp"
#include "cece/core/String.hpp"
#include "cece/core/FilePath.hpp"
#include "cece/config/Configuration.hpp"
#include "cece/plugin/Manager.hpp"
#include "cece/simulator/Simulation.hpp"

// Plugin
#include "cece/plugins/python/Utils.hpp"
#include "cece/plugins/python/Exception.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace python {

/* ************************************************************************ */

Object::Object(simulator::Simulation& simulation, const String& name, Type type)
    : object::Object(simulation, "python.Object", type)
{
    auto ends_with = [](const std::string& value, const std::string& ending) {
        if (ending.size() > value.size()) return false;
        return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
    };

    // External file
    if (ends_with(name, ".py"))
    {
        FilePath foundPath;

        // Foreach possible paths
        for (const auto& p : plugin::Manager::s().getDirectories())
        {
            auto path = p / name;

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

void Object::configure(const config::Configuration& config, simulator::Simulation& simulation)
{
    // Check if configuration contains code
    if (config.hasContent())
    {
        if (m_source.isInitialized())
            Log::warning("Overriding external script by internal code");

        // Initialize from source text
        m_source.initSource(config.getContent());
    }

    // Get function pointers
    m_configureFn = m_source.getFunction("configure");
    m_updateFn = m_source.getFunction("update");

#ifdef CECE_ENABLE_RENDER
    m_drawFn = m_source.getFunction("draw");
#endif

    if (!m_configureFn)
        return;

    // Call configure
    if (!call(m_configureFn, static_cast<object::Object*>(this), &config))
        throw Exception();
}

/* ************************************************************************ */

void Object::update(units::Duration dt)
{
    if (!m_updateFn)
        return;

    // Call update
    if (!call(m_updateFn, static_cast<object::Object*>(this), dt.value()))
        throw Exception();
}

/* ************************************************************************ */

#ifdef CECE_ENABLE_RENDER
void Object::draw(render::Context& context)
{
    if (!m_drawFn)
        return;

    // Call draw
    if (!call(m_drawFn, static_cast<object::Object*>(this), &context))
        throw Exception();
}
#endif

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */

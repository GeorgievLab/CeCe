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

// Declaration
#include "cece/plugins/python/Module.hpp"

// CeCe
#include "cece/core/Log.hpp"
#include "cece/core/String.hpp"
#include "cece/config/Configuration.hpp"
#include "cece/simulator/Simulation.hpp"

// Plugin
#include "cece/plugins/python/Exception.hpp"
#include "cece/plugins/python/Utils.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace python {

/* ************************************************************************ */

void Module::loadConfig(simulator::Simulation& simulation, const config::Configuration& config)
{
    // Configure parent
    module::Module::loadConfig(simulation, config);

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
    if (!call(m_configureFn, &config))
        throw Exception();
}

/* ************************************************************************ */

void Module::update(simulator::Simulation& simulation, units::Time dt)
{
    if (!m_updateFn)
        return;

    // Call update
    if (!call(m_updateFn, dt.value(), &simulation))
        throw Exception();
}

/* ************************************************************************ */

#ifdef CECE_ENABLE_RENDER
void Module::draw(const simulator::Simulation& simulation, render::Context& context)
{
    if (!m_drawFn)
        return;

    // Call draw
    if (!call(m_drawFn, &context, &simulation))
        throw Exception();
}
#endif

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */

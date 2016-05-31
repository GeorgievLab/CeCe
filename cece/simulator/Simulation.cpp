/* ************************************************************************ */
/* Georgiev Lab (c) 2015-2016                                               */
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
#include "cece/simulator/Simulation.hpp"

// C++
#include <utility>

// CeCe
#include "cece/core/UnitIo.hpp"
#include "cece/plugin/Api.hpp"
#include "cece/init/Initializer.hpp"
#include "cece/module/Module.hpp"
#include "cece/object/Type.hpp"
#include "cece/object/Object.hpp"
#include "cece/simulator/Visualization.hpp"

/* ************************************************************************ */

namespace cece {
namespace simulator {

/* ************************************************************************ */

Simulation::~Simulation() = default;

/* ************************************************************************ */

ViewPtr<plugin::Api> Simulation::loadPlugin(const config::Configuration& config)
{
    // Get plugin name
    const String name = config.get("name");

    // Load plugin with given name name
    auto plugin = loadPlugin(name);

    if (plugin)
        plugin->loadConfig(*this, config);

    return plugin;
}

/* ************************************************************************ */

ViewPtr<init::Initializer> Simulation::createInitializer(const config::Configuration& config)
{
    // Get initializer language/type
    const String name = config.has("language")
        ? config.get("language")
        : config.get("type")
    ;

    // Create initializer with given name
    auto initializer = createInitializer(name);

    // Configure initializer
    if (initializer)
        initializer->loadConfig(*this, config);

    return initializer;
}

/* ************************************************************************ */

ViewPtr<module::Module> Simulation::createModule(const config::Configuration& config)
{
    // Get module language/type/name
    const String name = config.has("language")
        ? config.get("language")
        : config.has("type")
            ? config.get("type")
            : config.get("name")
    ;

    // Create module
    auto module = createModule(name);

    // Configure module
    if (module)
        module->loadConfig(config);

    return module;
}

/* ************************************************************************ */

ViewPtr<object::Type> Simulation::createObjectType(const config::Configuration& config)
{
    // Get object type name
    const String name = config.get("name");

    // Create object type
    auto type = createObjectType(name);

    // Configure object type
    if (type)
    {
        //type->loadConfig(config);
        type->baseName = config.get("basename");
        type->config = config.toMemory();
    }

    return type;
}

/* ************************************************************************ */

ViewPtr<object::Object> Simulation::createObject(const config::Configuration& config)
{
    // Get object type
    const String type = config.has("type")
        ? config.get("type")
        : config.get("class")
    ;

    // Create object
    auto object = createObject(type);

    // Configure object
    if (object)
        //object->loadConfig(config);
        object->configure(config, *this);

    return object;
}

/* ************************************************************************ */

ViewPtr<program::Program> Simulation::createProgram(const config::Configuration& config)
{
    // Get program language/type
    const String type = config.has("language")
        ? config.get("language")
        : config.get("type")
    ;

    // Create program
    auto program = createProgram(config.get("name"), type);

    // Configure program
    if (program)
        program->loadConfig(*this, config);

    return program;
}

/* ************************************************************************ */

void Simulation::loadConfig(const config::Configuration& config)
{
    setWorldSize(config.get<SizeVector>("world-size"));
    setTimeStep(config.get<units::Time>("dt"));
    setIterations(config.get("iterations", getIterations()));

#ifdef CECE_ENABLE_RENDER
    {
        // Allow to specify visualization parameters in simulation configuration
        auto& visualization = getVisualization();
        visualization.setEnabled(config.get("visualized", visualization.isEnabled()));
        visualization.setBackgroundColor(config.get("background", visualization.getBackgroundColor()));

        auto visualizations = config.getConfigurations("visualization");
        if (!visualizations.empty())
            visualization.loadConfig(visualizations.front());
    }
#endif

    // Parse parameters
    for (auto&& cfg : config.getConfigurations("parameter"))
    {
        // Get parameter name
        const auto name = cfg.get("name");

        // Do not override previously defined parameters
        if (hasParameter(name))
            continue;

        if (!cfg.has("value"))
        {
            throw InvalidArgumentException(
                "Missing parameter value. Define it in simulation file or as application argument"
            );
        }

        // Store new parameter
        setParameter(std::move(name), cfg.get("value"));
    }

    // Parse plugins
    for (auto&& cfg : config.getConfigurations("plugin"))
        loadPlugin(cfg);

    // Register user types
    for (auto&& cfg : config.getConfigurations("type"))
        createObjectType(cfg);

    // Parse init
    for (auto&& cfg : config.getConfigurations("init"))
        createInitializer(cfg);

    // Parse modules
    for (auto&& cfg : config.getConfigurations("module"))
        createModule(cfg);

    // Parse programs
    for (auto&& cfg : config.getConfigurations("program"))
        createProgram(cfg);

    // Parse objects
    for (auto&& cfg : config.getConfigurations("object"))
        createObject(cfg);
}

/* ************************************************************************ */

void Simulation::storeConfig(config::Configuration& config) const
{
    // TODO: implement
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

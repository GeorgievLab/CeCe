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
#include "cece/plugin/Context.hpp"

// CeCe
#include "cece/core/Log.hpp"
#include "cece/core/Exception.hpp"
#include "cece/plugin/Repository.hpp"
#include "cece/plugin/Manager.hpp"
#include "cece/plugin/Api.hpp"
#include "cece/loader/Loader.hpp"
#include "cece/init/Initializer.hpp"
#include "cece/module/Module.hpp"
#include "cece/object/Object.hpp"
#include "cece/program/Program.hpp"
#include "cece/simulator/Simulation.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

/**
 * @brief Find factory name in all plugins to give a hint for user.
 *
 * @param name
 * @param repo
 * @param fn
 *
 * @return
 */
template<typename RecordFn>
DynamicArray<String> getPluginNames(StringView name, const Repository& repo, RecordFn fn)
{
    DynamicArray<String> names;
    const auto& manager = repo.getManager();

    // Foreach repository records
    for (const auto& record : repo.getRecords())
    {
        const auto* mgr = fn(record.second);
        Assert(mgr);

        if (mgr->exists(name))
            names.push_back(String(manager.getName(record.first)));
    }

    return names;
}

/* ************************************************************************ */

/**
 * @brief Join strings from array.
 *
 * @param array
 *
 * @return
 */
String join(const DynamicArray<String>& array) noexcept
{
    String res;

    for (auto it = array.begin(); it != array.end(); ++it)
    {
        if (it != array.begin())
            res += ", ";

        res += *it;
    }

    return res;
}

/* ************************************************************************ */

}

/* ************************************************************************ */

bool Context::isImported(StringView name) const noexcept
{
    auto it = m_plugins.find(String(name));
    return it != m_plugins.end();
}

/* ************************************************************************ */

ViewPtr<const Api> Context::importPlugin(StringView name)
{
    // Do not add duplicates
    auto it = m_plugins.find(String(name));

    // Already imported
    if (it != m_plugins.end())
        return it->second;

    // Load plugin
    auto api = getRepository().getManager().getApi(name);

    // Plugin not found
    if (!api)
        throw RuntimeException("Cannot import plugin: " + String(name));

    // Check conflict plugins
    for (const auto& pluginName : api->conflictPlugins())
    {
        if (isImported(pluginName))
            throw RuntimeException("Cannot import plugin: " + String(name) + ". Confliction with plugin: " + pluginName);
    }

    // Import required plugins
    for (const auto& pluginName : api->requiredPlugins())
        importPlugin(pluginName);

    // Store API
    m_plugins.emplace(String(name), api);

    return api;
}

/* ************************************************************************ */

ViewPtr<const Api> Context::removePlugin(StringView name)
{
    // Find plugin
    auto it = m_plugins.find(String(name));

    // Nothing to unload
    if (it == m_plugins.end())
        return nullptr;

    // Store api to return
    auto api = it->second;

    // Remove plugin
    m_plugins.erase(it);

    return api;
}

/* ************************************************************************ */

UniquePtr<simulator::Simulation> Context::createSimulation(const FilePath& filepath, ViewPtr<const Parameters> parameters) const
{
    // File extension
    auto ext = filepath.extension().string().substr(1);

    // Foreach loaded APIs and find factory
    for (auto plugin : m_plugins)
    {
        // Get factory manager
        auto factoryManager = getRepository().getLoaderFactoryManager(plugin.second);

        // Exists factory
        if (factoryManager && factoryManager->exists(ext))
        {
            // Find loader by extension
            auto loader = factoryManager->createLoader(ext);
            Assert(loader);

            // Create a simulation
            return loader->fromFile(getRepository(), filepath, parameters);
        }
    }

    throw RuntimeException("Unable to load file with extension: '" + ext + "'");
}

/* ************************************************************************ */

UniquePtr<simulator::Simulation> Context::createSimulation(StringView type, StringView source) const
{
    // Foreach loaded APIs and find factory
    for (auto plugin : m_plugins)
    {
        // Get factory manager
        auto factoryManager = getRepository().getLoaderFactoryManager(plugin.second);

        // Exists factory
        if (factoryManager && factoryManager->exists(type))
        {
            // Find loader
            auto loader = factoryManager->createLoader(type);
            Assert(loader);

            // Create a simulation
            return loader->fromSource(getRepository(), String(source));
        }
    }

    throw RuntimeException("Unable to find loader '" + String(type) + "'");
}

/* ************************************************************************ */

UniquePtr<simulator::Simulation> Context::createSimulation(StringView type, StringView source, const FilePath& filepath) const
{
    // Foreach loaded APIs and find factory
    for (auto plugin : m_plugins)
    {
        // Get factory manager
        auto factoryManager = getRepository().getLoaderFactoryManager(plugin.second);

        // Exists factory
        if (factoryManager && factoryManager->exists(type))
        {
            // Find loader
            auto loader = factoryManager->createLoader(type);
            Assert(loader);

            // Create a simulation
            return loader->fromSource(getRepository(), String(source), filepath);
        }
    }

    throw RuntimeException("Unable to find loader: " + String(type));
}

/* ************************************************************************ */

UniquePtr<init::Initializer> Context::createInitializer(StringView typeName) const
{
    ViewPtr<const init::FactoryManager> resultFactoryManager;
    DynamicArray<String> importedNames;

    // Foreach loaded APIs and find factory
    for (auto plugin : m_plugins)
    {
        // Get factory manager
        auto factoryManager = getRepository().getInitFactoryManager(plugin.second);

        // Exists factory
        if (factoryManager && factoryManager->exists(typeName))
        {
            resultFactoryManager = factoryManager;
            importedNames.push_back(plugin.first);
        }
    }

    // Multiple imports
    if (importedNames.size() > 1)
    {
        throw RuntimeException(
            "Multiple definition of module: " + String(typeName) + ". Module found in imported plugin(s): " + join(importedNames)
        );
    }

    // Factory manager selected
    if (resultFactoryManager)
        return resultFactoryManager->createInitializer(typeName);

    // Find in other APIs to give a hint to user
    auto names = getPluginNames(typeName, getRepository(), [](const RepositoryRecord& rec) {
        return &(rec.getModuleFactoryManager());
    });

    throw RuntimeException(
        "Cannot create initializer: " + String(typeName) + ". Initializer found in plugin(s): " + join(names)
    );
}

/* ************************************************************************ */

UniquePtr<module::Module> Context::createModule(StringView typeName, simulator::Simulation& simulation) const
{
    ViewPtr<const module::FactoryManager> resultFactoryManager;
    DynamicArray<String> importedNames;

    // Foreach loaded APIs and find factory
    for (auto plugin : m_plugins)
    {
        // Get factory manager
        auto factoryManager = getRepository().getModuleFactoryManager(plugin.second);

        // Exists factory
        if (factoryManager && factoryManager->exists(typeName))
        {
            resultFactoryManager = factoryManager;
            importedNames.push_back(plugin.first);
        }
    }

    // Multiple imports
    if (importedNames.size() > 1)
    {
        throw RuntimeException(
            "Multiple definition of module: " + String(typeName) + ". Module found in imported plugin(s): " + join(importedNames)
        );
    }

    // Factory manager selected
    if (resultFactoryManager)
        return resultFactoryManager->createModule(typeName, simulation);

    // Find in other APIs to give a hint to user
    auto names = getPluginNames(typeName, getRepository(), [](const RepositoryRecord& rec) {
        return &(rec.getModuleFactoryManager());
    });

    throw RuntimeException(
        "Cannot create module: " + String(typeName) + ". Module found in plugin(s): " + join(names)
    );
}

/* ************************************************************************ */

UniquePtr<object::Object> Context::createObject(StringView typeName, simulator::Simulation& simulation, object::Object::Type type) const
{
    ViewPtr<const object::FactoryManager> resultFactoryManager;
    DynamicArray<String> importedNames;

    // Foreach loaded APIs and find factory
    for (auto plugin : m_plugins)
    {
        // Get factory manager
        auto factoryManager = getRepository().getObjectFactoryManager(plugin.second);

        // Exists factory
        if (factoryManager && factoryManager->exists(typeName))
        {
            resultFactoryManager = factoryManager;
            importedNames.push_back(plugin.first);
        }
    }

    // Multiple imports
    if (importedNames.size() > 1)
    {
        throw RuntimeException(
            "Multiple definition of object: " + String(typeName) + ". Object found in imported plugin(s): " + join(importedNames)
        );
    }

    // Factory manager selected
    if (resultFactoryManager)
        return resultFactoryManager->createObject(typeName, simulation, type);

    // Find in other APIs to give a hint to user
    auto names = getPluginNames(typeName, getRepository(), [](const RepositoryRecord& rec) {
        return &(rec.getObjectFactoryManager());
    });

    throw RuntimeException(
        "Cannot create object: " + String(typeName) + ". Object found in plugin(s): " + join(names)
    );
}

/* ************************************************************************ */

UniquePtr<program::Program> Context::createProgram(StringView typeName) const
{
    ViewPtr<const program::FactoryManager> resultFactoryManager;
    DynamicArray<String> importedNames;

    // Foreach loaded APIs and find factory
    for (auto plugin : m_plugins)
    {
        // Get factory manager
        auto factoryManager = getRepository().getProgramFactoryManager(plugin.second);

        // Exists factory
        if (factoryManager && factoryManager->exists(typeName))
        {
            resultFactoryManager = factoryManager;
            importedNames.push_back(plugin.first);
        }
    }

    // Multiple imports
    if (importedNames.size() > 1)
    {
        throw RuntimeException(
            "Multiple definition of program: " + String(typeName) + ". Program found in imported plugin(s): " + join(importedNames)
        );
    }

    // Factory manager selected
    if (resultFactoryManager)
        return resultFactoryManager->createProgram(typeName);

    // Find in other APIs to give a hint to user
    auto names = getPluginNames(typeName, getRepository(), [](const RepositoryRecord& rec) {
        return &(rec.getProgramFactoryManager());
    });

    throw RuntimeException(
        "Cannot create program: " + String(typeName) + ". Program found in plugin(s): " + join(names)
    );
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

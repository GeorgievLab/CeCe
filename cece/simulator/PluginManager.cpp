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
#include "PluginManager.hpp"

// C++
#include <cstdint>
#include <cstdlib>
#include <regex>

// Boost
#include <boost/filesystem.hpp>

// CeCe
#include "cece/core/Log.hpp"
#include "cece/core/Exception.hpp"
#include "cece/core/FilePath.hpp"
#include "cece/core/Range.hpp"
#include "cece/simulator/PluginApi.hpp"
#include "cece/loader/Loader.hpp"
#include "cece/loader/FactoryManager.hpp"

/* ************************************************************************ */

namespace cece {
namespace simulator {

/* ************************************************************************ */

#define ITEM(name, validname) extern "C" simulator::PluginApi* PLUGIN_PROTOTYPE_NAME_BUILTIN(create, validname)();
BUILTIN_PLUGINS
#undef ITEM

/* ************************************************************************ */

#define ITEM(name, validname) { # name, PLUGIN_PROTOTYPE_NAME_BUILTIN(create, validname) },
const Map<String, Plugin::CreateFn> PluginManager::s_builtin{
    BUILTIN_PLUGINS
};
#undef ITEM

/* ************************************************************************ */

void PluginManager::addDirectory(String path)
{
    Log::debug("New plugins directory: `", path, "`");

    m_directories.push_back(std::move(path));
}

/* ************************************************************************ */

DynamicArray<String> PluginManager::getNamesBuiltin() noexcept
{
    DynamicArray<String> names;
    names.reserve(s_builtin.size());

    for (const auto& p : s_builtin)
        names.push_back(p.first);

    return names;
}

/* ************************************************************************ */

DynamicArray<String> PluginManager::getNamesExtern() noexcept
{
    DynamicArray<String> names;
    names.reserve(m_extern.size());

    for (const auto& p : m_extern)
        names.push_back(p.first);

    return names;
}

/* ************************************************************************ */

DynamicArray<String> PluginManager::getNames() noexcept
{
    auto names = getNamesBuiltin();
    auto namesExtern = getNamesExtern();
    names.reserve(names.size() + namesExtern.size());

    std::move(namesExtern.begin(), namesExtern.end(), std::back_inserter(names));

    return names;
}

/* ************************************************************************ */

bool PluginManager::isLoaded(const String& name) noexcept
{
    return m_loaded.find(name) != m_loaded.end();
}

/* ************************************************************************ */

ViewPtr<PluginApi> PluginManager::getApi(const String& name) noexcept
{
    // Try to find library
    auto it = m_loaded.find(name);

    if (it == m_loaded.end())
        return nullptr;

    return std::get<1>(*it).getApi();
}

/* ************************************************************************ */

ViewPtr<PluginApi> PluginManager::load(const String& name)
{
    return loadInternal(name).getApi();
}

/* ************************************************************************ */

void PluginManager::init()
{
    rescanDirectories();

    auto addUnique = [this](const String& name) {
        if (std::find(m_loadOrder.begin(), m_loadOrder.end(), name) == m_loadOrder.end()) {
            m_loadOrder.push_back(name);
        }
    };

    // Load all plugins
    for (const auto& name : getNames())
    {
        auto api = loadInternal(name).getApi();

        // List of required plugins.
        auto plugins = api->requiredPlugins();

        for (const auto& pluginName : plugins)
            addUnique(pluginName);

        // Add this plugin
        addUnique(name);
    }

    loadPlugins();
}

/* ************************************************************************ */

UniquePtr<Simulation> PluginManager::createSimulation(const FilePath& filepath)
{
    // File extension
    auto ext = filepath.extension().string().substr(1);

    // Find loader by extension
    auto loader = getContext().getLoaderFactoryManager().create(ext);

    if (!loader)
        throw RuntimeException("Unable to load file with extension: " + ext);

    // Create simulation
    return loader->fromFile(getContext(), filepath);
}

/* ************************************************************************ */

Map<String, FilePath> PluginManager::scanDirectory(const FilePath& directory) noexcept
{
    using namespace boost::filesystem;

    Map<String, FilePath> result;

    // Regular expression
    const String pattern = Plugin::FILE_PREFIX + "(.*)" + Plugin::FILE_EXTENSION;
    std::regex regex(pattern);
    std::smatch matches;

    Log::debug("Scanning `", directory.string(), "` for plugins with pattern `", pattern, "`");

    if (!is_directory(directory))
    {
        Log::warning("Directory `", directory.string(), "` doesn't exists");
        return result;
    }

    // Foreach directory
    for (const auto& entry : makeRange(directory_iterator(directory), directory_iterator()))
    {
        // Only files
        if (!is_regular_file(entry))
            continue;

        // Get path
        auto path = entry.path();
        const auto filename = path.filename().string();

        // Match file name
        if (!std::regex_match(filename, matches, regex))
            continue;

        if (matches.size() == 2)
            result.emplace(matches[1].str(), std::move(path));
    }

    return result;
}

/* ************************************************************************ */

Map<String, FilePath> PluginManager::scanDirectories() noexcept
{
    using namespace boost::filesystem;

    Map<String, FilePath> result;

    // Foreach directories
    for (const auto& dirName : getDirectories())
    {
        auto tmp = scanDirectory(dirName);
        result.insert(make_move_iterator(tmp.begin()), make_move_iterator(tmp.end()));
    }

    return result;
}

/* ************************************************************************ */

void PluginManager::loadPlugins()
{
    for (const auto& name : m_loadOrder)
    {
        auto it = m_loaded.find(name);

        // Found
        if (it == m_loaded.end())
            throw RuntimeException("Plugin '" + name + "' not found");

        it->second.getApi()->onLoad(m_context);
    }
}

/* ************************************************************************ */

void PluginManager::unloadPlugins()
{
    for (auto i = m_loadOrder.rbegin(); i != m_loadOrder.rend(); ++i)
    {
        auto it = m_loaded.find(*i);
        Assert(it != m_loaded.end());
        it->second.getApi()->onUnload(m_context);
    }
}

/* ************************************************************************ */

Plugin& PluginManager::loadInternal(const String& name)
{
    // Try to find library in cache
    auto it = m_loaded.find(name);

    // Found
    if (it != m_loaded.end())
        return std::get<1>(*it);

    // Try to find in builtin libraries
    auto itBuiltin = s_builtin.find(name);

    // Required library is builtin
    if (itBuiltin != s_builtin.end())
    {
        Log::debug("Loading builtin plugin `", name, "`...");

        // Insert into cache
        auto ptr = m_loaded.emplace(std::piecewise_construct,
            std::forward_as_tuple(name),
            std::forward_as_tuple(name, UniquePtr<PluginApi>(itBuiltin->second()))
        );

        return std::get<1>(*std::get<0>(ptr));
    }
    else
    {
        Log::debug("Loading external plugin `", name, "`...");

        // Load extern
        auto it = m_extern.find(name);

        if (it == m_extern.end())
            throw InvalidArgumentException("Unable to find plugin '" + name + "'");

        // Insert into cache
        auto ptr = m_loaded.emplace(std::piecewise_construct,
            std::forward_as_tuple(name),
            std::forward_as_tuple(name, it->second)
        );

        return std::get<1>(*std::get<0>(ptr));
    }
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

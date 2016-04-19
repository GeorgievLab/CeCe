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
#include "Manager.hpp"

// C++
#include <utility>

// Boost
#include <boost/filesystem.hpp>

// CeCe
#include "cece/core/Log.hpp"
#include "cece/core/Exception.hpp"
#include "cece/core/Range.hpp"
#include "cece/loader/Loader.hpp"
#include "cece/loader/FactoryManager.hpp"
#include "cece/plugin/Api.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {

/* ************************************************************************ */

#define ITEM(name, validname) extern "C" plugin::Api* PLUGIN_PROTOTYPE_NAME_BUILTIN(create, validname)();
BUILTIN_PLUGINS
#undef ITEM

/* ************************************************************************ */

#define ITEM(name, validname) { # name, PLUGIN_PROTOTYPE_NAME_BUILTIN(create, validname) },
const Map<String, Library::CreateFn> Manager::s_builtin{
    BUILTIN_PLUGINS
};
#undef ITEM

/* ************************************************************************ */

DynamicArray<String> Manager::getNamesBuiltin() const noexcept
{
    DynamicArray<String> names;
    names.reserve(s_builtin.size());

    for (const auto& p : s_builtin)
        names.push_back(p.first);

    return names;
}

/* ************************************************************************ */

DynamicArray<String> Manager::getNamesExtern() const noexcept
{
    DynamicArray<String> names;
    names.reserve(m_extern.size());

    for (const auto& p : m_extern)
        names.push_back(p.first);

    return names;
}

/* ************************************************************************ */

DynamicArray<String> Manager::getNames() const noexcept
{
    auto names = getNamesBuiltin();
    auto namesExtern = getNamesExtern();
    names.reserve(names.size() + namesExtern.size());

    std::move(namesExtern.begin(), namesExtern.end(), std::back_inserter(names));

    return names;
}

/* ************************************************************************ */

void Manager::addDirectory(FilePath path)
{
    Log::debug("New plugins directory: `", path, "`");

    m_directories.push_back(std::move(path));

    // Scan added directory
    auto plugins = scanDirectory(path);

    // Append plugin directories
    m_extern.insert(make_move_iterator(plugins.begin()), make_move_iterator(plugins.end()));
}

/* ************************************************************************ */

void Manager::loadAll()
{
    for (const auto& name : getNames())
        load(name);
}

/* ************************************************************************ */

ViewPtr<Api> Manager::load(StringView name)
{
    // Plugin is already loaded
    if (isLoaded(name))
        return getApi(name);

    // Load internal
    auto api = loadInternal(String(name)).getApi();

    // Load dependencies
    for (const auto& plugin : api->requiredPlugins())
    {
        // Recursive load
        load(plugin);
    }

    // Store plugin name for unload order (reversed)
    // It's much easier to store in reverse order than in regular order.
    // No duplicate are stored because isLoaded is checked before.
    m_unloadOrderRev.push_back(String(name));

    Log::info("Using plugin '", name,"'...");

    // Load plugin
    api->onLoad(m_context);

    return api;
}

/* ************************************************************************ */

Map<String, FilePath> Manager::scanDirectory(const FilePath& directory) noexcept
{
    using namespace boost::filesystem;

    Map<String, FilePath> result;

    Log::debug("Scanning `", directory.string(), "` for plugins");

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
        const auto prefixLength = Library::FILE_PREFIX.length();
        const auto suffixLength = Library::FILE_EXTENSION.length();
        const auto suffixStart = filename.length() - suffixLength;

        // Different prefix
        if (filename.substr(0, prefixLength) != Library::FILE_PREFIX)
            continue;

        // Different extension
        if (filename.substr(suffixStart) != Library::FILE_EXTENSION)
            continue;

        result.emplace(filename.substr(prefixLength, suffixStart - prefixLength), std::move(path));
    }

    return result;
}

/* ************************************************************************ */

Map<String, FilePath> Manager::scanDirectories() const noexcept
{
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

Library& Manager::loadInternal(String name)
{
    // Try to find library in cache
    // FIXME: In C++14 there is overloaded version of find
    auto it = m_loaded.find(name);

    // Found
    if (it != m_loaded.end())
        return std::get<1>(*it);

    // Try to find in builtin libraries
    // FIXME: In C++14 there is overloaded version of find
    auto itBuiltin = s_builtin.find(name);

    // Required library is builtin
    if (itBuiltin != s_builtin.end())
    {
        Log::debug("Loading builtin plugin `", name, "`...");

        // Insert into cache
        auto ptr = m_loaded.emplace(std::piecewise_construct,
            std::forward_as_tuple(name),
            std::forward_as_tuple(name, UniquePtr<Api>(itBuiltin->second()))
        );

        return std::get<1>(*std::get<0>(ptr));
    }
    else
    {
        Log::debug("Loading external plugin `", name, "`...");

        // Load extern
        // FIXME: In C++14 there is overloaded version of find
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

void Manager::unloadPlugins()
{
    // List is in reverse order
    for (auto i = m_unloadOrderRev.rbegin(); i != m_unloadOrderRev.rend(); ++i)
    {
        auto it = m_loaded.find(*i);
        Assert(it != m_loaded.end());
        it->second.getApi()->onUnload(m_context);
    }
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

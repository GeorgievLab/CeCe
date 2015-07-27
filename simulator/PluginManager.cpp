/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* ************************************************************************ */

// Declaration
#include "PluginManager.hpp"

// C++
#include <cstdint>
#include <cstdlib>
#include <regex>

// Boost
#include <boost/filesystem.hpp>

// Simulator
#include "core/Log.hpp"
#include "core/Exception.hpp"
#include "core/FilePath.hpp"
#include "core/Range.hpp"
#include "simulator/PluginApi.hpp"

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

DynamicArray<String> PluginManager::s_directories;

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

Map<String, FilePath> PluginManager::s_extern;

/* ************************************************************************ */

Map<String, Plugin> PluginManager::s_loaded;

/* ************************************************************************ */

void PluginManager::addDirectory(String path)
{
    Log::debug("New plugins directory: `", path, "`");

    s_directories.push_back(std::move(path));
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
    names.reserve(s_extern.size());

    for (const auto& p : s_extern)
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
    return s_loaded.find(name) != s_loaded.end();
}

/* ************************************************************************ */

ViewPtr<PluginApi> PluginManager::getApi(const String& name) noexcept
{
    // Try to find library
    auto it = s_loaded.find(name);

    if (it == s_loaded.end())
        return nullptr;

    return std::get<1>(*it).getApi();
}

/* ************************************************************************ */

ViewPtr<PluginApi> PluginManager::load(const String& name)
{
    // Try to find library in cache
    auto it = s_loaded.find(name);

    // Found
    if (it != s_loaded.end())
        return std::get<1>(*it).getApi();

    // Try to find in builtin libraries
    auto itBuiltin = s_builtin.find(name);

    // Required library is builtin
    if (itBuiltin != s_builtin.end())
    {
        Log::debug("Loading builtin plugin `", name, "`...");

        // Insert into cache
        auto ptr = s_loaded.emplace(std::piecewise_construct,
            std::forward_as_tuple(name),
            std::forward_as_tuple(name, UniquePtr<PluginApi>(itBuiltin->second()))
        );

        return std::get<1>(*std::get<0>(ptr)).getApi();
    }
    else
    {
        Log::debug("Loading external plugin `", name, "`...");

        // Load extern
        auto it = s_extern.find(name);

        if (it == s_extern.end())
            throw InvalidArgumentException("Unable to find plugin '" + name + "'");

        // Insert into cache
        auto ptr = s_loaded.emplace(std::piecewise_construct,
            std::forward_as_tuple(name),
            std::forward_as_tuple(name, it->second)
        );

        return std::get<1>(*std::get<0>(ptr)).getApi();
    }
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
        const auto filename = path.filename();

        // Match file name
        if (!std::regex_match(filename.string(), matches, regex))
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

}

/* ************************************************************************ */

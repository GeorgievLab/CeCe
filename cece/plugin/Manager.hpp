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

#pragma once

/* ************************************************************************ */

// CeCe
#include "cece/export.hpp"
#include "cece/core/String.hpp"
#include "cece/core/StringView.hpp"
#include "cece/core/DynamicArray.hpp"
#include "cece/core/Map.hpp"
#include "cece/core/ViewPtr.hpp"
#include "cece/core/FilePath.hpp"
#include "cece/plugin/Library.hpp"
#include "cece/plugin/Context.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {

/* ************************************************************************ */

class Api;

/* ************************************************************************ */

/**
 * @brief Manager of simulator plugins.
 *
 * This class is responsible for loading and unloading plugins. It all begins
 * with registering plugins directory:
 * @code
    plugin::Manager m;
    m.addDirectory("plugins");
   @endcode
 * which scan given plugins directory and store paths of those plugins into internal
 * variable. Then when plugin is required, it's loaded from given path.
 * @code
    plugin::Manager m;
    auto pluginApi = m.load("myplugin");
   @endcode
 * Loading plugin also loads dependent plugins.
 */
class CECE_EXPORT Manager final
{

// Public Ctors & Dtors
public:


    /**
     * @brief Destructor.
     */
    ~Manager()
    {
        unloadPlugins();
    }


// Public Accessors
public:


    /**
     * @brief Returns directories where plugins are stored.
     *
     * @return List of directories.
     *
     * @see addDirectory
     */
    const DynamicArray<FilePath>& getDirectories() const noexcept
    {
        return m_directories;
    }


    /**
     * @brief Check if builtin plugin with given name exists.
     *
     * @param name Plugin name.
     *
     * @return If builtin plugin exists.
     */
    bool isAvailableBuiltin(StringView name) const noexcept
    {
        // FIXME: In C++14 there is overloaded version of find
        return s_builtin.find(String(name)) != s_builtin.end();
    }


    /**
     * @brief Check if extern plugin with given name exists.
     *
     * @param name Plugin name.
     *
     * @return If extern plugin exists.
     */
    bool isAvailableExtern(StringView name) const noexcept
    {
        // FIXME: In C++14 there is overloaded version of find
        return m_extern.find(String(name)) != m_extern.end();
    }


    /**
     * @brief Check if plugin with given name exists.
     *
     * @param name Plugin name.
     *
     * @return If plugin exists.
     */
    bool isAvailable(StringView name) const noexcept
    {
        return isAvailableBuiltin(name) || isAvailableExtern(name);
    }


    /**
     * @brief Return a list of builtin plugins.
     *
     * @return An array of builtin plugins names.
     */
    DynamicArray<String> getNamesBuiltin() const noexcept;


    /**
     * @brief Return a list of extern plugins.
     *
     * @return An array of extern plugin names.
     */
    DynamicArray<String> getNamesExtern() const noexcept;


    /**
     * @brief Return a list of plugins.
     *
     * @return An array of plugin names.
     */
    DynamicArray<String> getNames() const noexcept;


    /**
     * @brief Check if plugin is loaded.
     *
     * @param name Plugin name.
     *
     * @return
     */
    bool isLoaded(StringView name) const noexcept
    {
        // FIXME: In C++14 there is overloaded version of find
        return m_loaded.find(String(name)) != m_loaded.end();
    }


    /**
     * @brief Returns plugin API of loaded plugin.
     *
     * @param name Plugin name.
     *
     * @return Pointer to API or nullptr, if plugin is not loaded.
     */
    ViewPtr<Api> getApi(StringView name) const noexcept
    {
        // Try to find library
        // FIXME: In C++14 there is overloaded version of find
        auto it = m_loaded.find(String(name));

        if (it == m_loaded.end())
            return nullptr;

        return std::get<1>(*it).getApi();
    }


    /**
     * @brief Returns plugin context.
     *
     * @return
     */
    Context& getContext() noexcept
    {
        return m_context;
    }


    /**
     * @brief Returns plugin context.
     *
     * @return
     */
    const Context& getContext() const noexcept
    {
        return m_context;
    }


// Public Mutators
public:


    /**
     * @brief Store additional directories.
     *
     * @param directories List of directories.
     *
     * @see addDirectory
     */
    void addDirectories(DynamicArray<FilePath> directories)
    {
        for (auto&& directory : directories)
            addDirectory(std::move(directory));
    }


    /**
     * @brief Add directory where the plugins are stored.
     *
     * @param path
     */
    void addDirectory(FilePath path);


// Public Operations
public:


    /**
     * @brief Load all plugins.
     */
    void loadAll();


    /**
     * @brief Load plugin.
     *
     * @param name Plugin name.
     *
     * @return Pointer to API or nullptr, if plugin is not loaded.
     */
    ViewPtr<Api> load(StringView name);


    /**
     * @brief Scan directory for plugins.
     *
     * @param directory Plugins directory to scan.
     *
     * @return Paths to plugins.
     */
    static Map<String, FilePath> scanDirectory(const FilePath& directory) noexcept;


    /**
     * @brief Scan plugins directories for plugins.
     *
     * @return
     *
     * @see scanDirectory
     */
    Map<String, FilePath> scanDirectories() const noexcept;


    /**
     * @brief Rescan directories for extern plugins.
     *
     * @see scanDirectory
     */
    void rescanDirectories() noexcept
    {
        m_extern = scanDirectories();
    }


    /**
     * @brief Release all loaded plugins.
     *
     * After this point the plugins features are not available.
     */
    void releasePlugins() noexcept
    {
        // Unload plugins
        unloadPlugins();

        // Release plugins
        m_loaded.clear();
    }


    /**
     * @brief Returns global instance of plugin manager.
     *
     * @return
     */
    static Manager& s()
    {
        static Manager instance;
        return instance;
    }


// Private Operations
private:


    /**
     * @brief Load plugin (internal).
     *
     * @param name Plugin name.
     *
     * @return Reference to loaded plugin.
     */
    Library& loadInternal(String name);


    /**
     * @brief Load all plugins.
     *
     * It calls `onUnload` for all loaded plugins.
     */
    void unloadPlugins();


// Private Data Members
private:

    /// Plugin context.
    Context m_context;

    /// Plugins directory paths.
    DynamicArray<FilePath> m_directories;

    /// Extern plugins paths.
    Map<String, FilePath> m_extern;

    /// Loaded plugins.
    Map<String, Library> m_loaded;

    /// Order in which plugins should be unloaded (in reverse order).
    DynamicArray<String> m_unloadOrderRev;

    /// Builtin plugins create API functions.
    static const Map<String, Library::CreateFn> s_builtin;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */

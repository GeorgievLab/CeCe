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
#include "cece/core/String.hpp"
#include "cece/core/DynamicArray.hpp"
#include "cece/core/Map.hpp"
#include "cece/core/ViewPtr.hpp"
#include "cece/simulator/Plugin.hpp"
#include "cece/simulator/PluginContext.hpp"

/* ************************************************************************ */

namespace cece {
namespace simulator {

/* ************************************************************************ */

class PluginApi;
class Simulation;

/* ************************************************************************ */

/**
 * @brief Manager of simulator plugins.
 */
class PluginManager final
{

// Public Accessors
public:


    /**
     * @brief Returns directories where plugins are stored.
     *
     * @return
     */
    const DynamicArray<String>& getDirectories() noexcept
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
    bool isAvailableBuiltin(const String& name) noexcept
    {
        return s_builtin.find(name) != s_builtin.end();
    }


    /**
     * @brief Check if extern plugin with given name exists.
     *
     * @param name Plugin name.
     *
     * @return If extern plugin exists.
     */
    bool isAvailableExtern(const String& name) noexcept
    {
        return m_extern.find(name) != m_extern.end();
    }


    /**
     * @brief Check if plugin with given name exists.
     *
     * @param name Plugin name.
     *
     * @return If plugin exists.
     */
    bool isAvailable(const String& name) noexcept
    {
        return isAvailableBuiltin(name) || isAvailableExtern(name);
    }


    /**
     * @brief Return a list of builtin plugins.
     *
     * @return An array of builtin plugins names.
     */
    DynamicArray<String> getNamesBuiltin() noexcept;


    /**
     * @brief Return a list of extern plugins.
     *
     * @return An array of extern plugin names.
     */
    DynamicArray<String> getNamesExtern() noexcept;


    /**
     * @brief Return a list of plugins.
     *
     * @return An array of plugin names.
     */
    DynamicArray<String> getNames() noexcept;


    /**
     * @brief Check if plugin is loaded.
     *
     * @param name Plugin name.
     *
     * @return
     */
    bool isLoaded(const String& name) noexcept;


    /**
     * @brief Returns plugin API of loaded plugin.
     *
     * @param name Plugin name.
     *
     * @return Pointer to API or nullptr, if plugin is not loaded.
     */
    ViewPtr<PluginApi> getApi(const String& name) noexcept;


    /**
     * @brief Load plugin.
     *
     * @param name Plugin name.
     *
     * @return Pointer to API or nullptr, if plugin is not loaded.
     */
    ViewPtr<PluginApi> load(const String& name);


    /**
     * @brief Returns plugin context.
     *
     * @return
     */
    PluginContext& getContext() noexcept
    {
        return m_context;
    }


// Public Mutators
public:


    /**
     * @brief Add directory where the plugins are stored.
     *
     * @param path
     */
    void addDirectory(String path);


// Public Operations
public:


    /**
     * @brief Initialize manager.
     */
    void init();


    /**
     * @brief Create simulation from file.
     *
     * @param filepath Path to file.
     *
     * @return
     */
    UniquePtr<Simulation> createSimulation(const FilePath& filepath);


    /**
     * @brief Scan directory for plugins.
     *
     * @param directory Plugins directory.
     *
     * @return
     */
    Map<String, FilePath> scanDirectory(const FilePath& directory) noexcept;


    /**
     * @brief Scan plugins directories for plugins.
     *
     * @return
     */
    Map<String, FilePath> scanDirectories() noexcept;


    /**
     * @brief Rescan directories for extern plugins.
     */
    void rescanDirectories() noexcept
    {
        m_extern = scanDirectories();
    }


    /**
     * @brief Release all loaded plugins.
     */
    void releasePlugins() noexcept
    {
        // Release plugins
        m_loaded.clear();
    }


    /**
     * @brief Returns global instance of plugin manager.
     *
     * @return
     */
    static PluginManager& s()
    {
        static PluginManager instance;
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
    Plugin& loadInternal(const String& name);


// Private Data Members
private:

    /// Plugin context.
    PluginContext m_context;

    /// Plugins directory paths.
    DynamicArray<String> m_directories;

    /// Extern plugins paths.
    Map<String, FilePath> m_extern;

    /// Loaded plugins.
    Map<String, Plugin> m_loaded;

    /// Builtin plugins create API functions.
    static const Map<String, Plugin::CreateFn> s_builtin;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */

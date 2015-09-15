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

// Simulator
#include "core/String.hpp"
#include "core/DynamicArray.hpp"
#include "core/Map.hpp"
#include "core/ViewPtr.hpp"
#include "simulator/Plugin.hpp"

/* ************************************************************************ */

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

// Public Structures
public:


    /**
     * @brief Helper class for managing plugins at local scope.
     */
    struct Local
    {

        /**
         * @brief Constructor.
         */
        Local() noexcept
        {
            PluginManager::rescanDirectories();
        }


        /**
         * @brief Copy constructor.
         */
        Local(const Local&) = delete;


        /**
         * @brief Move constructor.
         */
        Local(Local&&) = default;


        /**
         * @brief Destructor.
         */
        ~Local()
        {
            PluginManager::releasePlugins();
        }


        /**
         * @brief Copy assignment operator.
         */
        Local& operator=(const Local&) = delete;


        /**
         * @brief Move assignment operator.
         */
        Local& operator=(Local&&) = default;

    };


// Public Accessors
public:


    /**
     * @brief Create local scope helper.
     *
     * @return
     */
    static Local local() noexcept
    {
        return {};
    }


    /**
     * @brief Returns directories where plugins are stored.
     *
     * @return
     */
    static const DynamicArray<String>& getDirectories() noexcept
    {
        return s_directories;
    }


    /**
     * @brief Check if builtin plugin with given name exists.
     *
     * @param name Plugin name.
     *
     * @return If builtin plugin exists.
     */
    static bool isAvailableBuiltin(const String& name) noexcept
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
    static bool isAvailableExtern(const String& name) noexcept
    {
        return s_extern.find(name) != s_extern.end();
    }


    /**
     * @brief Check if plugin with given name exists.
     *
     * @param name Plugin name.
     *
     * @return If plugin exists.
     */
    static bool isAvailable(const String& name) noexcept
    {
        return isAvailableBuiltin(name) || isAvailableExtern(name);
    }


    /**
     * @brief Return a list of builtin plugins.
     *
     * @return An array of builtin plugins names.
     */
    static DynamicArray<String> getNamesBuiltin() noexcept;


    /**
     * @brief Return a list of extern plugins.
     *
     * @return An array of extern plugin names.
     */
    static DynamicArray<String> getNamesExtern() noexcept;


    /**
     * @brief Return a list of plugins.
     *
     * @return An array of plugin names.
     */
    static DynamicArray<String> getNames() noexcept;


    /**
     * @brief Check if plugin is loaded.
     *
     * @param name Plugin name.
     *
     * @return
     */
    static bool isLoaded(const String& name) noexcept;


    /**
     * @brief Returns plugin API of loaded plugin.
     *
     * @param name Plugin name.
     *
     * @return Pointer to API or nullptr, if plugin is not loaded.
     */
    static ViewPtr<PluginApi> getApi(const String& name) noexcept;


    /**
     * @brief Load plugin.
     *
     * @param name Plugin name.
     *
     * @return Pointer to API or nullptr, if plugin is not loaded.
     */
    static ViewPtr<PluginApi> load(const String& name);


// Public Mutators
public:


    /**
     * @brief Add directory where the plugins are stored.
     *
     * @param path
     */
    static void addDirectory(String path);


// Public Operations
public:


    /**
     * @brief Scan directory for plugins.
     *
     * @param directory Plugins directory.
     *
     * @return
     */
    static Map<String, FilePath> scanDirectory(const FilePath& directory) noexcept;


    /**
     * @brief Scan plugins directories for plugins.
     *
     * @return
     */
    static Map<String, FilePath> scanDirectories() noexcept;


    /**
     * @brief Rescan directories for extern plugins.
     */
    static void rescanDirectories() noexcept
    {
        s_extern = scanDirectories();
    }


    /**
     * @brief Release all loaded plugins.
     */
    static void releasePlugins() noexcept
    {
        // Release plugins
        s_loaded.clear();
    }


// Private Data Members
private:

    /// Plugins directory paths.
    static DynamicArray<String> s_directories;

    /// Builtin plugins create API functions.
    static const Map<String, Plugin::CreateFn> s_builtin;

    /// Extern plugins paths.
    static Map<String, FilePath> s_extern;

    /// Loaded plugins.
    static Map<String, Plugin> s_loaded;

};

/* ************************************************************************ */

}

/* ************************************************************************ */

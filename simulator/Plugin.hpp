/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */

#pragma once

/* ************************************************************************ */

// Simulator
#include "core/compatibility.hpp"
#include "core/String.hpp"
#include "core/DynamicArray.hpp"
#include "core/Map.hpp"
#include "core/ViewPtr.hpp"
#include "core/UniquePtr.hpp"
#include "simulator/Program.hpp"
#include "simulator/Module.hpp"
#include "simulator/Object.hpp"

/* ************************************************************************ */

/**
 * @brief Plugin API version.
 */
#define PLUGIN_API_VERSION 1

/* ************************************************************************ */

/**
 * @brief Define function name for extern plugins.
 *
 * @param prefix Function prefix.
 * @param name   Plugin name.
 */
#define PLUGIN_PROTOTYPE_NAME_EXTERN(prefix, name) prefix

/* ************************************************************************ */

/**
 * @brief Define function name for builtin plugins.
 *
 * @param prefix Function prefix.
 * @param name   Plugin name.
 */
#define PLUGIN_PROTOTYPE_NAME_BUILTIN(prefix, name) prefix ## _ ## name

/* ************************************************************************ */

/**
 * @brief Function name.
 *
 * @param name Plugin name.
 */
#if PLUGIN_BUILTIN
#define PLUGIN_PROTOTYPE_NAME(prefix, name) PLUGIN_PROTOTYPE_NAME_BUILTIN(prefix, name)
#else
#define PLUGIN_PROTOTYPE_NAME(prefix, name) PLUGIN_PROTOTYPE_NAME_EXTERN(prefix, name)
#endif

/* ************************************************************************ */

/**
 * @brief Prototype of function for creating plugin API object.
 *
 * @param name Plugin name.
 */
#define PLUGIN_CREATE_PROTOTYPE(name) \
    extern "C" simulator::PluginApi* PLUGIN_PROTOTYPE_NAME(create, name)()

/* ************************************************************************ */

/**
 * @brief Declare function for creating plugin API object.
 *
 * @param name Plugin name.
 */
#define DECLARE_PLUGIN_CREATE(name) PLUGIN_CREATE_PROTOTYPE(name)

/* ************************************************************************ */

/**
 * @brief Define function for creating plugin API object.
 *
 * @param name      Plugin name.
 * @param className Plugin API class name.
 */
#define DEFINE_PLUGIN_CREATE(name, className) \
    PLUGIN_CREATE_PROTOTYPE(name) \
    { \
        return new className{}; \
    }

/* ************************************************************************ */

/**
 * @brief Prototype of function for returning plugin API version.
 *
 * @param name Plugin name.
 */
#define PLUGIN_API_VERSION_PROTOTYPE(name) \
    extern "C" int PLUGIN_PROTOTYPE_NAME(api_version, name)()

/* ************************************************************************ */

/**
 * @brief Declare function for returning plugin API version.
 *
 * @param name Plugin name.
 */
#define DECLARE_PLUGIN_API_VERSION(name) PLUGIN_API_VERSION_PROTOTYPE(name)

/* ************************************************************************ */

/**
 * @brief Define function for returning plugin API version.
 *
 * @param name Plugin name.
 */
#define DEFINE_PLUGIN_API_VERSION(name) \
    PLUGIN_API_VERSION_PROTOTYPE(name) \
    { \
        return PLUGIN_API_VERSION; \
    }

/* ************************************************************************ */

/**
 * @brief Declare plugin functions.
 *
 * @param name Plugin name.
 */
#define DECLARE_PLUGIN(name) \
    DECLARE_PLUGIN_CREATE(name); \
    DECLARE_PLUGIN_API_VERSION(name)

/* ************************************************************************ */

/**
 * @brief Define plugin functions.
 *
 * @param name      Plugin name.
 * @param className Plugin API class name.
 */
#define DEFINE_PLUGIN(name, className) \
    DEFINE_PLUGIN_CREATE(name, className) \
    DEFINE_PLUGIN_API_VERSION(name)

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

class PluginApi;
class Simulation;

/* ************************************************************************ */

/**
 * @brief Helper class for wrapping plugin.
 */
class DLL_EXPORT Plugin final
{

// Public Types
public:


    /// Create API function pointer type.
    using CreateFn = PluginApi* (*)();

    /// API version function pointer type.
    using ApiVersionFn = int (*)();


// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * Constructor search plugin in builtin plugins and if there is none
     * with given name it will look for shared library file in the plugins
     * directory. In case the shared library doesn't have exported
     * required functions an exception is thrown.
     *
     * @param name Plugin name.
     *
     * @throw In case valid plugin cannot be created.
     */
    explicit Plugin(String name);


    /**
     * @brief Destructor.
     */
    ~Plugin();


// Public Accessors
public:


    /**
     * @brief Returns plugin name.
     *
     * @return
     */
    const String& getName() const NOEXCEPT
    {
        return m_name;
    }


    /**
     * @brief Returns API object.
     *
     * @return
     */
    ViewPtr<PluginApi> getApi() const NOEXCEPT
    {
        return m_api;
    }


    /**
     * @brief Returns library paths.
     *
     * @return
     */
    static const DynamicArray<String>& getDirectories() NOEXCEPT
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
    static bool isAvailableBuiltin(const String& name) NOEXCEPT
    {
        return s_builtinPlugins.find(name) != s_builtinPlugins.end();
    }


    /**
     * @brief Check if extern plugin with given name exists.
     *
     * @param name Plugin name.
     *
     * @return If extern plugin exists.
     */
    static bool isAvailableExtern(const String& name) NOEXCEPT
    {
        return s_externPlugins.find(name) != s_externPlugins.end();
    }


    /**
     * @brief Check if plugin with given name exists.
     *
     * @param name Plugin name.
     *
     * @return If plugin exists.
     */
    static bool isAvailable(const String& name) NOEXCEPT
    {
        return isAvailableBuiltin(name) || isAvailableExtern(name);
    }


    /**
     * @brief Return a list of builtin plugins.
     *
     * @return An array of builtin plugins names.
     */
    static DynamicArray<String> getNamesBuiltin() NOEXCEPT;


    /**
     * @brief Return a list of extern plugins.
     *
     * @return An array of extern plugin names.
     */
    static DynamicArray<String> getNamesExtern() NOEXCEPT;


    /**
     * @brief Return a list of plugins.
     *
     * @return An array of plugin names.
     */
    static DynamicArray<String> getNames() NOEXCEPT;


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
    static Map<String, FilePath> scanDirectory(const FilePath& directory) NOEXCEPT;


    /**
     * @brief Scan plugins directories for plugins.
     *
     * @return
     */
    static Map<String, FilePath> scanDirectories() NOEXCEPT;


    /**
     * @brief Rescan directories for extern plugins.
     */
    static void rescanDirectories() NOEXCEPT
    {
        s_externPlugins = scanDirectories();
    }


// Private Data Members
private:

    /// Plugin name.
    String m_name;

    /// Implementation
    struct Impl;
    UniquePtr<Impl> m_impl;

    /// Plugin API.
    UniquePtr<PluginApi> m_api;

    /// Plugins directory paths.
    static DynamicArray<String> s_directories;

    /// Builtin plugins create API functions.
    static const Map<String, CreateFn> s_builtinPlugins;

    /// Extern plugins paths.
    static Map<String, FilePath> s_externPlugins;

};

/* ************************************************************************ */

}

/* ************************************************************************ */

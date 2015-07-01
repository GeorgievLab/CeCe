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
    static const DynamicArray<String>& getLibraryPaths() NOEXCEPT
    {
        return s_libraryPaths;
    }


    /**
     * @brief Check if given library name is builtin.
     *
     * @param name Library name.
     *
     * @return
     */
    static bool isBuiltin(const String& name) NOEXCEPT
    {
        return s_builtinLibraries.find(name) != s_builtinLibraries.end();
    }


    /**
     * @brief Return a list of builtin extension libraries.
     *
     * @return An array of builtin library names.
     */
    static DynamicArray<String> getBuiltInNames() NOEXCEPT;


    /**
     * @brief Return a list of extern extension libraries.
     *
     * @return An array of extern library names.
     */
    static DynamicArray<String> getExternNames() NOEXCEPT;


// Public Mutators
public:


    /**
     * @brief Add path where are libraries stored.
     *
     * @param path
     */
    static void addLibraryPath(String path);


// Private Data Members
private:

    /// Plugin name.
    String m_name;

    /// Implementation
    struct Impl;
    UniquePtr<Impl> m_impl;

    /// Plugin API.
    UniquePtr<PluginApi> m_api;

    /// Library paths.
    static DynamicArray<String> s_libraryPaths;

    /// Builin plugins create API functions.
    static const Map<String, CreateFn> s_builtinLibraries;

};

/* ************************************************************************ */

}

/* ************************************************************************ */


#pragma once

/* ************************************************************************ */

// Simulator
#include "core/compatibility.hpp"
#include "core/String.hpp"
#include "core/DynamicArray.hpp"
#include "core/Map.hpp"
#include "core/UniquePtr.hpp"
#include "simulator/Program.hpp"
#include "simulator/Module.hpp"
#include "simulator/Object.hpp"

/* ************************************************************************ */

/**
 * @brief Library API version.
 */
#define LIBRARY_API_VERSION 1

/* ************************************************************************ */

/**
 * @brief Define function name for dynamic libraries.
 *
 * @param prefix Function prefix.
 * @param name   Library name.
 */
#define LIBRARY_PROTOTYPE_NAME_DYNAMIC(prefix, name) prefix

/* ************************************************************************ */

/**
 * @brief Define function name for build-in libraries.
 *
 * @param prefix Function prefix.
 * @param name   Library name.
 */
#define LIBRARY_PROTOTYPE_NAME_BUILDIN(prefix, name) prefix ## _ ## name

/* ************************************************************************ */

/**
 * @brief Function name.
 *
 * @param name Library name.
 */
#if LIBRARY_BUILDIN
#define LIBRARY_PROTOTYPE_NAME(prefix, name) LIBRARY_PROTOTYPE_NAME_BUILDIN(prefix, name)
#else
#define LIBRARY_PROTOTYPE_NAME(prefix, name) LIBRARY_PROTOTYPE_NAME_DYNAMIC(prefix, name)
#endif

/* ************************************************************************ */

/**
 * @brief Prototype of function for creating library object.
 *
 * @param name Library name.
 */
#define LIBRARY_CREATE_PROTOTYPE(name) \
    extern "C" simulator::LibraryApi* LIBRARY_PROTOTYPE_NAME(create, name)()

/* ************************************************************************ */

/**
 * @brief Declare function for creating library object.
 *
 * @param name Library name.
 */
#define DECLARE_LIBRARY_CREATE(name) LIBRARY_CREATE_PROTOTYPE(name)

/* ************************************************************************ */

/**
 * @brief Define function for creating library object.
 *
 * @param name Library name.
 */
#define DEFINE_LIBRARY_CREATE(name) LIBRARY_CREATE_PROTOTYPE(name)

/* ************************************************************************ */

/**
 * @brief Define function for creating library object.
 *
 * @param name      Library name.
 * @param className
 */
#define DEFINE_LIBRARY_CREATE_IMPL(name, className) \
    LIBRARY_CREATE_PROTOTYPE(name) \
    { \
        return new className{}; \
    }

/* ************************************************************************ */

/**
 * @brief Prototype of function for returning library API version.
 *
 * @param name Library name.
 */
#define LIBRARY_API_VERSION_PROTOTYPE(name) \
    extern "C" int LIBRARY_PROTOTYPE_NAME(api_version, name)()

/* ************************************************************************ */

/**
 * @brief Declare function for returning library API version.
 *
 * @param name Library name.
 */
#define DECLARE_LIBRARY_API_VERSION(name) LIBRARY_API_VERSION_PROTOTYPE(name)

/* ************************************************************************ */

/**
 * @brief Define function for returning library API version.
 *
 * @param name Library name.
 */
#define DEFINE_LIBRARY_API_VERSION(name) LIBRARY_API_VERSION_PROTOTYPE(name)

/* ************************************************************************ */

/**
 * @brief Define function for returning library API version.
 *
 * @param name Library name.
 */
#define DEFINE_LIBRARY_API_VERSION_IMPL(name) \
    LIBRARY_API_VERSION_PROTOTYPE(name) \
    { \
        return LIBRARY_API_VERSION; \
    }

/* ************************************************************************ */

/**
 * @brief Declare library functions.
 *
 * @param name Library name.
 */
#define DECLARE_LIBRARY(name) \
    DECLARE_LIBRARY_CREATE(name); \
    DECLARE_LIBRARY_API_VERSION(name)

/* ************************************************************************ */

/**
 * @brief Define library functions.
 *
 * @param name Library name.
 */
#define DEFINE_LIBRARY(name, className) \
    DEFINE_LIBRARY_CREATE_IMPL(name, className) \
    DEFINE_LIBRARY_API_VERSION_IMPL(name)

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

class LibraryApi;
class Simulation;

/* ************************************************************************ */

/**
 * @brief Class for loading module libraries into simulation.
 */
class DLL_EXPORT Library final
{

// Public Types
public:


    /// Create API function pointer type.
    using CreateFn = LibraryApi* (*)();

    /// API version function pointer type.
    using ApiVersionFn = int (*)();


// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param name Library name.
     */
    explicit Library(const String& name);


    /**
     * @brief Destructor.
     */
    ~Library();


// Public Accessors
public:


    /**
     * @brief Returns if library is loaded.
     *
     * @return
     */
    bool isLoaded() const NOEXCEPT;


    /**
     * @brief Returns loading library error.
     *
     * @return
     */
    String getError() const NOEXCEPT;


    /**
     * @brief Returns API object.
     *
     * @return
     */
    LibraryApi* getApi() const NOEXCEPT
    {
        return m_api.get();
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
     * @brief Check if given library name is build-in.
     *
     * @param name Library name.
     *
     * @return
     */
    static bool isBuildin(const String& name) NOEXCEPT
    {
        return s_buildinLibraries.find(name) != s_buildinLibraries.end();
    }


    /**
     * @brief Return a list of build-in extension libraries.
     *
     * @return An array of build-in library names.
     */
    static DynamicArray<String> getBuildInNames() NOEXCEPT;


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

    /// Implementation
    struct Impl;
    UniquePtr<Impl> m_impl;

    /// Object for library API.
    UniquePtr<LibraryApi> m_api;

    /// Library paths.
    static DynamicArray<String> s_libraryPaths;

    /// Build-in static library builders.
    static const Map<String, CreateFn> s_buildinLibraries;

};

/* ************************************************************************ */

}

/* ************************************************************************ */

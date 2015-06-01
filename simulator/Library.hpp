
#pragma once

/* ************************************************************************ */

// C++
#include <string>
#include <memory>
#include <vector>

// Simulator
#include "core/compatibility.hpp"
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
     * @param name
     */
    explicit Library(const std::string& name);


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
    std::string getError() const NOEXCEPT;


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
    static const std::vector<std::string>& getLibraryPaths() NOEXCEPT
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
    static bool isBuildin(const std::string& name) NOEXCEPT
    {
        return s_buildinLibraries.find(name) != s_buildinLibraries.end();
    }


    /**
     * @brief Return a list of build-in libraries
     *
     * @param path
     */
    static std::vector<std::string> getBuildInNames() NOEXCEPT;


// Public Mutators
public:


    /**
     * @brief Add path where are libraries stored.
     *
     * @param path
     */
    static void addLibraryPath(std::string path);


// Private Data Members
private:

#if !STATIC_BUILD
    /// Implementation
    struct Impl;
    std::unique_ptr<Impl> m_impl;
#endif

    /// Object for library API.
    std::unique_ptr<LibraryApi> m_api;

    /// Library paths.
    static std::vector<std::string> s_libraryPaths;

    /// Build-in static library builders.
    static const std::map<std::string, CreateFn> s_buildinLibraries;

};

/* ************************************************************************ */

}

/* ************************************************************************ */


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
 * @brief Function name for dynamic.
 *
 * @param name Library name.
 */
#define LIBRARY_CREATE_PROTOTYPE_NAME_DYNAMIC(name) create

/* ************************************************************************ */

/**
 * @brief Function name for dynamic.
 *
 * @param name Library name.
 */
#define LIBRARY_CREATE_PROTOTYPE_NAME_BUILDIN(name) create_ ## name

/* ************************************************************************ */

/**
 * @brief Function name.
 *
 * @param name Library name.
 */
#if LIBRARY_BUILDIN
#define LIBRARY_CREATE_PROTOTYPE_NAME(name) LIBRARY_CREATE_PROTOTYPE_NAME_BUILDIN(name)
#else
#define LIBRARY_CREATE_PROTOTYPE_NAME(name) LIBRARY_CREATE_PROTOTYPE_NAME_DYNAMIC(name)
#endif

/* ************************************************************************ */

/**
 * @brief Prototype of function for creating library object.
 *
 * @param name Library name.
 */
#define LIBRARY_CREATE_PROTOTYPE(name) \
    extern "C" simulator::LibraryApi* LIBRARY_CREATE_PROTOTYPE_NAME(name)()

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
 * @brief Declare library functions.
 *
 * @param name Library name.
 */
#define DECLARE_LIBRARY(name) \
    DECLARE_LIBRARY_CREATE(name)

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

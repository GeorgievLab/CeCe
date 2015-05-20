
#pragma once

/* ************************************************************************ */

// C++
#include <string>
#include <memory>
#include <vector>

// Simulator
#include "simulator/Program.hpp"
#include "simulator/Module.hpp"
#include "simulator/Object.hpp"

/* ************************************************************************ */

/**
 * @brief Prototype of function for creating library object.
 */
#define LIBRARY_CREATE_PROTOTYPE \
    extern "C" simulator::LibraryApi* create()

/* ************************************************************************ */

/**
 * @brief Declare function for creating library object.
 */
#define DECLARE_LIBRARY_CREATE LIBRARY_CREATE_PROTOTYPE

/* ************************************************************************ */

/**
 * @brief Define function for creating library object.
 */
#define DEFINE_LIBRARY_CREATE LIBRARY_CREATE_PROTOTYPE

/* ************************************************************************ */

/**
 * @brief Define function for creating library object.
 */
#define DEFINE_LIBRARY_CREATE_IMPL(name) \
    LIBRARY_CREATE_PROTOTYPE \
    { \
        return new name{}; \
    }

/* ************************************************************************ */

/**
 * @brief Declare library functions.
 */
#define DECLARE_LIBRARY \
    DECLARE_LIBRARY_CREATE

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

class LibraryApi;
class Simulation;

/* ************************************************************************ */

/**
 * @brief Class for loading module libraries into simulation.
 */
class Library final
{

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
    bool isLoaded() const noexcept;


    /**
     * @brief Returns loading library error.
     *
     * @return
     */
    std::string getError() const noexcept;


    /**
     * @brief Returns API object.
     *
     * @return
     */
    LibraryApi* getApi() const noexcept
    {
        return m_api.get();
    }


    /**
     * @brief Returns library paths.
     *
     * @return
     */
    static const std::vector<std::string>& getLibraryPaths() noexcept
    {
        return s_libraryPaths;
    }


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

    /// Implementation
    struct Impl;
    std::unique_ptr<Impl> m_impl;

    /// Object for library API.
    std::unique_ptr<LibraryApi> m_api;

    /// Library paths.
    static std::vector<std::string> s_libraryPaths;
};

/* ************************************************************************ */

/**
 * @brief Library API type.
 */
class LibraryApi
{

// Public Ctors & Dtors
public:


    /**
     * @brief Destructor.
     */
    virtual ~LibraryApi()
    {
        // Nothing to do
    }


// Public Operations
public:


    /**
     * @brief Init simulation.
     *
     * @param simulation Simulation.
     */
    virtual void initSimulation(Simulation& simulation) noexcept
    {
        // Nothing to do
    }


    /**
     * @brief Finalize simulation.
     *
     * @param simulation Simulation.
     */
    virtual void finalizeSimulation(Simulation& simulation) noexcept
    {
        // Nothing to do
    }


    /**
     * @brief Create module from current library.
     *
     * @param simulation Simulation for that module is created.
     * @param name       Module name.
     *
     * @return Created module.
     */
    virtual std::unique_ptr<Module> createModule(Simulation& simulation, const std::string& name) noexcept
    {
        return nullptr;
    }


    /**
     * @brief Create object from current library.
     *
     * @param simulation Simulation for that module is created.
     * @param name       Object name.
     * @param dynamic    If object should be dynamic.
     *
     * @return Created object.
     */
    virtual std::unique_ptr<Object> createObject(Simulation& simulation, const std::string& name, bool dynamic = true) noexcept
    {
        return nullptr;
    }


    /**
     * @brief Create program from current library.
     *
     * @param simulation Simulation for that module is created.
     * @param name       Object name.
     * @param dynamic    If object should be dynamic.
     *
     * @return Created object.
     */
    virtual Program createProgram(Simulation& simulation, const std::string& name) noexcept
    {
        return {};
    }

};

/* ************************************************************************ */

}

/* ************************************************************************ */

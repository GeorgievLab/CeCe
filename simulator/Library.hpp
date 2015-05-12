
#pragma once

/* ************************************************************************ */

// C++
#include <string>
#include <memory>
#include <vector>

/* ************************************************************************ */

/**
 * @brief Declare function for setup simulation.
 */
#define DECLARE_LIBRARY_SETUP \
    extern "C" void setup(simulator::Simulation*)

/* ************************************************************************ */

/**
 * @brief Define function for setup simulation.
 */
#define DEFINE_LIBRARY_SETUP(sim) \
    void setup(simulator::Simulation* sim)

/* ************************************************************************ */

/**
 * @brief Declare function for cleanup simulation.
 */
#define DECLARE_LIBRARY_CLEANUP \
    extern "C" void cleanup(simulator::Simulation*)

/* ************************************************************************ */

/**
 * @brief Define function for cleanup simulation.
 */
#define DEFINE_LIBRARY_CLEANUP(sim) \
    void cleanup(simulator::Simulation* sim)

/* ************************************************************************ */

/**
 * @brief Declare function for creating library class.
 */
#define DECLARE_LIBRARY_CREATE \
    extern "C" simulator::Module* create_module(simulator::Simulation*, const char*)

/* ************************************************************************ */

/**
 * @brief Define function for creating library class.
 */
#define DEFINE_LIBRARY_CREATE(sim, name) \
    simulator::Module* create_module(simulator::Simulation* sim, const char* name)

/* ************************************************************************ */

/**
 * @brief Declare library functions.
 */
#define DECLARE_LIBRARY \
    DECLARE_LIBRARY_CREATE

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

class Module;
class Simulation;

/* ************************************************************************ */

/**
 * @brief Setup simulation function pointer.
 */
using SetupSimulation = void (*)(Simulation*);

/* ************************************************************************ */

/**
 * @brief Cleanup simulation function pointer.
 */
using CleanupSimulation = void (*)(Simulation*);

/* ************************************************************************ */

/**
 * @brief Create module function.
 */
using CreateModule = Module* (*)(Simulation*, const char*);

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
     * @param name Library name.
     */
    explicit Library(const std::string& name);


    /**
     * @brief Destructor.
     */
    ~Library();


// Public Accessors
public:


    /**
     * @brief Returns if library is successfully loaded.
     *
     * @return
     */
    bool isLoaded() const noexcept;


    /**
     * @brief Returns error string.
     *
     * @return
     */
    const std::string& getError() const noexcept;


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


// Public Operations
public:


    /**
     * @brief Setup simulation.
     *
     * @param simulation Pointer to simulation.
     */
    void setupSimulation(Simulation* simulation);


    /**
     * @brief Cleaup simulation.
     *
     * @param simulation Pointer to simulation.
     */
    void cleanupSimulation(Simulation* simulation);


    /**
     * @brief Create module from current library.
     *
     * @param simulation Pointer to simulation for that module is created.
     * @param name       Module name.
     *
     * @return Created module.
     */
    std::unique_ptr<Module> createModule(Simulation* simulation, const std::string& name);


// Private Data Members
private:

    /// Implementation
    struct Impl;
    std::unique_ptr<Impl> m_impl;

    /// Function pointer to setup simulation.
    SetupSimulation m_setupSimulation;

    /// Function pointer to cleanup simulation.
    CleanupSimulation m_cleanupSimulation;

    /// Function pointer to create module.
    CreateModule m_createModule;

    /// Library paths.
    static std::vector<std::string> s_libraryPaths;
};

/* ************************************************************************ */

}

/* ************************************************************************ */

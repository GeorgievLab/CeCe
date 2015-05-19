
#pragma once

/* ************************************************************************ */

// C++
#include <string>
#include <memory>
#include <vector>

/* ************************************************************************ */

/**
 * @brief Declare function for init simulation.
 */
#define DECLARE_LIBRARY_INIT \
    extern "C" void init_simulation(simulator::Simulation*)

/* ************************************************************************ */

/**
 * @brief Define function for init simulation.
 */
#define DEFINE_LIBRARY_INIT(sim) \
    void init_simulation(simulator::Simulation* sim)

/* ************************************************************************ */

/**
 * @brief Declare function for finalize simulation.
 */
#define DECLARE_LIBRARY_FINALIZE \
    extern "C" void finalize_simulation(simulator::Simulation*)

/* ************************************************************************ */

/**
 * @brief Define function for finalize simulation.
 */
#define DEFINE_LIBRARY_FINALIZE(sim) \
    void finalize_simulation(simulator::Simulation* sim)

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
 * @brief Init simulation function pointer.
 */
using InitSimulationFn = void (*)(Simulation*);

/* ************************************************************************ */

/**
 * @brief Finalize simulation function pointer.
 */
using FinalizeSimulationFn = void (*)(Simulation*);

/* ************************************************************************ */

/**
 * @brief Create module function.
 */
using CreateModuleFn = Module* (*)(Simulation*, const char*);

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
     * @brief Init simulation.
     *
     * @param simulation Pointer to simulation.
     */
    void initSimulation(Simulation* simulation);


    /**
     * @brief Finalize simulation.
     *
     * @param simulation Pointer to simulation.
     */
    void finalizeSimulation(Simulation* simulation);


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

    /// Function pointer to init simulation.
    InitSimulationFn m_initSimulation;

    /// Function pointer to finalize simulation.
    FinalizeSimulationFn m_finalizeSimulation;

    /// Function pointer to create module.
    CreateModuleFn m_createModule;

    /// Library paths.
    static std::vector<std::string> s_libraryPaths;
};

/* ************************************************************************ */

}

/* ************************************************************************ */

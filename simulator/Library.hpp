
#pragma once

/* ************************************************************************ */

// C++
#include <string>
#include <memory>

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


// Public Operations
public:


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

    /// Function pointer to create module.
    CreateModule m_createModule;
};

/* ************************************************************************ */

}

/* ************************************************************************ */

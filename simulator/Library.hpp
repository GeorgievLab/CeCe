
#pragma once

/* ************************************************************************ */

// C++
#include <string>
#include <memory>
#include <tuple>

/* ************************************************************************ */

/**
 * @brief Declare function for creating library class.
 */
#define DECLARE_LIBRARY_CREATE \
    extern "C" simulator::Module* create_module(const char*)

/* ************************************************************************ */

/**
 * @brief Define function for creating library class.
 */
#define DEFINE_LIBRARY_CREATE(name) \
    simulator::Module* create_module(const char* name)

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

/* ************************************************************************ */

/**
 * @brief Create module function.
 */
using CreateModule = Module* (*)(const char*);

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


// Public Operations
public:


    /**
     * @brief Create module from current library.
     *
     * @param name Module name.
     *
     * @return Created module.
     */
    std::unique_ptr<Module> createModule(const std::string& name);


    /**
     * @brief Load library into memory.
     *
     * @param name
     *
     * @return
     */
    static Library* load(const std::string& name);


    /**
     * @brief Load library and create module.
     *
     * @param library Library name.
     * @param name    Module name.
     *
     * @return Created module.
     */
    static std::unique_ptr<Module> createModule(const std::string& library, const std::string& name);


    /**
     * @brief Load library and create module.
     *
     * @param path
     *
     * @return Created module.
     */
    static std::unique_ptr<Module> createModule(const std::tuple<std::string, std::string>& path)
    {
        return createModule(std::get<0>(path), std::get<1>(path));
    }


    /**
     * @brief Split path into separate values.
     *
     * @param path
     *
     * @return Split path.
     */
    static std::tuple<std::string, std::string> splitPath(const std::string& path);


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


#pragma once

/* ************************************************************************ */

// C++
#include <string>
#include <memory>

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

class Module;

/* ************************************************************************ */

/**
 * @brief Abstract class for loading module libraries into simulation.
 */
class Library
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
    virtual ~Library();


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


// Private Data Members
private:

    /// Implementation
    struct Impl;
    std::unique_ptr<Impl> m_impl;

};

/* ************************************************************************ */

}

/* ************************************************************************ */

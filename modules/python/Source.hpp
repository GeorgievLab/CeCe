
#pragma once

/* ************************************************************************ */

// C++
#include <string>

// Python
#include <Python.h>

// Simulator
#include "core/compatibility.hpp"

// Module
#include "Handle.hpp"

/* ************************************************************************ */

namespace module {
namespace python {

/* ************************************************************************ */

/**
 * @brief Simple wrapper functor for Python code.
 */
class Source
{

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     */
    Source();


// Public Accessors
public:


    /**
     * @brief Returns if source is initialized.
     *
     * @param source
     */
    bool isInitialized() const NOEXCEPT
    {
        return m_initialized;
    }


// Public Accessors
public:


    /**
     * @brief Returns Python object of required name.
     *
     * @param name Function name.
     *
     * @return Pointer to function or nullptr.
     */
    Handle<PyObject> getFunction(const char* name) const NOEXCEPT;


    /**
     * @brief Returns Python object of required name.
     *
     * @param name Function name.
     *
     * @return Pointer to function or nullptr.
     */
    Handle<PyObject> getFunction(const std::string& name) const NOEXCEPT
    {
        return getFunction(name.c_str());
    }


// Public Operations
public:


    /**
     * @brief Initialize from source.
     *
     * @param source Source code.
     */
    void initSource(const std::string& source);


    /**
     * @brief Initialize from file.
     *
     * @param filename Path to source file.
     */
    void initFile(const std::string& filename);


// Private Data Members
private:

    /// If source is initialized.
    bool m_initialized = false;

    /// Main module dictionary.
    Handle<PyObject> m_dictionary;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */

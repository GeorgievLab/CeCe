
#pragma once

/* ************************************************************************ */

// Simulator
#include "core/compatibility.hpp"
#include "core/String.hpp"
#include "core/FilePath.hpp"

// Module
#include "Python.hpp"
#include "Handle.hpp"
#include "View.hpp"

/* ************************************************************************ */

namespace plugin {
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
     * @return
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
    Handle<PyObject> getFunction(const String& name) const NOEXCEPT
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
    void initSource(const String& source);


    /**
     * @brief Initialize from file.
     *
     * @param filename Path to source file.
     */
    void initFile(const FilePath& filename);


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

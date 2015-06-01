
#pragma once

/* ************************************************************************ */

// Simulator
#include "core/Units.hpp"
#include "simulator/Object.hpp"

// Python
#include <Python.h>

// Module
#include "Source.hpp"

/* ************************************************************************ */

namespace module {
namespace python {

/* ************************************************************************ */

/**
 * @brief Simple wrapper functor for Python code.
 */
class Program
{

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     */
    Program();


    /**
     * @brief Destructor.
     */
    ~Program();


// Public Operations
public:


    /**
     * @brief Initialize from source.
     *
     * @param source Source code.
     */
    void initSource(const std::string& source)
    {
        m_source.initSource(source);
        m_call = m_source.getFunction("call");
    }


    /**
     * @brief Initialize from file.
     *
     * @param filename Path to source file.
     */
    void initFile(const std::string& filename)
    {
        m_source.initFile(filename);
        m_call = m_source.getFunction("call");
    }


    /**
     * @brief Call program.
     *
     * @param object Simulation object.
     * @param dt     Simulation time step.
     */
    void operator()(simulator::Object& object, core::units::Duration dt) const;


// Private Data Members
private:


    /// Source.
    Source m_source;

    /// Call function.
    Handle<PyObject> m_call;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */


#pragma once

/* ************************************************************************ */

// Simulator
#include "core/Units.hpp"
#include "simulator/Object.hpp"
#include "simulator/Simulation.hpp"

// Module
#include "Python.hpp"
#include "Source.hpp"

/* ************************************************************************ */

namespace plugin {
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
    void initSource(const String& source)
    {
        m_source.initSource(source);
        m_call = m_source.getFunction("call");
    }


    /**
     * @brief Initialize from file.
     *
     * @param filename Path to source file.
     */
    void initFile(const FilePath& filename)
    {
        m_source.initFile(filename);
        m_call = m_source.getFunction("call");
    }


    /**
     * @brief Call program.
     *
     * @param object     Simulation object.
     * @param simulation Current simulation.
     * @param dt         Simulation time step.
     */
    void operator()(simulator::Object& object, simulator::Simulation& simulation, units::Duration dt) const;


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

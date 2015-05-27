
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

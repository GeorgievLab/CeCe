
#pragma once

/* ************************************************************************ */

// Simulator
#include "render/Object.hpp"

// Python
#include <boost/python.hpp>

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


    /// Code module.
    boost::python::object m_module;

    /// Call function.
    boost::python::object m_call;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */

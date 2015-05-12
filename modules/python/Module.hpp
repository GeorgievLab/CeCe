
#pragma once

/* ************************************************************************ */

// Simulator
#include "simulator/Module.hpp"
#include "render/Context.hpp"

// Boost
#include <boost/filesystem/path.hpp>

// Python
#include <Python.h>

/* ************************************************************************ */

namespace module {
namespace python {

/* ************************************************************************ */

/**
 * @brief Module for Python support.
 */
class Module : public simulator::Module
{

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param simulation
     * @param path       Path to source file.
     */
    explicit Module(simulator::Simulation& simulation, const boost::filesystem::path& filename);


    /**
     * @brief Destructor.
     */
    ~Module();


// Public Accessors
public:



// Public Mutators
public:



// Public Operations
public:


    /**
     * @brief Update module state.
     *
     * @param dt    Simulation time step.
     * @param simulation
     */
    void update(units::Duration dt, simulator::Simulation& simulation) override;


// Private Data Members
private:


    /// Precompiled python code.
    PyCodeObject* m_compiledCode = nullptr;
};

/* ************************************************************************ */

}
}

/* ************************************************************************ */

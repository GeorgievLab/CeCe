
#pragma once

/* ************************************************************************ */

// Simulator
#include "simulator/Module.hpp"
#include "render/Context.hpp"

// Boost
#include <boost/filesystem/path.hpp>

// Python
#include <boost/python.hpp>

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


    /**
     * @brief Configure module.
     *
     * @param config
     */
    void configure(const simulator::ConfigurationBase& config) override;


#if ENABLE_RENDER
    /**
     * @brief Initialize module for rendering.
     *
     * @param context
     */
    void drawInit(render::Context& context) override;
#endif


#if ENABLE_RENDER
    /**
     * @brief Render module.
     *
     * @param context
     * @param world
     */
    void draw(render::Context& context, const simulator::Simulation& simulation) override;
#endif


// Private Data Members
private:


    /// Code module.
    boost::python::object m_module;

    /// Configure function.
    boost::python::object m_configureFn;

    /// Update function.
    boost::python::object m_updateFn;

#if ENABLE_RENDER
    /// Draw init function.
    boost::python::object m_drawInitFn;
#endif

#if ENABLE_RENDER
    /// Draw function.
    boost::python::object m_drawFn;
#endif
};

/* ************************************************************************ */

}
}

/* ************************************************************************ */

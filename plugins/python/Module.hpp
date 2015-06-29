
#pragma once

/* ************************************************************************ */

// Simulator
#include "simulator/Module.hpp"
#include "render/Context.hpp"

// Module
#include "Python.hpp"
#include "Handle.hpp"
#include "Source.hpp"

/* ************************************************************************ */

namespace plugin {
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
     * @param name Module name. Can be path to python source.
     */
    explicit Module(const std::string& name);


// Public Operations
public:


    /**
     * @brief Configure module.
     *
     * @param config
     */
    void configure(const simulator::Configuration& config, simulator::Simulation& simulation) override;


    /**
     * @brief Update module state.
     *
     * @param dt    Simulation time step.
     * @param simulation
     */
    void update(units::Duration dt, simulator::Simulation& simulation) override;


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


    /// Source.
    Source m_source;

    /// Configure function.
    Handle<PyObject> m_configureFn;

    /// Update function.
    Handle<PyObject> m_updateFn;

#if ENABLE_RENDER
    /// Draw function.
    Handle<PyObject> m_drawFn;
#endif
};

/* ************************************************************************ */

}
}

/* ************************************************************************ */


#pragma once

/* ************************************************************************ */

// C++
#include <string>

// Simulator
#include "simulator/Object.hpp"
#include "render/Context.hpp"

// Module
#include "Python.hpp"
#include "Handle.hpp"
#include "Source.hpp"

/* ************************************************************************ */

namespace module {
namespace python {

/* ************************************************************************ */

/**
 * @brief Python defined simulation object.
 */
class Object : public simulator::Object
{

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param name Module name. Can be path to python source.
     */
    explicit Object(simulator::Simulation& simulation, const std::string& name, Type type = Type::Static);


// Public Operations
public:


    /**
     * @brief Configure object.
     *
     * @param config
     * @param simulation
     */
    void configure(const simulator::ConfigurationBase& config, simulator::Simulation& simulation) override;


    /**
     * @brief Update object state.
     *
     * @param dt Simulation time step.
     */
    void update(units::Duration dt) override;


#if ENABLE_RENDER
    /**
     * @brief Render object.
     *
     * @param context
     */
    void draw(render::Context& context) override;
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

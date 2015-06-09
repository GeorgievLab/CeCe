/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* ************************************************************************ */

#pragma once

/* ************************************************************************ */

// Simulator
#include "core/Vector.hpp"
#include "core/Units.hpp"
#include "core/Grid.hpp"
#include "core/StaticArray.hpp"
#include "simulator/Module.hpp"
#include "simulator/Object.hpp"

#ifdef ENABLE_RENDER
#include "render/Context.hpp"
#include "render/Object.hpp"
#include "render/GridColor.hpp"
#endif

// Module
#include "Lattice.hpp"

/* ************************************************************************ */

namespace module {
namespace streamlines {

/* ************************************************************************ */

/**
 * @brief Module for streamlines.
 */
class Module : public simulator::Module
{

// Public Ctors & Dtors
public:


    /**
     * @brief Destructor.
     */
    virtual ~Module();


// Public Accessors
public:


    /**
     * @brief Returns grid.
     *
     * @return
     */
    Lattice& getLattice() noexcept
    {
        return m_lattice;
    }


    /**
     * @brief Returns grid.
     *
     * @return
     */
    const Lattice& getLattice() const noexcept
    {
        return m_lattice;
    }


    /**
     * @brief Returns flow speed.
     *
     * @return
     */
    float getFlowSpeed() const noexcept
    {
        return m_flowSpeed;
    }


// Public Mutators
public:


    /**
     * @brief Set flow speed.
     *
     * @param speed.
     */
    void setFlowSpeed(float speed) noexcept
    {
        m_flowSpeed = speed;
    }


// Public Operations
public:


    /**
     * @brief Initialize lattice.
     *
     * @param size
     */
    void init(Size size);


    /**
     * @brief Update module state.
     *
     * @param dt         Simulation time step.
     * @param simulation
     */
    void update(units::Duration dt, simulator::Simulation& simulation) override;


    /**
     * @brief Configure module.
     *
     * @param config
     */
    void configure(const simulator::Configuration& config, simulator::Simulation& simulation) override;


#ifdef ENABLE_RENDER
    /**
     * @brief Render module.
     *
     * @param context
     * @param simulation
     */
    void draw(render::Context& context, const simulator::Simulation& simulation) override;
#endif


// Private Data Members
private:

    /// Flow speed.
    float m_flowSpeed = 50.f;

    /// Lattice.
    Lattice m_lattice;

#if ENABLE_RENDER && OPT_DRAW_VELOCITY
    /// Rendering grid with filled cells.
    render::ObjectPtr<render::GridColor> m_drawable;
#endif

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */

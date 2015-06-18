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
#include "core/compatibility.hpp"
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
#include "render/GridVector.hpp"
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
     * @brief Constructor.
     */
    Module();


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
    Lattice& getLattice() NOEXCEPT
    {
        return m_lattice;
    }


    /**
     * @brief Returns grid.
     *
     * @return
     */
    const Lattice& getLattice() const NOEXCEPT
    {
        return m_lattice;
    }


    /**
     * @brief Returns maximum flow velocity.
     *
     * @return
     */
    units::Velocity getVelocityMax() const NOEXCEPT
    {
        return m_velocityMax;
    }


    /**
     * @brief Returns inflow velocity.
     *
     * @return
     */
    units::Velocity getVelocityInflow() const NOEXCEPT
    {
        return m_velocityInflow;
    }


    /**
     * @brief Returns fluid viscosity.
     *
     * @return
     */
    units::Viscosity getViscosity() const NOEXCEPT
    {
        return m_viscosity;
    }


// Public Mutators
public:


    /**
     * @brief Set maximum velocity.
     *
     * @param velocity
     */
    void setVelocityMax(units::Velocity velocity) NOEXCEPT
    {
        m_velocityMax = velocity;
    }


    /**
     * @brief Set inflow velocity.
     *
     * @param velocity
     */
    void setVelocityInflow(units::Velocity velocity) NOEXCEPT
    {
        m_velocityInflow = velocity;
    }


    /**
     * @brief Set fluid viscosity.
     *
     * @param viscosity
     */
    void setViscosity(units::Viscosity viscosity) NOEXCEPT
    {
        m_viscosity = viscosity;
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

// Protected Operations
protected:


    /**
     * @brief Check time step condition: dt <= dx / u_max.
     *
     * @param dt         Time step.
     * @param simulation Simulation.
     */
    void checkTimeStepCondition(units::Duration dt, const simulator::Simulation& simulation);


    /**
     * @brief Update dynamic obstacle map from dynamic objects.
     *
     * @param simulation
     */
    void updateDynamicObstacleMap(const simulator::Simulation& simulation);


    /**
     * @brief Apply streamlines to objects.
     *
     * @param simulation
     */
    void applyToObjects(const simulator::Simulation& simulation);


    /**
     * @brief Apply boundary conditions.
     *
     * @param simulation
     */
    void applyBoundaryConditions(const simulator::Simulation& simulation);


// Private Data Members
private:

    /// Maximum flow velocity.
    units::Velocity m_velocityMax = 50.f;

    /// In-flow velocity.
    units::Velocity m_velocityInflow = 10.f;

    /// Fluid viscosity.
    units::Viscosity m_viscosity = 1.f;

    /// Lattice.
    Lattice m_lattice;

#if ENABLE_RENDER && OPT_DRAW_VELOCITY
    /// Rendering grid with filled cells.
    render::ObjectPtr<render::GridColor> m_drawable;
#endif

#if ENABLE_RENDER
    /// Render grid for velocities
    render::ObjectPtr<render::GridVector> m_drawableVector;
#endif

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */

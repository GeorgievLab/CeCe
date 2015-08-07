/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
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
#include "core/Real.hpp"
#include "core/StaticArray.hpp"
#include "simulator/Module.hpp"
#include "simulator/Object.hpp"

#ifdef ENABLE_RENDER
#include "render/Context.hpp"
#include "render/Object.hpp"
#include "render/GridColor.hpp"
#include "render/GridVector.hpp"
#endif

// Plugin
#include "Lattice.hpp"
#include "ObstacleMap.hpp"

#if THREAD_SAFE
#include "core/Mutex.hpp"
#endif

/* ************************************************************************ */

namespace plugin {
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
     * @brief Returns inflow velocity.
     *
     * @return
     */
    VelocityVector getVelocityInflow() const noexcept
    {
        return m_velocityInflow;
    }


    /**
     * @brief Returns fluid kinematic viscosity.
     *
     * @return
     */
    units::KinematicViscosity getKinematicViscosity() const noexcept
    {
        return m_kinematicViscosity;
    }


    /**
     * @brief Returns fixup coefficient.
     *
     * @return
     */
    RealType getCoefficient() const noexcept
    {
        return m_coefficient;
    }


    /**
     * @brief Returns inner iteration count.
     *
     * @return
     */
    unsigned int getIterations() const noexcept
    {
        return m_iterations;
    }


// Public Mutators
public:


    /**
     * @brief Set inflow velocity.
     *
     * @param velocity
     */
    void setVelocityInflow(VelocityVector velocity) noexcept
    {
        m_velocityInflow = velocity;
    }


    /**
     * @brief Set fluid viscosity.
     *
     * @param viscosity
     */
    void setKinematicViscosity(units::KinematicViscosity viscosity) noexcept
    {
        m_kinematicViscosity = viscosity;
    }


    /**
     * @brief Setup static obstacle map.
     *
     * @param map
     */
    void setStaticObstacleMap(const ObstacleMap& map);


    /**
     * @brief Set fixup coefficient.
     *
     * @param coefficient
     */
    void setCoefficient(RealType coefficient) noexcept
    {
        m_coefficient = coefficient;
    }


    /**
     * @brief Set inner iteration count.
     *
     * @param iterations
     */
    void setIterations(unsigned int iterations) noexcept
    {
        m_iterations = iterations;
    }


// Public Operations
public:


    /**
     * @brief Initialize lattice.
     */
    void init();


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
     * @brief Update dynamic obstacle map from dynamic objects.
     *
     * @param simulation
     * @param vMax
     */
    void updateDynamicObstacleMap(const simulator::Simulation& simulation, const VelocityVector& vMax);


    /**
     * @brief Apply streamlines to objects.
     *
     * @param simulation
     * @param vMax
     */
    void applyToObjects(const simulator::Simulation& simulation, const VelocityVector& vMax);


    /**
     * @brief Apply boundary conditions.
     *
     * @param simulation
     * @param vMax
     */
    void applyBoundaryConditions(const simulator::Simulation& simulation, const VelocityVector& vMax);


    /**
     * @brief Calculate inlet velocity profile.
     *
     * @param coord
     *
     * @return
     */
    VelocityVector inletVelocityProfile(Lattice::CoordinateType coord) const noexcept;


// Private Data Members
private:

    /// In-flow velocity.
    VelocityVector m_velocityInflow{units::um_s(10.f), Zero};

    /// Fluid viscosity (of Water).
    units::KinematicViscosity m_kinematicViscosity = units::mm2_s(0.658);

    /// Fixup coefficient.
    RealType m_coefficient = 1;

    /// Number of inner iterations.
    unsigned int m_iterations = 1;

    /// Lattice.
    Lattice m_lattice;

#if ENABLE_RENDER && DEV_DRAW_VELOCITY
    /// Rendering grid with filled cells.
    render::ObjectPtr<render::GridColor> m_drawable;
#endif

#if ENABLE_RENDER && DEV_DRAW_VELOCITY
    /// Render grid for velocities
    render::ObjectPtr<render::GridVector> m_drawableVector;
#endif

#if THREAD_SAFE
    /// Access mutex.
    mutable Mutex m_mutex;
#endif

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */

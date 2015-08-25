/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* Author: Hynek Kasl <hkasl@students.zcu.cz>                               */
/* ************************************************************************ */

#pragma once

/* ************************************************************************ */

// Simulator
#include "core/Vector.hpp"
#include "core/Units.hpp"
#include "core/Grid.hpp"
#include "core/Real.hpp"
#include "core/StaticArray.hpp"
#include "simulator/Simulation.hpp"
#include "simulator/Module.hpp"
#include "simulator/Object.hpp"
#include "simulator/Obstacle.hpp"

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

// Public Enums
public:


    /**
     * @brief Layout type for side.
     */
    enum class LayoutType
    {
        None,
        Barrier,
        Inlet,
        Outlet
    };


    /**
     * @brief Layout position.
     */
    enum LayoutPosition
    {
        LayoutPosRight   = 0,
        LayoutPosLeft    = 1,
        LayoutPosTop     = 2,
        LayoutPosBottom  = 3,
        LayoutPosCount
    };


// Public Constants
public:


#if ENABLE_RENDER && DEV_PLUGIN_streamlines_RENDER
    /// Flag for drawing velocities
    static constexpr DrawFlags DRAW_VELOCITY = 0x1;
#endif


// Public Structures
public:


    /**
     * @brief Streamlines layout.
     */
    struct Layout : public StaticArray<LayoutType, LayoutPosCount>
    {
        using StaticArray<LayoutType, LayoutPosCount>::StaticArray;
    };


    /**
     * @brief Layout inlet velocities.
     */
    struct InletVelocities : public StaticArray<units::Velocity, LayoutPosCount>
    {
        using StaticArray<units::Velocity, LayoutPosCount>::StaticArray;
    };


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
     * @brief Returns layout inlet velocities.
     *
     * @return
     */
    InletVelocities getInletVelocities() const noexcept
    {
        return m_inletVelocities;
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
     * @brief Returns relaxation time.
     *
     * @return
     */
    RealType getTau() const noexcept
    {
        return m_tau;
    }


    /**
     * @brief Returns inner iteration count.
     *
     * @return
     */
    simulator::IterationCount getIterations() const noexcept
    {
        return m_iterations;
    }


    /**
     * @brief Returns init iteration count.
     *
     * @return
     */
    simulator::IterationCount getInitIterations() const noexcept
    {
        return m_initIterations;
    }


    /**
     * @brief Returns layout description.
     *
     * @return
     */
    const Layout& getLayout() const noexcept
    {
        return m_layout;
    }


    /**
     * @brief If dynamic objects are used as obstacles.
     *
     * @return
     */
    bool isDynamicObjectsObstacles() const noexcept
    {
        return m_dynamicObjectsObstacles;
    }


#if ENABLE_RENDER && DEV_PLUGIN_streamlines_RENDER
    /**
     * @brief If velocity vectors are drawn.
     *
     * @return
     */
    bool isDrawVectors() const noexcept
    {
        return getDrawFlags() & DRAW_VELOCITY;
    }
#endif


// Public Mutators
public:


    /**
     * @brief Set layout inlet velocities.
     *
     * @param velocities
     */
    void setInletVelocities(InletVelocities velocities) noexcept
    {
        m_inletVelocities = velocities;
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
     * @brief Set relaxation time.
     *
     * @param tau
     */
    void setTau(RealType tau) noexcept
    {
        m_tau = tau;
    }


    /**
     * @brief Set inner iteration count.
     *
     * @param iterations
     */
    void setIterations(simulator::IterationCount iterations) noexcept
    {
        m_iterations = iterations;
    }


    /**
     * @brief Set init iteration count.
     *
     * @param iterations
     */
    void setInitIterations(simulator::IterationCount iterations) noexcept
    {
        m_initIterations = iterations;
    }


    /**
     * @brief Set layout.
     *
     * @param layout
     */
    void setLayout(Layout layout) noexcept
    {
        m_layout = layout;
    }


    /**
     * @brief Enable or disable dynamic objects obstacles.
     *
     * @param flag
     */
    void setDynamicObjectsObstacles(bool flag) noexcept
    {
        m_dynamicObjectsObstacles = flag;
    }


#if ENABLE_RENDER && DEV_PLUGIN_streamlines_RENDER
    /**
     * @brief Draw velocity vectors.
     *
     * @param flag
     */
    void setDrawVectors(bool flag) noexcept
    {
        setDrawFlags(getDrawFlags() | DRAW_VELOCITY);
    }
#endif


// Public Operations
public:


    /**
     * @brief Initialize lattice.
     *
     * @param simulation
     */
    void init(simulator::Simulation& simulation);


    /**
     * @brief Initialize barriers.
     *
     * @param simulation
     */
    void initBarriers(simulator::Simulation& simulation);


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


    /**
     * @brief Calculate maximum velocity.
     *
     * @param dl Grid cell size.
     *
     * @return
     */
    VelocityVector calculateMaxVelocity(PositionVector dl) const noexcept;


// Protected Operations
protected:


    /**
     * @brief Update obstacle map from objects.
     *
     * @param simulation
     * @param vMax
     */
    void updateObstacleMap(const simulator::Simulation& simulation, const VelocityVector& vMax);


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
     * @param pos
     * @param inlets
     *
     * @return
     */
    VelocityVector inletVelocityProfile(
        Lattice::CoordinateType coord, LayoutPosition pos,
        DynamicArray<StaticArray<Lattice::CoordinateType, 2>> inlets
    ) const noexcept;


    /**
     * @brief Calculate coefficient.
     *
     * @param step Time step.
     * @param dl   Grid cell size.
     *
     * @return
     */
    RealType calculateCoefficient(units::Time step, PositionVector dl) const noexcept;


    /**
     * @brief Calculate viscosity from relaxation time.
     *
     * @return
     */
    RealType calculateViscosity() const noexcept
    {
        return (getTau() - 0.5) / 3.0;
    }


    /**
     * @brief Init border barrier.
     *
     * @param simulation
     * @param pos
     */
    void initBorderBarrier(simulator::Simulation& simulation, LayoutPosition pos);


    /**
     * @brief Init border inlet/outlet.
     *
     * @param simulation
     * @param pos
     * @param vMax
     */
    void initBorderInletOutlet(const simulator::Simulation& simulation,
        LayoutPosition pos, const VelocityVector& vMax);


// Private Data Members
private:

    /// Inlet velocities
    InletVelocities m_inletVelocities;

    /// Fluid viscosity (of Water).
    units::KinematicViscosity m_kinematicViscosity = units::mm2_s(0.658);

    /// Relaxation time.
    RealType m_tau = 1;

    /// Fixup coefficient.
    RealType m_coefficient = 1;

    /// Number of inner iterations.
    simulator::IterationCount m_iterations = 5;

    /// Number of init iterations.
    simulator::IterationCount m_initIterations = 100;

    /// Lattice.
    Lattice m_lattice;

    /// Streamlines layout.
    Layout m_layout;

    /// Barriers created for layout.
    StaticArray<ViewPtr<simulator::Obstacle>, LayoutPosCount> m_layoutBarriers;

    /// Use dynamic objects as obstacles
    bool m_dynamicObjectsObstacles = true;

#if ENABLE_RENDER && DEV_PLUGIN_streamlines_RENDER
    /// Render grid for velocities
    render::ObjectPtr<render::GridVector> m_drawable;
#endif

#if THREAD_SAFE
    /// Access mutex.
    mutable Mutex m_mutex;
#endif

};

/* ************************************************************************ */

/**
 * @brief Read layout type from stream.
 *
 * @param is   Input stream.
 * @param type Output type.
 *
 * @return is.
 */
InStream& operator>>(InStream& is, Module::LayoutType& type);

/* ************************************************************************ */

}
}

/* ************************************************************************ */

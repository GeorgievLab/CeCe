/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/*                                                                          */
/* This file is part of CeCe.                                               */
/*                                                                          */
/* CeCe is free software: you can redistribute it and/or modify             */
/* it under the terms of the GNU General Public License as published by     */
/* the Free Software Foundation, either version 3 of the License, or        */
/* (at your option) any later version.                                      */
/*                                                                          */
/* CeCe is distributed in the hope that it will be useful,                  */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of           */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            */
/* GNU General Public License for more details.                             */
/*                                                                          */
/* You should have received a copy of the GNU General Public License        */
/* along with CeCe.  If not, see <http://www.gnu.org/licenses/>.            */
/*                                                                          */
/* ************************************************************************ */

#pragma once

/* ************************************************************************ */

// Simulator
#include "core/Vector.hpp"
#include "core/Units.hpp"
#include "core/Grid.hpp"
#include "core/Real.hpp"
#include "core/StaticArray.hpp"
#include "core/UniquePtr.hpp"
#include "core/OutStream.hpp"
#include "core/InStream.hpp"
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
    /// Flag for debug drawing.
    static constexpr DrawFlags DRAW_DEBUG = 0x1;
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
    simulator::IterationCount getInnerIterations() const noexcept
    {
        return m_innerIterations;
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
     * @brief If debug data is drawn.
     *
     * @return
     */
    bool isDebugDraw() const noexcept
    {
        return getDrawFlags() & DRAW_DEBUG;
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
     * @brief Set layout inlet velocities.
     *
     * @param position Inlet position.
     * @param velocity
     */
    void setInletVelocity(LayoutPosition position, units::Velocity velocity) noexcept
    {
        m_inletVelocities[position] = velocity;
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
    void setInnerIterations(simulator::IterationCount iterations) noexcept
    {
        m_innerIterations = iterations;
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
     * @brief Draw debug data.
     *
     * @param flag
     */
    void setDebugDraw(bool flag) noexcept
    {
        setDrawFlags(getDrawFlags() | DRAW_DEBUG);
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
     * @brief Load module configuration.
     *
     * @param simulation Current simulation.
     * @param config     Source configuration.
     */
    void loadConfig(simulator::Simulation& simulation, const simulator::Configuration& config) override;


    /**
     * @brief Update module state.
     *
     * @param simulation Simulation object.
     * @param dt         Simulation time step.
     */
    void update(simulator::Simulation& simulation, units::Time dt) override;


#if ENABLE_RENDER
    /**
     * @brief Render module.
     *
     * @param simulation Current simulation.
     * @param context    Rendering context.
     */
    void draw(const simulator::Simulation& simulation, render::Context& context) override;
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
    simulator::IterationCount m_innerIterations = 5;

    /// Number of init iterations.
    simulator::IterationCount m_initIterations = 100;

    /// Lattice.
    Lattice m_lattice;

    /// Streamlines layout.
    Layout m_layout;

    /// Barriers created for layout.
    StaticArray<ViewPtr<simulator::Obstacle>, LayoutPosCount> m_layoutBarriers;

    /// Use dynamic objects as obstacles
    bool m_dynamicObjectsObstacles = false;

#if ENABLE_RENDER && DEV_PLUGIN_streamlines_RENDER
    /// Render grid for velocities
    render::ObjectPtr<render::GridVector> m_drawableVelocities;
#endif

#if ENABLE_RENDER && DEV_PLUGIN_streamlines_RENDER
    /// Rendering grid with filled cells.
    render::ObjectPtr<render::GridColor> m_drawableObstacles;
#endif

#if THREAD_SAFE
    /// Access mutex.
    mutable Mutex m_mutex;
#endif

    /// Outstream for streamlines data
    UniquePtr<OutStream> m_dataOut;
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

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

// CeCe config
#include "cece/config.hpp"

/* ************************************************************************ */

// CeCe
#include "cece/core/Vector.hpp"
#include "cece/core/Units.hpp"
#include "cece/core/Grid.hpp"
#include "cece/core/Real.hpp"
#include "cece/core/StaticArray.hpp"
#include "cece/core/UniquePtr.hpp"
#include "cece/core/OutStream.hpp"
#include "cece/core/InStream.hpp"
#include "cece/core/FilePath.hpp"
#include "cece/module/Module.hpp"
#include "cece/object/Object.hpp"
#include "cece/simulator/Simulation.hpp"

#ifdef CECE_ENABLE_RENDER
#  include "cece/render/Context.hpp"
#  include "cece/render/Object.hpp"
#  include "cece/render/GridColorColorMap.hpp"
#  include "cece/render/GridVector.hpp"
#endif

#ifdef CECE_THREAD_SAFE
#  include "cece/core/Mutex.hpp"
#endif

// Plugin
#include "cece/plugins/streamlines/Lattice.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace streamlines {

/* ************************************************************************ */

/**
 * @brief Module for streamlines.
 */
class Module : public module::Module
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


#if defined(CECE_ENABLE_RENDER) && DEV_PLUGIN_streamlines_RENDER

    /// Flag for debug drawing.
    static constexpr DrawFlags DRAW_DEBUG_MAGNITUDE = 0x01;
    static constexpr DrawFlags DRAW_DEBUG_DIRECTION = 0x02;
    static constexpr DrawFlags DRAW_DEBUG_OBSTACLES = 0x04;

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


#if defined(CECE_ENABLE_RENDER) && DEV_PLUGIN_streamlines_RENDER

    /**
     * @brief If debug flag is set.
     *
     * @param flag
     *
     * @return
     */
    bool checkDebugDraw(DrawFlags flag) const noexcept
    {
        return getDrawFlags() & flag;
    }


    /**
     * @brief Get debug velocity magnitude scale.
     *
     * @return
     */
    RealType getDebugMagnitudeScale() const noexcept
    {
        return m_debugMagnitudeScale;
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


#if defined(CECE_ENABLE_RENDER) && DEV_PLUGIN_streamlines_RENDER

    /**
     * @brief Set debug velocity magnitude scale.
     *
     * @param value
     */
    void setDebugMagnitudeScale(RealType value) noexcept
    {
        m_debugMagnitudeScale = value;
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
    void loadConfig(simulator::Simulation& simulation, const config::Configuration& config) override;


    /**
     * @brief Update module state.
     *
     * @param simulation Simulation object.
     * @param dt         Simulation time step.
     */
    void update(simulator::Simulation& simulation, units::Time dt) override;


#ifdef CECE_ENABLE_RENDER

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
     * @param dt
     * @param vMax
     */
    void applyToObjects(const simulator::Simulation& simulation, units::Time dt, const VelocityVector& vMax);

    /**
     * @brief Apply streamlines to object.
     *
     * @param object
     * @param simulation
     * @param dt
     * @param vMax
     */
    void applyToObject(object::Object& object, const simulator::Simulation& simulation,
        units::Time dt, const VelocityVector& vMax);


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


    /**
     * @brief Store streamlines data to file.
     *
     * @param filename
     */
    void storeToFile(const FilePath& filename);


    /**
     * @brief Load streamlines data from file.
     *
     * @param filename
     */
    void loadFromFile(const FilePath& filename);


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

    /// Path to initialization file.
    FilePath m_initFile;

    /// Lattice.
    Lattice m_lattice;

    /// Streamlines layout.
    Layout m_layout;

    /// Layout type - used for initialization.
    String m_layoutType;

    /// Barriers created for layout.
    StaticArray<ViewPtr<object::Object>, LayoutPosCount> m_layoutBarriers;

    /// Use dynamic objects as obstacles
    bool m_dynamicObjectsObstacles = false;

#if defined(CECE_ENABLE_RENDER) && DEV_PLUGIN_streamlines_RENDER
    /// Render grid for velocities
    render::ObjectPtr<render::GridVector> m_drawableDirections;

    /// Rendering grid with filled cells.
    render::ObjectPtr<render::GridColorColorMap> m_drawableDebug;

    RealType m_debugMagnitudeScale = 2;
#endif

#ifdef CECE_THREAD_SAFE
    /// Access mutex.
    mutable Mutex m_mutex;
#endif

    /// Outstream for streamlines data
    UniquePtr<OutStream> m_dataOut;

    /// If cell density should be stored.
    bool m_dataOutDensity = false;

    /// If cell populations should be stored.
    bool m_dataOutPopulations = false;
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
}

/* ************************************************************************ */

/* ************************************************************************ */
/* Georgiev Lab (c) 2015-2016                                               */
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
#include "cece/simulator/IterationType.hpp"

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
     * @brief Inlet velocity profile type.
     */
    enum class InletType
    {
        Auto,
        Constant,
        Parabolic
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


#if defined(CECE_ENABLE_RENDER)

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


    /**
     * @brief Inlet velocity profiles.
     */
    struct InletTypes : public StaticArray<InletType, LayoutPosCount>
    {
        using StaticArray<InletType, LayoutPosCount>::StaticArray;
    };


// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param simulation
     */
    Module(simulator::Simulation& simulation);


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
    const InletVelocities& getInletVelocities() const noexcept
    {
        return m_inletVelocities;
    }


    /**
     * @brief Returns inlet velocity profile types.
     *
     * @return
     */
    const InletTypes& getInletTypes() const noexcept
    {
        return m_inletTypes;
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
     * @brief Returns init iteration count.
     *
     * @return
     */
    simulator::IterationCount getInitIterations() const noexcept
    {
        return m_initIterations;
    }


    /**
     * @brief Returns init iteration count.
     *
     * @return
     */
    simulator::IterationCount getInnerIterations() const noexcept
    {
        return m_innerIterations;
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


#if defined(CECE_ENABLE_RENDER)

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


    /**
     * @brief Returns characteristic length.
     *
     * @return
     */
    units::Length getCharLength() const noexcept
    {
        return m_charLength;
    }


    /**
     * @brief Returns characteristic time.
     *
     * @return
     */
    units::Time getCharTime() const noexcept
    {
        return m_charTime;
    }


    /**
     * @brief Returns characteristic velocity.
     *
     * @return
     */
    units::Velocity getCharVelocity() const noexcept
    {
        return getCharLength() / getCharTime();
    }


    /**
     * @brief Returns number of nodes in LB along characteristic length.
     *
     * @return
     */
    unsigned int getNumberNodes() const noexcept
    {
        return m_numberNodes;
    }


    /**
     * @brief Returns number of time steps in LB for units conversion.
     *
     * @return
     */
    unsigned int getNumberSteps() const noexcept
    {
        return m_numberSteps;
    }


    /**
     * @brief Returns if streamlines are dynamic during simulation.
     *
     * @return
     */
    bool isDynamic() const noexcept
    {
        return m_dynamic;
    }


    /**
     * @brief Returns fluid dynamics.
     *
     * @return
     */
    ViewPtr<Dynamics> getFluidDynamics() const noexcept
    {
        return m_fluidDynamics;
    }


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
     * @brief Set inlet velocity profile types.
     *
     * @param types
     */
    void setInletTypes(InletTypes types) noexcept
    {
        m_inletTypes = types;
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
     * @brief Set init iteration count.
     *
     * @param iterations
     */
    void setInitIterations(simulator::IterationCount iterations) noexcept
    {
        m_initIterations = iterations;
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


#if defined(CECE_ENABLE_RENDER)

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


    /**
     * @brief Set characteristic length.
     *
     * @param length
     */
    void setCharLength(units::Length length) noexcept
    {
        m_charLength = length;
    }


    /**
     * @brief Set characteristic time.
     *
     * @param time
     */
    void setCharTime(units::Time time) noexcept
    {
        m_charTime = time;
    }


    /**
     * @brief Set number of nodes in LB for units conversion.
     *
     * @param nodes
     */
    void setNumberNodes(unsigned int nodes) noexcept
    {
        Assert(nodes > 0);
        m_numberNodes = nodes;
    }


    /**
     * @brief Set number of time steps in LB for units conversion.
     *
     * @param steps
     */
    void setNumberSteps(unsigned int steps) noexcept
    {
        Assert(steps > 0);
        m_numberSteps = steps;
    }


    /**
     * @brief Set if streamlines should be dynamic.
     *
     * @param dynamic
     */
    void setDynamic(bool dynamic)
    {
        m_dynamic = dynamic;
    }


    /**
     * @brief Set fluid dynamics.
     *
     * @param dynamics
     */
    void setFluidDynamics(UniquePtr<Dynamics> dynamics) noexcept
    {
        m_fluidDynamics = std::move(dynamics);
    }


// Public Operations
public:


    /**
     * @brief Initialize lattice.
     *
     * @param termFlag
     */
    void init(AtomicBool& termFlag) override;


    /**
     * @brief Initialize barriers.
     */
    void initBarriers();


    /**
     * @brief Load module configuration.
     *
     * @param config Source configuration.
     */
    void loadConfig(const config::Configuration& config) override;


    /**
     * @brief Store module configuration.
     *
     * @param config Output configuration.
     */
    void storeConfig(config::Configuration& config) const override;


    /**
     * @brief Update module state.
     */
    void update() override;


#ifdef CECE_ENABLE_RENDER

    /**
     * @brief Render module.
     *
     * @param context Rendering context.
     */
    void draw(render::Context& context) override;

#endif


    /**
     * @brief Convert length from LB to physical.
     *
     * @param length
     *
     * @return
     */
    units::Length convertLength(RealType length) const noexcept
    {
        const auto charLength = m_charLength / getNumberNodes();
        return charLength * length;
    }


    /**
     * @brief Convert length from physical to LB.
     *
     * @param length
     *
     * @return
     */
    RealType convertLength(units::Length length) const noexcept
    {
        const auto charLength = m_charLength / getNumberNodes();
        return length / charLength;
    }


    /**
     * @brief Convert velocity from LB to physical.
     *
     * @param vel
     *
     * @return
     */
    units::Velocity convertVelocity(RealType vel) const noexcept
    {
        const auto charTime = m_charTime / getNumberSteps();
        const auto charLength = m_charLength / getNumberNodes();
        return charLength / charTime * vel;
    }


    /**
     * @brief Convert velocity from LB to physical.
     *
     * @param vel
     *
     * @return
     */
    VelocityVector convertVelocity(Vector<RealType> vel) const noexcept
    {
        const auto charTime = m_charTime / getNumberSteps();
        const auto charLength = m_charLength / getNumberNodes();
        return charLength / charTime * vel;
    }


    /**
     * @brief Convert velocity from physical to LB.
     *
     * @param vel
     *
     * @return
     */
    RealType convertVelocity(units::Velocity vel) const noexcept
    {
        const auto charTime = m_charTime / getNumberSteps();
        const auto charLength = m_charLength / getNumberNodes();
        return charTime / charLength * vel;
    }


    /**
     * @brief Convert velocity from physical to LB.
     *
     * @param vel
     *
     * @return
     */
    Vector<RealType> convertVelocity(VelocityVector vel) const noexcept
    {
        const auto charTime = m_charTime / getNumberSteps();
        const auto charLength = m_charLength / getNumberNodes();
        return charTime / charLength * vel;
    }


// Protected Operations
protected:


    /**
     * @brief Create fluid dynamics.
     *
     * @return
     */
    virtual UniquePtr<Dynamics> createFluidDynamics() const;


    /**
     * @brief Create border dynamics.
     *
     * @param pos
     *
     * @return
     */
    virtual UniquePtr<Dynamics> createBorderDynamics(LayoutPosition pos) const;


    /**
     * @brief Update obstacle map from objects.
     */
    void updateObstacleMap();


    /**
     * @brief Apply streamlines to objects.
     */
    void applyToObjects();

    /**
     * @brief Apply streamlines to object.
     *
     * @param object
     */
    void applyToObject(object::Object& object);


    /**
     * @brief Apply boundary conditions.
     */
    void applyBoundaryConditions();


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
     * @brief Calculate viscosity from relaxation time.
     *
     * @return
     */
    RealType calculateViscosity() const noexcept;


    /**
     * @brief Calculate viscosity from relaxation time.
     *
     * @return
     */
    RealType calculateTau() const noexcept;


    /**
     * @brief Calculate relaxation frequency.
     *
     * @return
     */
    RealType calculateOmega() const noexcept
    {
        return 1.0 / calculateTau();
    }


    /**
     * @brief Calculate number of time steps from tau.
     *
     * @param tau
     *
     * @return
     */
    unsigned int calculateNumberSteps(RealType tau) const noexcept;


    /**
     * @brief Calculate Reynolds number.
     *
     * @return
     */
    RealType calculateRe() const noexcept
    {
        return getCharLength() * getCharLength() / getCharTime() / getKinematicViscosity();
    }


    /**
     * @brief Init border barrier.
     *
     * @param pos
     */
    void initBorderBarrier(LayoutPosition pos);


    /**
     * @brief Init border inlet/outlet.
     *
     * @param pos
     */
    void initBorderInletOutlet(LayoutPosition pos);


    /**
     * @brief Print streamlines informations.
     */
    virtual void printInfo();


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


    /**
     * @brief Write header into data file.
     */
    void storeDataHeader();


    /**
     * @brief Write data file.
     */
    void storeData();


// Private Data Members
private:

    /// Inlet velocities
    InletVelocities m_inletVelocities;

    /// Inlet velocity profile types.
    InletTypes m_inletTypes;

    /// Fluid viscosity (of Water).
    units::KinematicViscosity m_kinematicViscosity = units::mm2_s(0.658);

    /// Number of init iterations.
    simulator::IterationCount m_initIterations = 0;

    /// Number of inner iterations.
    simulator::IterationCount m_innerIterations = 1;

    /// Characteristic length.
    units::Length m_charLength = units::um(1);

    /// Characteristic time.
    units::Time m_charTime = units::s(1);

    /// Number of LB nodes for units conversions.
    unsigned int m_numberNodes = 1;

    /// Number of LB time steps for units conversions
    unsigned int m_numberSteps = 1;

    /// If streamlines is updated during simulation iterations.
    bool m_dynamic = true;

    /// Path to initialization file.
    FilePath m_initFile;

    /// Lattice.
    Lattice m_lattice;

    /// Streamlines layout.
    Layout m_layout;

    /// Barriers created for layout.
    StaticArray<ViewPtr<object::Object>, LayoutPosCount> m_layoutBarriers;

    /// Boundary dynamics
    StaticArray<UniquePtr<Dynamics>, LayoutPosCount> m_boundaries;

    /// Use dynamic objects as obstacles
    bool m_dynamicObjectsObstacles = false;

#if defined(CECE_ENABLE_RENDER)
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

    UniquePtr<Dynamics> m_fluidDynamics;

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

/**
 * @brief Write layout type from stream.
 *
 * @param os   Output stream.
 * @param type Input type.
 *
 * @return os.
 */
OutStream& operator<<(OutStream& os, const Module::LayoutType& type);

/* ************************************************************************ */

/**
 * @brief Read inlet velocity type.
 *
 * @param is   Input stream.
 * @param type Output type.
 *
 * @return is.
 */
InStream& operator>>(InStream& is, Module::InletType& type);

/* ************************************************************************ */

/**
 * @brief Write inlet velocity type.
 *
 * @param os   Output stream.
 * @param type Input type.
 *
 * @return os.
 */
OutStream& operator<<(OutStream& os, const Module::InletType& type);

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */

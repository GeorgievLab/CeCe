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
#include "cece/plugins/streamlines/Boundaries.hpp"
#include "cece/plugins/streamlines/Lattice.hpp"
#include "cece/plugins/streamlines/Converter.hpp"

/* ************************************************************************ */

namespace cece { inline namespace core { class CsvFile; } }

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

// Public Constants
public:


#if defined(CECE_ENABLE_RENDER)

    /// Flag for debug drawing.
    static constexpr DrawFlags DRAW_DEBUG_MAGNITUDE = 0x01;
    static constexpr DrawFlags DRAW_DEBUG_DIRECTION = 0x02;
    static constexpr DrawFlags DRAW_DEBUG_OBSTACLES = 0x04;

#endif


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
     * @brief Returns units converter.
     *
     * @return
     */
    const Converter& getConverter() const noexcept
    {
        return m_converter;
    }


    /**
     * @brief Returns units converter.
     *
     * @return
     */
    Converter& getConverter() noexcept
    {
        return m_converter;
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
     * @brief Returns boundaries.
     *
     * @return
     */
    const Boundaries& getBoundaries() const noexcept
    {
        return m_boundaries;
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


    /**
     * @brief Returns wall dynamics.
     *
     * @return
     */
    ViewPtr<Dynamics> getWallDynamics() const noexcept
    {
        return m_wallDynamics;
    }


// Public Mutators
public:


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


    /**
     * @brief Set wall dynamics.
     *
     * @param dynamics
     */
    void setWallDynamics(UniquePtr<Dynamics> dynamics) noexcept
    {
        m_wallDynamics = std::move(dynamics);
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


// Protected Operations
protected:


    /**
     * @brief Create fluid dynamics.
     *
     * @return
     */
    virtual UniquePtr<Dynamics> createFluidDynamics() const;


    /**
     * @brief Create wall dynamics.
     *
     * @return
     */
    virtual UniquePtr<Dynamics> createWallDynamics() const;


    /**
     * @brief Create border dynamics.
     *
     * @param pos
     *
     * @return
     */
    virtual UniquePtr<Dynamics> createBorderDynamics(Boundary::Position pos) const;


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

    /// Units converter.
    Converter m_converter;

    /// Number of init iterations.
    simulator::IterationCount m_initIterations = 0;

    /// Number of inner iterations.
    simulator::IterationCount m_innerIterations = 1;

    /// If streamlines is updated during simulation iterations.
    bool m_dynamic = true;

    /// Boundaries.
    Boundaries m_boundaries;

    /// Path to initialization file.
    FilePath m_initFile;

    /// Lattice.
    Lattice m_lattice;

    /// Use dynamic objects as obstacles
    bool m_dynamicObjectsObstacles = false;

#if defined(CECE_ENABLE_RENDER)
    /// Render grid for velocities
    render::ObjectPtr<render::GridVector> m_drawableDirections;

    /// Rendering grid with filled cells.
    render::ObjectPtr<render::GridColorColorMap> m_drawableDebug;

    /// Magnitude scale.
    RealType m_debugMagnitudeScale = 2;
#endif

#ifdef CECE_THREAD_SAFE
    /// Access mutex.
    mutable Mutex m_mutex;
#endif

    UniquePtr<Dynamics> m_fluidDynamics;

    UniquePtr<Dynamics> m_wallDynamics;

    /// Outstream for streamlines data
    UniquePtr<CsvFile> m_dataOut;

    /// If cell density should be stored.
    bool m_dataOutDensity = false;

    /// If cell populations should be stored.
    bool m_dataOutPopulations = false;
};

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */

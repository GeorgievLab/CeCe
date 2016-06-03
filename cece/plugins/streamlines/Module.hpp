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
#include "cece/core/Units.hpp"
#include "cece/core/UnitsCtors.hpp"
#include "cece/core/Vector.hpp"
#include "cece/core/Units.hpp"
#include "cece/core/Grid.hpp"
#include "cece/core/Real.hpp"
#include "cece/core/StaticArray.hpp"
#include "cece/core/UniquePtr.hpp"
#include "cece/core/OutStream.hpp"
#include "cece/core/InStream.hpp"
#include "cece/core/FilePath.hpp"
#include "cece/core/IterationType.hpp"
#include "cece/module/Module.hpp"
#include "cece/object/Object.hpp"

#ifdef CECE_ENABLE_RENDER
#  include "cece/render/Context.hpp"
#  include "cece/render/Object.hpp"
#  include "cece/render/GridColor.hpp"
#  include "cece/render/GridColorColorMap.hpp"
#endif

#ifdef CECE_THREAD_SAFE
#  include "cece/core/Mutex.hpp"
#endif

// Plugin
#include "cece/plugins/streamlines/Boundaries.hpp"
#include "cece/plugins/streamlines/Lattice.hpp"
#include "cece/plugins/streamlines/Converter.hpp"

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
    IterationType getInitIterations() const noexcept
    {
        return m_initIterations;
    }


    /**
     * @brief Returns init iteration count.
     *
     * @return
     */
    IterationType getInnerIterations() const noexcept
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


#if defined(CECE_ENABLE_RENDER)

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
     * @brief If dynamic objects are used as obstacles.
     *
     * @return
     */
    bool isDynamicObjectsObstacles() const noexcept
    {
        return m_dynamicObjectsObstacles;
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
    void setInitIterations(IterationType iterations) noexcept
    {
        m_initIterations = iterations;
    }


    /**
     * @brief Set inner iteration count.
     *
     * @param iterations
     */
    void setInnerIterations(IterationType iterations) noexcept
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
     * @param flag
     */
    void init(AtomicBool& flag) override;


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
     * @param visualization
     * @param context Rendering context.
     */
    void draw(const simulator::Visualization& visualization, render::Context& context) override;

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


// Private Data Members
private:

    /// Units converter.
    Converter m_converter;

    /// Number of init iterations.
    IterationType m_initIterations = 0;

    /// Number of inner iterations.
    IterationType m_innerIterations = 1;

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
    /// Velocity magnitude scale.
    RealType m_debugMagnitudeScale = 2;

    /// Name of layer for flow dynamics type visualization.
    String m_visualizationLayerDynamicsType;

    /// Name of layer for velocity magnitude visualization.
    String m_visualizationLayerMagnitude;

    /// Name of layer for density visualization.
    String m_visualizationLayerDensity;

    /// Rendering grid with dynamics type.
    render::ObjectPtr<render::GridColor> m_drawableDynamicsType;

    /// Rendering grid with magnitude.
    render::ObjectPtr<render::GridColorColorMap> m_drawableMagnitude;

    /// Rendering grid with density.
    render::ObjectPtr<render::GridColorColorMap> m_drawableDensity;
#endif

#ifdef CECE_THREAD_SAFE
    /// Access mutex.
    mutable Mutex m_mutex;
#endif

    /// Used flow dynamics.
    UniquePtr<Dynamics> m_fluidDynamics;

    /// Used wall dynamics.
    UniquePtr<Dynamics> m_wallDynamics;

};

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */

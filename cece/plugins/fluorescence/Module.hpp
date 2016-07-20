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

// CeCe
#include "cece/config.hpp"
#include "cece/core/Real.hpp"
#include "cece/core/Grid.hpp"
#include "cece/module/Module.hpp"
#include "cece/render/Context.hpp"

#ifdef CECE_ENABLE_RENDER
#  include "cece/render/Object.hpp"
#  include "cece/render/State.hpp"
#  include "cece/render/Image.hpp"
#  include "cece/render/GridColorSmooth.hpp"
#endif

/* ************************************************************************ */

#ifdef CECE_ENABLE_RENDER
namespace cece { namespace render { class Context; } }
namespace cece { namespace simulator { class Visualization; } }
#endif

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace fluorescence {

/* ************************************************************************ */

/**
 * @brief Module for fluorescence visualization.
 */
class Module : public module::Module
{

// Public Structures
public:


    /**
     * @brief Fluorescence proteins.
     */
    template<typename T>
    struct Proteins
    {
        T rfp;
        T gfp;
        T yfp;
    };


    /**
     * @brief Grid value state.
     */
    struct Record
    {
        /// Number of proteins.
        Proteins<unsigned long> proteins;

        /// Number of cells.
        unsigned long count;
    };


// Public Types
public:

    /// Number per area.
    using DensityType = typename units::Inverse<units::Area>::type;

    /// Grid type.
    using GridType = Grid<Record>;


// Public Ctors & Dtors
public:


    using module::Module::Module;


// Public Accessors
public:


    /**
     * @brief Returns grid.
     *
     * @return
     */
    GridType& getGrid() noexcept
    {
        return m_grid;
    }


    /**
     * @brief Returns grid.
     *
     * @return
     */
    const GridType& getGrid() const noexcept
    {
        return m_grid;
    }


// Public Operations
public:


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
     * @brief Initialize module.
     */
    void init() override;


    /**
     * @brief Update module state.
     */
    void update() override;


#ifdef CECE_ENABLE_RENDER

    /**
     * @brief Render module.
     *
     * @param visualization Visualization context.
     * @param context       Rendering context.
     */
    void draw(const simulator::Visualization& visualization, render::Context& context) override;


    /**
     * @brief Store current state for drawing.
     * State should be stored in back state because the front state is
     * used for rendering.
     * Drawing state should contain data that can be modified during update()
     * call and are used for rendering.
     */
    void drawStoreState() override;


    /**
     * @brief Swap render state.
     * Calling this function should be guarded by mutex for all modules
     * to ensure all modules are in same render state.
     * Function should be fast because otherwise it will block rendering.
     */
    void drawSwapState() override;

#endif

// Private Structures
private:

#ifdef CECE_ENABLE_RENDER
    struct RenderState
    {
        units::ScaleVector scale;
        render::Image image;
    };
#endif

// Private Data Members
private:

    /// Fluorescence grid.
    GridType m_grid;

#ifdef CECE_ENABLE_RENDER
    /// Render grid
    render::ObjectPtr<render::GridColorSmooth> m_drawable;

    /// Render state.
    render::State<RenderState> m_drawableState;

    /// Layer names for proteins.
    Proteins<String> m_layerNames;

    /// GFP saturation.
    Proteins<DensityType> m_saturations;
#endif
};

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */

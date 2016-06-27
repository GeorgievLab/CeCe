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

/* ************************************************************************ */

#ifdef CECE_ENABLE_RENDER

/* ************************************************************************ */

// C++
#include <utility>

// CeCe
#include "cece/core/StringView.hpp"
#include "cece/core/DynamicArray.hpp"
#include "cece/render/Color.hpp"
#include "cece/simulator/VisualizationLayer.hpp"

/* ************************************************************************ */

namespace cece { namespace config { class Configuration; } }

/* ************************************************************************ */

namespace cece {
namespace simulator {

/* ************************************************************************ */

/**
 * @brief Class which store information about simulation visualization.
 */
class Visualization
{


// Public Accessors
public:


    /**
     * @brief Returns if visualization is enabled.
     *
     * @return
     */
    bool isEnabled() const noexcept
    {
        return m_enabled;
    }


    /**
     * @brief Returns if visualization layer is enabled.
     *
     * @param name Layer name.
     * @param def  In case the layer doesn't exists.
     *
     * @return If layer is enabled.
     *
     * @note In case layer with given name doesn't exists, true is returned.
     */
    bool isEnabled(StringView name, bool def = false) const noexcept;


    /**
     * @brief Returns background color.
     *
     * @return
     */
    const render::Color& getBackgroundColor() const noexcept
    {
        return m_backgroundColor;
    }


    /**
     * @brief Returns available layers.
     *
     * @return
     */
    DynamicArray<VisualizationLayer>& getLayers() noexcept
    {
        return m_layers;
    }


    /**
     * @brief Returns available layers.
     *
     * @return
     */
    const DynamicArray<VisualizationLayer>& getLayers() const noexcept
    {
        return m_layers;
    }


// Public Mutators
public:


    /**
     * @brief Enable or disable visualization.
     *
     * @param flag
     */
    void setEnabled(bool flag) noexcept
    {
        m_enabled = flag;
    }


    /**
     * @brief Set background color.
     *
     * @param color
     */
    void setBackgroundColor(render::Color color) noexcept
    {
        m_backgroundColor = std::move(color);
    }


// Public Operations
public:


    /**
     * @brief Configure visualization.
     *
     * @param config
     */
    void loadConfig(const config::Configuration& config);


    /**
     * @brief Store visualization configuration.
     *
     * @param config
     */
    void storeConfig(config::Configuration& config) const;


// Private Data Members
private:

    /// If is enabled.
    bool m_enabled = true;

    /// Background (clear) color.
    render::Color m_backgroundColor = render::colors::WHITE;

    /// Available visualization layers.
    DynamicArray<VisualizationLayer> m_layers;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */

#endif

/* ************************************************************************ */

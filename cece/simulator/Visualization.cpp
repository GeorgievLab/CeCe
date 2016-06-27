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

// Declaration
#include "cece/simulator/Visualization.hpp"

// C++
#include <algorithm>

// CeCe
#include "cece/config/Configuration.hpp"

/* ************************************************************************ */

#ifdef CECE_ENABLE_RENDER

/* ************************************************************************ */

namespace cece {
namespace simulator {

/* ************************************************************************ */

bool Visualization::isEnabled(StringView name, bool def) const noexcept
{
    if (name.getLength() == 0)
        return def;

    auto it = std::find_if(m_layers.begin(), m_layers.end(), [&](const VisualizationLayer& layer) {
        return layer.getName() == name;
    });

    if (it == m_layers.end())
        return def;

    return it->isEnabled();
}

/* ************************************************************************ */

void Visualization::loadConfig(const config::Configuration& config)
{
    setEnabled(config.get("enabled", isEnabled()));
    setBackgroundColor(config.get("background", getBackgroundColor()));

    m_layers.clear();

    // Foreach layers
    for (const auto& layer : config.getConfigurations("layer"))
    {
        m_layers.emplace_back(layer.get("name"), layer.get("key", String{}), layer.get("enabled", false));
    }
}

/* ************************************************************************ */

void Visualization::storeConfig(config::Configuration& config) const
{
    config.set("enabled", isEnabled());
    config.set("background", getBackgroundColor());

    for (const auto& layer : getLayers())
    {
        auto layerConfig = config.addConfiguration("layer");
        layerConfig.set("name", layer.getName());
        layerConfig.set("key", layer.getKey());
        layerConfig.set("enabled", layer.isEnabled());
    }
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

#endif

/* ************************************************************************ */

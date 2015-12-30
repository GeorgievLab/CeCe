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

#ifndef CECE_ENABLE_RENDER
#error Background plugin requires enabled rendering.
#endif

/* ************************************************************************ */

// CeCe
#include "cece/core/String.hpp"
#include "cece/core/Grid.hpp"
#include "cece/module/Module.hpp"
#include "cece/render/Object.hpp"
#include "cece/render/Color.hpp"
#include "cece/render/GridColor.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace background {

/* ************************************************************************ */

/**
 * @brief Module for rendering background image.
 */
class Module : public module::Module
{


// Public Operations
public:


    /**
     * @brief Load module configuration.
     *
     * @param simulation Current simulation.
     * @param config     Source configuration.
     */
    void loadConfig(simulator::Simulation& simulation, const config::Configuration& config) override;


    /**
     * @brief Render module.
     *
     * @param simulation Current simulation.
     * @param context    Rendering context.
     */
    void draw(const simulator::Simulation& simulation, render::Context& context) override;


// Private Data Members
private:

    /// Image data.
    DynamicArray<unsigned char> m_data;

    /// Image size.
    Vector<unsigned int> m_size = Zero;

    // Number of channels.
    int m_channels;

    /// Drawable object.
    render::ObjectPtr<render::GridColor> m_drawable;

};

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */

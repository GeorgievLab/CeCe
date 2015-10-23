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

// OpenCV
#include <opencv2/core/core.hpp>

// Simulator
#include "core/String.hpp"
#include "simulator/Module.hpp"

#if ENABLE_RENDER
#  include "render/Object.hpp"
#  include "render/GridColor.hpp"
#endif

/* ************************************************************************ */

namespace plugin {
namespace background {

/* ************************************************************************ */

/**
 * @brief Module for rendering background image.
 */
class Module : public simulator::Module
{


// Public Operations
public:


    /**
     * @brief Load module configuration.
     *
     * @param simulation Current simulation.
     * @param config     Source configuration.
     */
    void loadConfig(simulator::Simulation& simulation, const simulator::Configuration& config) override;


#if ENABLE_RENDER
    /**
     * @brief Render module.
     *
     * @param simulation Current simulation.
     * @param context    Rendering context.
     */
    void draw(const simulator::Simulation& simulation, render::Context& context) override;
#endif


// Private Data Members
private:

    /// Source image.
    cv::Mat m_img;

#if ENABLE_RENDER
    /// Drawable object.
    render::ObjectPtr<render::GridColor> m_drawable;
#endif

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */

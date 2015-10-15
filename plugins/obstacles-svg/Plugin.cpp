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

// Simulator
#include "core/DynamicArray.hpp"
#include "core/StringStream.hpp"
#include "simulator/Plugin.hpp"
#include "simulator/PluginApi.hpp"
#include "simulator/Simulation.hpp"
#include "simulator/Configuration.hpp"
#include "simulator/Obstacle.hpp"

// NanoSVG
#define NANOSVG_IMPLEMENTATION  // Expands implementation
#include "nanosvg/nanosvg.h"

/* ************************************************************************ */

using namespace simulator;

/* ************************************************************************ */

class ObstaclesSvgApi : public PluginApi
{
    void configure(Simulation& simulation, const Configuration& config) override
    {
        const auto filepath = config.buildFilePath(config.get("filename"));

        // SVG image
        NSVGimage* image = nsvgParseFromFile(filepath.c_str(), "px", 96);

        if (!image)
            throw RuntimeException("Cannot parse SVG");

        auto obstacle = simulation.createObject<simulator::Obstacle>();
        auto& shapes = obstacle->getMutableShapes();

        const auto size = Vector<float>(image->width, image->height);

        // Coordinate scale
        const auto scale = simulation.getWorldSize() / size;
        const auto offset = size * 0.5;

        // Foreach shapes
        for (NSVGshape* shape = image->shapes; shape != nullptr; shape = shape->next)
        {
            simulator::ShapeEdges oShape;

            // Foreach paths
            for (NSVGpath* path = shape->paths; path != nullptr; path = path->next)
            {
                for (int i = 0; i < path->npts - 1; i += 3)
                {
                    float* p = &path->pts[i * 2];

                    const auto pos = Vector<float>{p[0], p[1]};
                    oShape.edges.push_back((pos - offset) * scale);
                }
            }

            shapes.push_back(std::move(oShape));
        }

        obstacle->initShapes();
    }
};

/* ************************************************************************ */

DEFINE_PLUGIN(obstacles_svg, ObstaclesSvgApi)

/* ************************************************************************ */

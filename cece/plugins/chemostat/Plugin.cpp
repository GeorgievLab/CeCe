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

// CeCe
#include "cece/core/Units.hpp"
#include "cece/core/Vector.hpp"
#include "cece/core/DynamicArray.hpp"
#include "cece/plugin/definition.hpp"
#include "cece/plugin/Api.hpp"
#include "cece/config/Configuration.hpp"
#include "cece/simulator/Simulation.hpp"

/* ************************************************************************ */

using namespace cece;
using namespace cece::simulator;

/* ************************************************************************ */

class ChemostatApi : public plugin::Api
{

    /**
     * @brief Returns a list of required plugins.
     *
     * @return
     */
    DynamicArray<String> requiredPlugins() const override
    {
        return {"obstacle"};
    }


    void loadConfig(simulator::Simulation& simulation, const config::Configuration& config) override
    {
        constexpr auto SLOPE = units::um(5);

        const auto worldSizeHalf = simulation.getWorldSize() * 0.5;

        const auto pipeTop = config.get<units::Length>("pipe-top");
        const auto pipeRadius = config.get<units::Length>("pipe-radius");
        const auto size = config.get<Vector<units::Length>>("size");
        const auto visible = config.get("visible", false);

        // Upper part
        {
            auto obstacle = simulation.buildObject("obstacle.Polygon");
            auto& shapes = obstacle->getMutableShapes();
            shapes.resize(1);

            StaticArray<units::Length, 4> borders{{
                -worldSizeHalf.getX() - units::um(1), // Left
                 worldSizeHalf.getX() + units::um(1), // Right
                 worldSizeHalf.getY() + units::um(1), // Top
                 worldSizeHalf.getY() - pipeTop       // Bottom
            }};

            DynamicArray<PositionVector> vertices;
            // Top left
            vertices.push_back(PositionVector{borders[0], borders[2]});
            // Top right
            vertices.push_back(PositionVector{borders[1], borders[2]});
            // Bottom right
            vertices.push_back(PositionVector{borders[1], borders[3]});
            // Bottom left
            vertices.push_back(PositionVector{borders[0], borders[3]});

            shapes[0] = Shape::makeEdges(vertices);

            obstacle->initShapes();
#ifdef CECE_ENABLE_RENDER
            obstacle->setVisible(visible);
#endif
        }

        // Bottom part
        {
            const auto sizeHalf = size * 0.5;

            auto obstacle = simulation.buildObject("obstacle.Polygon");
            auto& shapes = obstacle->getMutableShapes();
            shapes.resize(1);

            StaticArray<units::Length, 8> borders{{
                -worldSizeHalf.getWidth() - units::um(1),                                // Left
                 worldSizeHalf.getWidth() + units::um(1),                                // Right
                -sizeHalf.getWidth(),                                                    // Middle Left
                 sizeHalf.getWidth(),                                                    // Middle Right
                 worldSizeHalf.getHeight() - pipeTop - 2 * pipeRadius,                   // Top
                -worldSizeHalf.getHeight() - units::um(1),                               // Bottom
                 worldSizeHalf.getHeight() - pipeTop - 2 * pipeRadius,                   // Middle Top
                 worldSizeHalf.getHeight() - pipeTop - 2 * pipeRadius - size.getHeight() // Middle Bottom
            }};

            DynamicArray<PositionVector> vertices;
            // Bottom left
            vertices.push_back(PositionVector{borders[0], borders[5]});
            // Top left
            vertices.push_back(PositionVector{borders[0], borders[6]});
            // Middle top left
            vertices.push_back(PositionVector{borders[2] - SLOPE, borders[6]});
            vertices.push_back(PositionVector{borders[2], borders[6] - SLOPE});
            // Middle bottom left
            vertices.push_back(PositionVector{borders[2], borders[7] + SLOPE});
            vertices.push_back(PositionVector{borders[2] + SLOPE, borders[7]});
            // Middle bottom right
            vertices.push_back(PositionVector{borders[3] - SLOPE, borders[7]});
            vertices.push_back(PositionVector{borders[3], borders[7] + SLOPE});
            // Middle top right
            vertices.push_back(PositionVector{borders[3], borders[6] - SLOPE});
            vertices.push_back(PositionVector{borders[3] + SLOPE, borders[6]});
            // Top right
            vertices.push_back(PositionVector{borders[1], borders[6]});
            // Bottom right
            vertices.push_back(PositionVector{borders[1], borders[5]});

            shapes[0] = Shape::makeEdges(vertices);

            obstacle->initShapes();
#ifdef CECE_ENABLE_RENDER
            obstacle->setVisible(visible);
#endif
        }
    }
};

/* ************************************************************************ */

CECE_DEFINE_PLUGIN(chemostat, ChemostatApi)

/* ************************************************************************ */

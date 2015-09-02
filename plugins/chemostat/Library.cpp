/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */

// Simulator
#include "core/Units.hpp"
#include "core/Vector.hpp"
#include "core/DynamicArray.hpp"
#include "simulator/Plugin.hpp"
#include "simulator/PluginApi.hpp"
#include "simulator/Simulation.hpp"
#include "simulator/Configuration.hpp"
#include "simulator/Obstacle.hpp"

/* ************************************************************************ */

using namespace simulator;

/* ************************************************************************ */

class ChemostatApi : public PluginApi
{
    void configure(Simulation& simulation, const Configuration& config) override
    {
        const auto worldSizeHalf = simulation.getWorldSize() * 0.5;

        const auto pipeTop = config.get<units::Length>("pipe-top");
        const auto pipeRadius = config.get<units::Length>("pipe-radius");
        const auto size = config.get<Vector<units::Length>>("size");

        // Upper part
        {
            Obstacle* obstacle = simulation.createObject<Obstacle>();
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
        }

        // Bottom part
        {
            const auto sizeHalf = size * 0.5;

            Obstacle* obstacle = simulation.createObject<Obstacle>();
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
            vertices.push_back(PositionVector{borders[2], borders[6]});
            // Middle bottom left
            vertices.push_back(PositionVector{borders[2], borders[7]});
            // Middle bottom right
            vertices.push_back(PositionVector{borders[3], borders[7]});
            // Middle top right
            vertices.push_back(PositionVector{borders[3], borders[6]});
            // Top right
            vertices.push_back(PositionVector{borders[1], borders[6]});
            // Bottom right
            vertices.push_back(PositionVector{borders[1], borders[5]});

            shapes[0] = Shape::makeEdges(vertices);

            obstacle->initShapes();
        }
    }
};

/* ************************************************************************ */

DEFINE_PLUGIN(chemostat, ChemostatApi)

/* ************************************************************************ */

/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* ************************************************************************ */

// Declaration
#include "Diffusion.hpp"

// C++
#include <random>

// Simulator
#include "simulator/ShapeToGrid.hpp"

/* ************************************************************************ */

namespace plugin {
namespace stochastic_reactions_diffusive {

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

// Random generators
std::random_device g_randomDevice;
std::mt19937 g_randomEngine(g_randomDevice());

/* ************************************************************************ */

}

/* ************************************************************************ */

DynamicArray<plugin::diffusion::Module::Coordinate> getCoordinates(
    const plugin::diffusion::Module::SizeType& gridSize,
    const SizeVector& worldSize,
    const plugin::cell::CellBase& cell)
{
    const auto start = worldSize * -0.5f;
    const auto step = worldSize / gridSize;

    // Get cell position
    const auto pos = cell.getPosition() - start;

    // Check if position is in range
    if (!pos.inRange(Zero, worldSize))
        return {};

    // Get position in grid
    const auto coord = plugin::diffusion::Module::Coordinate(pos / step);

    DynamicArray<plugin::diffusion::Module::Coordinate> coords;
    auto coordIt = std::inserter(coords, coords.end());

    // Get shapes
    const auto& shapes = cell.getShapes();

    // Get coordinates from shapes
    for (const auto& shape : shapes)
    {
        coordIt = mapShapeBorderToGrid(coordIt, shape, step, coord, gridSize);
    }

    // In case of single shape, all coordinates are unique
    if (shapes.size() > 1)
    {
        // Only unique coordinates
        std::sort(coords.begin(), coords.end());
        coords.erase(std::unique(coords.begin(), coords.end()), coords.end());
    }

    return coords;
}

/* ************************************************************************ */

void changeMolecules(
    plugin::diffusion::Module& diffusion,
    const DynamicArray<plugin::diffusion::Module::Coordinate>& coords,
    plugin::diffusion::Module::SignalId id, int change)
{
    // No coordinates
    if (coords.empty())
        return;

    // Select random coordinate
    std::uniform_int_distribution<> distribution(0, coords.size() - 1);
    const auto idx = distribution(g_randomEngine);

    // Change molecules
    diffusion.getSignal(id, coords[idx]) += change;
}

/* ************************************************************************ */

unsigned int getAmountOfMolecules(
    plugin::diffusion::Module& diffusion,
    const DynamicArray<plugin::diffusion::Module::Coordinate>& coords,
    plugin::diffusion::Module::SignalId id
)
{
    plugin::diffusion::Module::Signal amount = 0u;

    // Foreach all coordinates and calculate amount of molecules
    for (const auto& coord : coords)
        amount += diffusion.getSignal(id, coord);

    return amount;
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */


// Declaration
#include "ReactionDiffusive.hpp"

// C++
#include <random>

// Simulator
#include "core/String.hpp"
#include "core/Vector.hpp"
#include "simulator/ShapeToGrid.hpp"

// Plugins
#include "plugins/cell/CellBase.hpp"
#include "plugins/diffusion/Module.hpp"

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

// Random generators
std::random_device g_randomDevice;
std::mt19937 g_randomEngine(g_randomDevice());

/* ************************************************************************ */

/**
 * @brief Returns coordinates in diffusion grid for given cell object.
 *
 * @param gridSize  Size of diffusion grid.
 * @param worldSize Simulation world size.
 * @param cell      Cell object.
 */
DynamicArray<plugin::diffusion::Module::Coordinate> getCoordinates(
    const plugin::diffusion::Module::SizeType& gridSize,
    const SizeVector& worldSize,
    plugin::cell::CellBase& cell)
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

    // Get coordinates
    for (const auto& shape : cell.getShapes())
    {
        coordIt = mapShapeToGrid(coordIt, shape, step, coord, cell.getRotation(), gridSize);
    }

    // Only unique coordinates
    std::sort(coords.begin(), coords.end());
    coords.erase(std::unique(coords.begin(), coords.end()), coords.end());

    return coords;
}

/* ************************************************************************ */

/**
 * @brief Increase amount of molecules outside of cell.
 *
 * @param diffusion Diffusion module. Obtained by <code>simulation.useModule<plugin::diffusion::Module>("diffusion");</code>
 * @param worldSize Simulation world size. <code>simulation.getWorldSize();</code>
 * @param cell      Cell object.
 * @param name      Molecule name.
 * @param amount    Number of molecules to add.
 */
void addMolecules(plugin::diffusion::Module& diffusion,
    const SizeVector& worldSize, plugin::cell::CellBase& cell,
    const String& name, unsigned int amount = 1)
{
    // Get coordinates
    const auto coords = getCoordinates(diffusion.getGridSize(), worldSize, cell);

    // Select random coordinate
    std::uniform_int_distribution<> distribution(0, coords.size());
    const auto idx = distribution(g_randomEngine);

    // Add molecules
    diffusion.getSignal(name, coords[idx]) += amount;
}

/* ************************************************************************ */

/**
 * @brief Decrease amount of molecules outside of cell.
 *
 * @param diffusion Diffusion module. Obtained by <code>simulation.useModule<plugin::diffusion::Module>("diffusion");</code>
 * @param worldSize Simulation world size. <code>simulation.getWorldSize();</code>
 * @param cell      Cell object.
 * @param name      Molecule name.
 * @param amount    Number of molecules to add.
 */
void removeMolecules(plugin::diffusion::Module& diffusion,
    const SizeVector& worldSize, plugin::cell::CellBase& cell,
    const String& name, unsigned int amount = 1)
{
    // Get coordinates
    const auto coords = getCoordinates(diffusion.getGridSize(), worldSize, cell);

    // Select random coordinate
    std::uniform_int_distribution<> distribution(0, coords.size());
    const auto idx = distribution(g_randomEngine);

    // Add molecules
    diffusion.getSignal(name, coords[idx]) -= amount;
}

/* ************************************************************************ */

/**
 * @brief Receive amount of molecules outside of cell.
 *
 * @param diffusion Diffusion module. Obtained by <code>simulation.useModule<plugin::diffusion::Module>("diffusion");</code>
 * @param worldSize Simulation world size. <code>simulation.getWorldSize();</code>
 * @param cell      Cell object.
 * @param name      Molecule name.
 */
unsigned int getAmountOfMolecules(plugin::diffusion::Module& diffusion,
    const SizeVector& worldSize, plugin::cell::CellBase& cell,
    const String& name)
{
    unsigned int amount = 0u;

    // Get coordinates
    const auto coords = getCoordinates(diffusion.getGridSize(), worldSize, cell);

    // Get signal id (improve performance)
    const auto id = diffusion.getSignalId(name);

    // Foreach all coordinates and calculate amount of molecules
    for (const auto& coord : coords)
        amount += diffusion.getSignal(id, coord);

    return amount;
}

/* ************************************************************************ */

}

/* ************************************************************************ */

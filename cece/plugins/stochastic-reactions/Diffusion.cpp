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

// Declaration
#include "Diffusion.hpp"

// C++
#include <random>

// CeCe
#include "cece/core/ShapeToGrid.hpp"
#include "cece/simulator/Simulation.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace stochastic_reactions {

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

// Random generators
std::random_device g_randomDevice;
std::mt19937 g_randomEngine(g_randomDevice());

/* ************************************************************************ */

/**
 * @brief Avogadro constant.
 */
constexpr units::Inverse<units::AmountOfSubstance>::type NA = 6.022140857e23 / units::mol(1);

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
        coordIt = mapShapeToGrid(coordIt, shape, step, coord, cell.getRotation(), gridSize);
    }

    // In case of single shape, all coordinates are unique
    if (shapes.size() > 1)
    {
        // Only unique coordinates
        std::sort(coords.begin(), coords.end());
        coords.erase(std::unique(coords.begin(), coords.end()), coords.end());
    }

    // Cell must be in at least one grid cell for diffusion
    if (coords.empty())
        coords.push_back(coord);

    return coords;
}

/* ************************************************************************ */

void changeMolecules(
    const simulator::Simulation& simulation,
    plugin::diffusion::Module& diffusion,
    const DynamicArray<plugin::diffusion::Module::Coordinate>& coords,
    plugin::diffusion::Module::SignalId id,
    int molecules)
{
    // No coordinates
    if (coords.empty())
        return;

    // Calculate on cell volume
    const auto size = simulation.getWorldSize() / diffusion.getGridSize();
    const units::Volume volume = size.getX() * size.getY() * units::Length(1);

    // Select random coordinate
    std::uniform_int_distribution<> distribution(0, coords.size() - 1);
    const auto idx = distribution(g_randomEngine);

    // Change molecules
    auto& concentration = diffusion.getSignal(id, coords[idx]);
    concentration += molecules / (NA * volume);

    concentration = std::max<plugin::diffusion::Module::SignalConcentration>(concentration, Zero);
    assert(concentration >= Zero);
}

/* ************************************************************************ */

unsigned int getAmountOfMolecules(
    const simulator::Simulation& simulation,
    plugin::diffusion::Module& diffusion,
    const DynamicArray<plugin::diffusion::Module::Coordinate>& coords,
    plugin::diffusion::Module::SignalId id
)
{
    if (coords.empty())
        return 0u;

    // Calculate on cell volume
    const auto size = simulation.getWorldSize() / diffusion.getGridSize();
    const units::Volume volume = size.getX() * size.getY() * units::Length(1);

    // Get concentration
    const auto concentration = getMolarConcentration(diffusion, coords, id);

    return concentration * NA * volume;
}

/* ************************************************************************ */

units::MolarConcentration getMolarConcentration(
    plugin::diffusion::Module& diffusion,
    const DynamicArray<plugin::diffusion::Module::Coordinate>& coords,
    plugin::diffusion::Module::SignalId id
)
{
    if (coords.empty())
        return Zero;

    plugin::diffusion::Module::SignalConcentration concentration = Zero;

    // Foreach all coordinates and sum concentration
    for (const auto& coord : coords)
        concentration += diffusion.getSignal(id, coord);

    // Negative concentration is invalid
    assert(concentration >= Zero);

    return concentration / coords.size();
}

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */

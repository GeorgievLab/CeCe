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

// CeCe
#include "cece/core/String.hpp"
#include "cece/core/Vector.hpp"

// Plugins
#include "cece/plugins/cell/CellBase.hpp"
#include "cece/plugins/diffusion/Module.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace stochastic_reactions{

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
    const plugin::cell::CellBase& cell);

/* ************************************************************************ */

/**
 * @brief Change amount of molecules outside of cell.
 *
 * @param simulation Current simulation.
 * @param diffusion  Diffusion module.
 * @param coords     Coordinates.
 * @param id         Molecule signal identifier.
 * @param change     Number of molecules to change. Positive to add and negative to remove.
 */
void changeMolecules(
    const simulator::Simulation& simulation,
    plugin::diffusion::Module& diffusion,
    const DynamicArray<plugin::diffusion::Module::Coordinate>& coords,
    plugin::diffusion::Module::SignalId id,
    int change
);

/* ************************************************************************ */

/**
 * @brief Increase amount of molecules outside of cell.
 *
 * @param simulation Current simulation.
 * @param diffusion  Diffusion module.
 * @param coords     Coordinates.
 * @param id         Molecule signal identifier.
 * @param amount     Number of molecules to add.
 */
inline void addMolecules(
    const simulator::Simulation& simulation,
    plugin::diffusion::Module& diffusion,
    const DynamicArray<plugin::diffusion::Module::Coordinate>& coords,
    plugin::diffusion::Module::SignalId id,
    unsigned int amount = 1)
{
    changeMolecules(simulation, diffusion, coords, id, amount);
}

/* ************************************************************************ */

/**
 * @brief Decrease amount of molecules outside of cell.
 *
 * @param simulation Current simulation.
 * @param diffusion  Diffusion module.
 * @param coords     Coordinates.
 * @param id         Molecule signal identifier.
 * @param amount     Number of molecules to remove.
 */
inline void removeMolecules(
    const simulator::Simulation& simulation,
    plugin::diffusion::Module& diffusion,
    const DynamicArray<plugin::diffusion::Module::Coordinate>& coords,
    plugin::diffusion::Module::SignalId id,
    unsigned int amount = 1)
{
    changeMolecules(simulation, diffusion, coords, id, -amount);
}

/* ************************************************************************ */

/**
 * @brief Receive amount of molecules outside of cell.
 *
 * @param simulation Current simulation.
 * @param diffusion  Diffusion module.
 * @param coords     Coordinates.
 * @param id         Molecule signal identifier.
 */
unsigned int getAmountOfMolecules(
    const simulator::Simulation& simulation,
    plugin::diffusion::Module& diffusion,
    const DynamicArray<plugin::diffusion::Module::Coordinate>& coords,
    plugin::diffusion::Module::SignalId id
);

/* ************************************************************************ */

/**
 * @brief Receive molecule molar concentration outside of cell.
 *
 * @param diffusion  Diffusion module.
 * @param coords     Coordinates.
 * @param id         Molecule signal identifier.
 */
units::MolarConcentration getMolarConcentration(
    plugin::diffusion::Module& diffusion,
    const DynamicArray<plugin::diffusion::Module::Coordinate>& coords,
    plugin::diffusion::Module::SignalId id
);

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */

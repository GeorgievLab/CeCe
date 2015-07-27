/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* ************************************************************************ */

// Simulator
#include "core/String.hpp"
#include "core/Vector.hpp"

// Plugins
#include "plugins/cell/CellBase.hpp"
#include "plugins/diffusion/Module.hpp"

/* ************************************************************************ */

namespace plugin {
namespace stochastic_reactions_diffusive {

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
 * @param diffusion Diffusion module.
 * @param coords    Coordinates.
 * @param id        Molecule signal identifier.
 * @param change    Number of molecules to change. Positive to add and negative to remove.
 */
void changeMolecules(
    plugin::diffusion::Module& diffusion,
    const DynamicArray<plugin::diffusion::Module::Coordinate>& coords,
    plugin::diffusion::Module::SignalId id, int change
);

/* ************************************************************************ */

/**
 * @brief Increase amount of molecules outside of cell.
 *
 * @param diffusion Diffusion module.
 * @param coords    Coordinates.
 * @param id        Molecule signal identifier.
 * @param amount    Number of molecules to add.
 */
inline void addMolecules(
    plugin::diffusion::Module& diffusion,
    const DynamicArray<plugin::diffusion::Module::Coordinate>& coords,
    plugin::diffusion::Module::SignalId id, unsigned int amount = 1
)
{
    changeMolecules(diffusion, coords, id, amount);
}

/* ************************************************************************ */

/**
 * @brief Decrease amount of molecules outside of cell.
 *
 * @param diffusion Diffusion module.
 * @param coords    Coordinates.
 * @param id        Molecule signal identifier.
 * @param amount    Number of molecules to remove.
 */
inline void removeMolecules(
    plugin::diffusion::Module& diffusion,
    const DynamicArray<plugin::diffusion::Module::Coordinate>& coords,
    plugin::diffusion::Module::SignalId id, unsigned int amount = 1
)
{
    changeMolecules(diffusion, coords, id, -amount);
}

/* ************************************************************************ */

/**
 * @brief Receive amount of molecules outside of cell.
 *
 * @param diffusion Diffusion module.
 * @param coords    Coordinates.
 * @param id        Molecule signal identifier.
 */
unsigned int getAmountOfMolecules(
    plugin::diffusion::Module& diffusion,
    const DynamicArray<plugin::diffusion::Module::Coordinate>& coords,
    plugin::diffusion::Module::SignalId id
);

/* ************************************************************************ */

}
}

/* ************************************************************************ */

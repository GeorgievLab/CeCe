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
namespace stochastic_reactions {

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
    plugin::cell::CellBase& cell);

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
    const String& name, unsigned int amount = 1);

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
    const String& name, unsigned int amount = 1);

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
    const String& name);

/* ************************************************************************ */

}
}

/* ************************************************************************ */

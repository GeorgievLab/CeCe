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
#include "LoaderManager.hpp"

// Simulator
#include "simulator/Simulation.hpp"
#include "simulator/SimulationLoader.hpp"

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

Map<String, UniquePtr<SimulationLoader>> LoaderManager::s_loaders;

/* ************************************************************************ */

DynamicArray<String> LoaderManager::getNames() noexcept
{
    DynamicArray<String> names;
    names.reserve(s_loaders.size());

    for (const auto& p : s_loaders)
        names.push_back(p.first);

    return names;
}

/* ************************************************************************ */

UniquePtr<Simulation> LoaderManager::create(const FilePath& filepath)
{
    // File extension
    auto ext = filepath.extension().string().substr(1);

    // Get loader by extension
    auto loader = get(ext);

    if (!loader)
        throw RuntimeException("Unable to load file with extension: " + ext);

    // Create simulation
    return loader->fromFile(filepath);
}

/* ************************************************************************ */

}

/* ************************************************************************ */

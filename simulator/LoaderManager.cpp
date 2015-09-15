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

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
#include "cece/simulator/PluginContext.hpp"

// CeCe
#include "cece/loader/Loader.hpp"
#include "cece/simulator/Simulation.hpp"

/* ************************************************************************ */

namespace cece {
namespace simulator {

/* ************************************************************************ */

UniquePtr<Simulation> PluginContext::createSimulation(const FilePath& filepath)
{
    // File extension
    auto ext = filepath.extension().string().substr(1);

    // Find loader by extension
    auto loader = getLoaderFactoryManager().create(ext);

    if (!loader)
        throw RuntimeException("Unable to load file with extension: '" + ext + "'");

    // Create simulation
    return loader->fromFile(*this, filepath);
}

/* ************************************************************************ */

UniquePtr<Module> PluginContext::createModule(StringView typeName, Simulation& simulation)
{
    return getModuleFactoryManager().createModule(typeName, simulation);
}

/* ************************************************************************ */

UniquePtr<Object> PluginContext::createObject(StringView typeName, Simulation& simulation, Object::Type type)
{
    return getObjectFactoryManager().createObject(typeName, simulation, type);
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

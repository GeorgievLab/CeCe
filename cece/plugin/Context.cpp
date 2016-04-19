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
#include "cece/plugin/Context.hpp"

// CeCe
#include "cece/loader/Loader.hpp"
#include "cece/simulator/Simulation.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {

/* ************************************************************************ */

UniquePtr<simulator::Simulation> Context::createSimulation(const FilePath& filepath, ViewPtr<const Parameters> parameters)
{
    // File extension
    auto ext = filepath.extension().string().substr(1);

    // Find loader by extension
    auto loader = getLoaderFactoryManager().createLoader(ext);

    if (!loader)
        throw RuntimeException("Unable to load file with extension: '" + ext + "'");

    // Create a simulation
    return loader->fromFile(*this, filepath, parameters);
}

/* ************************************************************************ */

UniquePtr<simulator::Simulation> Context::createSimulation(StringView type, StringView source)
{
    // Create a loader from type
    auto loader = getLoaderFactoryManager().createLoader(type);

    if (!loader)
        throw RuntimeException("Unable to find loader '" + String(type) + "'");

    // Create a simulation
    return loader->fromSource(*this, String(source));
}

/* ************************************************************************ */

UniquePtr<init::Initializer> Context::createInitializer(StringView typeName)
{
    return getInitFactoryManager().createInitializer(typeName);
}

/* ************************************************************************ */

UniquePtr<module::Module> Context::createModule(StringView typeName, simulator::Simulation& simulation)
{
    return getModuleFactoryManager().createModule(typeName, simulation);
}

/* ************************************************************************ */

UniquePtr<object::Object> Context::createObject(StringView typeName, simulator::Simulation& simulation, object::Object::Type type)
{
    return getObjectFactoryManager().createObject(typeName, simulation, type);
}

/* ************************************************************************ */

UniquePtr<program::Program> Context::createProgram(StringView typeName)
{
    return getProgramFactoryManager().createProgram(typeName);
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

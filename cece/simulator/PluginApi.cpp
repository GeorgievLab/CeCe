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
#include "cece/simulator/PluginApi.hpp"

// C++
#include <fstream>
#include <sstream>

// CeCe
#include "cece/simulator/Simulation.hpp"

/* ************************************************************************ */

namespace cece {
namespace simulator {

/* ************************************************************************ */

UniquePtr<Simulation> PluginApi::fromFile(const FilePath& filename) const
{
    std::ifstream file(filename.string(), std::ios::in);
    return fromStream(file, filename);
}

/* ************************************************************************ */

UniquePtr<Simulation> PluginApi::fromSource(const String& source, const FilePath& filename) const
{
    std::istringstream is(source, std::ios::in);
    return fromStream(is, filename);
}

/* ************************************************************************ */

void PluginApi::toFile(const Simulation& simulation, const FilePath& filename) const
{
    // Write code into file
    std::ofstream file(filename.string(), std::ios::out);
    toStream(file, simulation, filename);
}

/* ************************************************************************ */

String PluginApi::toSource(const Simulation& simulation, const FilePath& filename) const
{
    std::ostringstream os(std::ios::out);
    toStream(os, simulation, filename);
    return os.str();
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

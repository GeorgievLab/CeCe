/* ************************************************************************ */
/* Georgiev Lab (c) 2015-2016                                               */
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
#include "cece/loader/Loader.hpp"

// C++
#include <fstream>
#include <sstream>

// CeCe
#include "cece/core/Parameters.hpp"
#include "cece/simulator/Simulation.hpp"

/* ************************************************************************ */

namespace cece {
namespace loader {

/* ************************************************************************ */

UniquePtr<simulator::Simulation> Loader::fromFile(plugin::Context& context,
    const FilePath& filename, ViewPtr<const Parameters> parameters) const
{
    std::ifstream file(filename.string(), std::ios::in);
    return fromStream(context, file, filename, parameters);
}

/* ************************************************************************ */

UniquePtr<simulator::Simulation> Loader::fromSource(plugin::Context& context,
    const String& source, const FilePath& filename, ViewPtr<const Parameters> parameters) const
{
    std::istringstream is(source, std::ios::in);
    return fromStream(context, is, filename, parameters);
}

/* ************************************************************************ */

void Loader::toFile(const simulator::Simulation& simulation, const FilePath& filename) const
{
    // Write code into file
    std::ofstream file(filename.string(), std::ios::out);
    toStream(file, simulation, filename);
}

/* ************************************************************************ */

String Loader::toSource(const simulator::Simulation& simulation, const FilePath& filename) const
{
    std::ostringstream os(std::ios::out);
    toStream(os, simulation, filename);
    return os.str();
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

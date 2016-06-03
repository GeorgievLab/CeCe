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
#include "cece/plugins/cell/StoreMolecules.hpp"

// CeCe
#include "cece/core/Exception.hpp"
#include "cece/core/CsvFile.hpp"
#include "cece/object/Object.hpp"
#include "cece/simulator/Simulation.hpp"

// Plugin
#include "cece/plugins/cell/CellBase.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace cell {

/* ************************************************************************ */

// TODO: find a better solution
static bool g_header = false;
CsvFile g_file("molecules.csv");

/* ************************************************************************ */

StoreMolecules::~StoreMolecules() = default;

/* ************************************************************************ */

UniquePtr<program::Program> StoreMolecules::clone() const
{
    return makeUnique<StoreMolecules>(*this);
}

/* ************************************************************************ */

void StoreMolecules::call(simulator::Simulation& simulation, object::Object& object, units::Time dt)
{
    // Cast to cell
    auto& cell = object.castThrow<CellBase>("Cell object required");

    if (!g_header)
    {
        g_file.writeHeader("iteration", "totalTime", "oid", "molecule", "amount");
        g_header = true;
    }

    // Store molecules
    for (const auto& molecule : cell.getMolecules())
    {
        g_file.writeRecord(
            simulation.getIteration(),
            simulation.getTotalTime().value(),
            object.getId(),
            molecule.first,
            molecule.second
        );
    }
}

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */

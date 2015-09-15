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

/* ************************************************************************ */

// Simulator
#include "core/UniquePtr.hpp"
#include "core/InStream.hpp"
#include "core/FilePath.hpp"
#include "simulator/SimulationLoader.hpp"

/* ************************************************************************ */

namespace simulator { class Simulation; }

/* ************************************************************************ */

namespace loader {
namespace reactions {

/* ************************************************************************ */

/**
 * @brief Reactions loader.
 */
class SimulationLoader : public simulator::SimulationLoader
{


// Protected Operations
protected:


    /**
     * @brief Read simulation from input stream.
     *
     * @param is       Source stream.
     * @param filename Source file name.
     *
     * @return Created simulation.
     */
    UniquePtr<simulator::Simulation> fromStream(InStream& is,
        const FilePath& filename = "<stream>") const override;


    /**
     * @brief Write simulation into output stream.
     *
     * @param os         Output stream.
     * @param simulation Source simulation.
     *
     * @return Source code.
     */
    void toStream(OutStream& os, const simulator::Simulation& simulation,
        const FilePath& filename = "<stream>") const override;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */

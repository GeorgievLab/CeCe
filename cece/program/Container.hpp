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

#pragma once

/* ************************************************************************ */

// CeCe
#include "cece/core/Units.hpp"
#include "cece/core/PtrContainer.hpp"

/* ************************************************************************ */

namespace cece { namespace object { class Object; } }
namespace cece { namespace simulator { class Simulation; } }

/* ************************************************************************ */

namespace cece {
namespace program {

/* ************************************************************************ */

class Program;

/* ************************************************************************ */

/**
 * @brief Container for programs.
 */
class Container : public PtrContainer<Program>
{

// Public Operations
public:


    /**
     * @brief Call all programs.
     *
     * @param simulation Simulation object.
     * @param object     Object.
     * @param dt         Simulation time step.
     */
    void call(simulator::Simulation& simulation, object::Object& object, units::Time dt);


    /**
     * @brief Clone container.
     *
     * @return
     */
    Container clone() const;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */

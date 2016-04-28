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

// CeCe
#include "cece/export.hpp"
#include "cece/core/UniquePtr.hpp"
#include "cece/core/Units.hpp"

/* ************************************************************************ */

namespace cece { namespace object { class Object; } }
namespace cece { namespace config { class Configuration; } }
namespace cece { namespace simulator { class Simulation; } }

/* ************************************************************************ */

namespace cece {
namespace program {

/* ************************************************************************ */

/**
 * @brief Program that can be executed by objects.
 *
 * Programs are allowed to store information bound to specific object.
 */
class CECE_EXPORT Program
{

// Public Ctors & Dtors
public:


    /**
     * @brief Destructor.
     */
    virtual ~Program()
    {
        // Nothing to do
    }


// Public Operations
public:


    /**
     * @brief Clone program.
     *
     * @return
     */
    virtual UniquePtr<Program> clone() const = 0;


    /**
     * @brief Load program configuration.
     *
     * @param simulation Current simulation.
     * @param config     Source configuration.
     */
    virtual void loadConfig(simulator::Simulation& simulation, const config::Configuration& config)
    {
        // Nothing to do
    }


    /**
     * @brief Store program configuration.
     *
     * @param simulation Current simulation.
     * @param config     Output configuration.
     */
    virtual void storeConfig(const simulator::Simulation& simulation, config::Configuration& config) const
    {
        // Nothing to do
    }


    /**
     * @brief Allow to initialize program when is bound to specific object.
     *
     * @param simulation Simulation object.
     * @param object     Object.
     */
    virtual void init(simulator::Simulation& simulation, object::Object& object)
    {
        // Nothing to do
    }


    /**
     * @brief Call program for given object.
     *
     * @param simulation Simulation object.
     * @param object     Object.
     * @param dt         Simulation time step.
     */
    virtual void call(simulator::Simulation& simulation, object::Object& object, units::Time dt) = 0;


    /**
     * @brief Called when the program is unbound from object (or object is being deleted).
     *
     * @param simulation Simulation object.
     * @param object     Object.
     */
    virtual void terminate(simulator::Simulation& simulation, object::Object& object)
    {
        // Nothing to do
    }

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */

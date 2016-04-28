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

/* ************************************************************************ */

namespace cece { namespace simulator { class Simulation; } }
namespace cece { namespace config { class Configuration; } }

/* ************************************************************************ */

namespace cece {
namespace init {

/* ************************************************************************ */

/**
 * @brief Simulation initialization program.
 */
class CECE_EXPORT Initializer
{

// Public Ctors & Dtors
public:


    /**
     * @brief Destructor.
     */
    virtual ~Initializer()
    {
        // Nothing to do
    }


// Public Operations
public:


    /**
     * @brief Clone initializer.
     *
     * @return
     */
    virtual UniquePtr<Initializer> clone() const = 0;


    /**
     * @brief Load initializer configuration.
     *
     * @param simulation Current simulation.
     * @param config     Source configuration.
     */
    virtual void loadConfig(simulator::Simulation& simulation, const config::Configuration& config)
    {
        // Nothing to do
    }


    /**
     * @brief Store initializer configuration.
     *
     * @param simulation Current simulation.
     * @param config     Output configuration.
     */
    virtual void storeConfig(const simulator::Simulation& simulation, config::Configuration& config) const
    {
        // Nothing to do
    }


    /**
     * @brief Invoke initializer.
     *
     * @param simulation Simulation.
     */
    virtual void call(simulator::Simulation& simulation) = 0;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */

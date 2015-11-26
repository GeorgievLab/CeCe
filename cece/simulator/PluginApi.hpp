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
#include "cece/core/UniquePtr.hpp"
#include "cece/core/String.hpp"
#include "cece/simulator/Program.hpp"
#include "cece/simulator/Simulation.hpp"

/* ************************************************************************ */

namespace cece {
namespace simulator {

/* ************************************************************************ */

class Configuration;

/* ************************************************************************ */

/**
 * @brief Library API type.
 */
class PluginApi
{

// Public Ctors & Dtors
public:


    /**
     * @brief Destructor.
     */
    virtual ~PluginApi();


// Public Operations
public:


    /**
     * @brief On plugin load.
     */
    virtual void onLoad()
    {
        // Nothing to do
    }


    /**
     * @brief On plugin unload.
     */
    virtual void onUnload()
    {
        // Nothing to do
    }


    /**
     * @brief Init simulation.
     *
     * @param simulation Simulation.
     */
    virtual void initSimulation(Simulation& simulation)
    {
        // Nothing to do
    }


    /**
     * @brief Finalize simulation.
     *
     * @param simulation Simulation.
     */
    virtual void finalizeSimulation(Simulation& simulation)
    {
        // Nothing to do
    }


    /**
     * @brief Configure plugin.
     *
     * @param simulation Current simulation.
     * @param config     Plugin configuration.
     */
    virtual void configure(Simulation& simulation, const Configuration& config)
    {
        // Nothing to do
    }


    /**
     * @brief Create initializer.
     *
     * @param simulation Simulation for that module is created.
     * @param code       Program code.
     *
     * @return Created initializer.
     */
    virtual Simulation::Initializer createInitializer(Simulation& simulation, String code)
    {
        return {};
    }


    /**
     * @brief Create program from current library.
     *
     * @param simulation Simulation for that module is created.
     * @param name       Program name.
     * @param code       Optional program code.
     *
     * @return Created program.
     */
    virtual Program createProgram(Simulation& simulation, const String& name, String code = {})
    {
        return {};
    }

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */

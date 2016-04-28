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
#include "cece/export.hpp"
#include "cece/core/UniquePtr.hpp"
#include "cece/core/String.hpp"
#include "cece/core/DynamicArray.hpp"

/* ************************************************************************ */

namespace cece { namespace simulator { class Simulation; } }
namespace cece { namespace config { class Configuration; } }

/* ************************************************************************ */

namespace cece {
namespace plugin {

/* ************************************************************************ */

class Context;

/* ************************************************************************ */

/**
 * @brief Plugin API type. Custom plugins implements this class' functions
 * to provide additional functionality to the simulator.
 */
class CECE_EXPORT Api
{

// Public Ctors & Dtors
public:


    /**
     * @brief Destructor.
     */
    virtual ~Api()
    {
        // Nothing to do
    }


// Public Operations
public:


    /**
     * @brief Returns a list of required plugins.
     *
     * @return
     */
    virtual DynamicArray<String> requiredPlugins() const
    {
        return {};
    }


    /**
     * @brief On plugin load.
     *
     * @param context Plugin context.
     */
    virtual void onLoad(Context& context)
    {
        // Nothing to do
    }


    /**
     * @brief On plugin unload.
     *
     * @param context Plugin context.
     */
    virtual void onUnload(Context& context)
    {
        // Nothing to do
    }


    /**
     * @brief Init simulation.
     *
     * @param simulation Simulation.
     */
    virtual void initSimulation(simulator::Simulation& simulation)
    {
        // Nothing to do
    }


    /**
     * @brief Finalize simulation.
     *
     * @param simulation Simulation.
     */
    virtual void finalizeSimulation(simulator::Simulation& simulation)
    {
        // Nothing to do
    }


    /**
     * @brief Load plugin configuration.
     *
     * @param simulation Current simulation.
     * @param config     Plugin configuration.
     */
    virtual void loadConfig(simulator::Simulation& simulation, const config::Configuration& config)
    {
        // Nothing to do
    }


    /**
     * @brief Store plugin configuration.
     *
     * @param simulation Current simulation.
     * @param config     Plugin configuration.
     */
    virtual void storeConfig(const simulator::Simulation& simulation, config::Configuration& config) const
    {
        // Nothing to do
    }


};

/* ************************************************************************ */

}
}

/* ************************************************************************ */

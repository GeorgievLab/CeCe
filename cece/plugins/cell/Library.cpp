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

// CeCe
#include "cece/simulator/Simulation.hpp"
#include "cece/simulator/Plugin.hpp"
#include "cece/simulator/PluginApi.hpp"
#include "cece/simulator/PluginManager.hpp"
#include "cece/simulator/ObjectFactoryManager.hpp"

// Plugin
#include "Yeast.hpp"
#include "Cell.hpp"
#include "StoreMolecules.hpp"

/* ************************************************************************ */

using namespace cece;
using namespace cece::simulator;
using namespace cece::plugin::cell;

/* ************************************************************************ */

/**
 * @brief Cell plugin API.
 */
class CellApi : public PluginApi
{

    /**
     * @brief On plugin load.
     *
     * @param manager Plugin manager.
     */
    void onLoad(PluginManager& manager) override
    {
        manager.getObjectFactoryManager().createForObject<Cell>("cell.Cell");
        manager.getObjectFactoryManager().createForObject<Yeast>("cell.Yeast");
    }


    /**
     * @brief On plugin unload.
     *
     * @param manager Plugin manager.
     */
    void onUnload(PluginManager& manager) override
    {
        manager.getObjectFactoryManager().remove("cell.Cell");
        manager.getObjectFactoryManager().remove("cell.Yeast");
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
    Program createProgram(Simulation& simulation, const String& name, String code = {}) override
    {
        if (name == "store-molecules")
            return StoreMolecules{};

        return {};
    }

};

/* ************************************************************************ */

DEFINE_PLUGIN(cell, CellApi)

/* ************************************************************************ */

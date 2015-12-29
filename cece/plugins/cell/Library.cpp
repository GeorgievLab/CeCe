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
#include "cece/simulator/PluginContext.hpp"
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
     * @param context Plugin context.
     */
    void onLoad(PluginContext& context) override
    {
        context.registerObject<Cell>("cell.Cell");
        context.registerObject<Yeast>("cell.Yeast");
        context.registerProgram<StoreMolecules>("cell.store-molecules");
    }


    /**
     * @brief On plugin unload.
     *
     * @param context Plugin context.
     */
    void onUnload(PluginContext& context) override
    {
        context.unregisterProgram("cell.store-molecules");
        context.unregisterObject("cell.Cell");
        context.unregisterObject("cell.Yeast");
    }

};

/* ************************************************************************ */

DEFINE_PLUGIN(cell, CellApi)

/* ************************************************************************ */

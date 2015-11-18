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
     * @brief Create module from current library.
     *
     * @param simulation Simulation for that module is created.
     * @param name       Module name.
     *
     * @return Created module.
     */
    UniquePtr<Module> createModule(Simulation& simulation, const String& name) noexcept override
    {
        return nullptr;
    }


    /**
     * @brief Create object from current library.
     *
     * @param simulation Simulation for that module is created.
     * @param name       Object name.
     * @param type       Object type.
     *
     * @return Created object.
     */
    UniquePtr<Object> createObject(Simulation& simulation, const String& name, Object::Type type = Object::Type::Dynamic) noexcept override
    {
        if (name == "Yeast")
            return makeUnique<Yeast>(simulation, type);
        else if (name == "Cell")
            return makeUnique<Cell>(simulation, type);

        return nullptr;
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

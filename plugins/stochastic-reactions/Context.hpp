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

#include "plugins/cell/CellBase.hpp"
#include "Diffusion.hpp"

/* ************************************************************************ */

namespace plugin {
namespace stochastic_reactions {

/* ************************************************************************ */

/**
 * @brief Container for important pointers: current Cell and Diffusion.
 */
struct Context
{
    plugin::diffusion::Module* diffusion;
    plugin::cell::CellBase& cell;
    const DynamicArray<plugin::diffusion::Module::Coordinate>& coords;

    Context(plugin::diffusion::Module* d, plugin::cell::CellBase& c, const DynamicArray<plugin::diffusion::Module::Coordinate>& cs):
    diffusion(d), cell(c), coords(cs)
    {
        // Nothing to do
    }
};

/* ************************************************************************ */

}
}

/* ************************************************************************ */

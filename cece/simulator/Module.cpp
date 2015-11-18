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

// Declaration
#include "cece/simulator/Module.hpp"

// CeCe
#include "cece/simulator/Configuration.hpp"

/* ************************************************************************ */

namespace cece {
namespace simulator {

/* ************************************************************************ */

Module::~Module()
{
    // Nothing to do
}

/* ************************************************************************ */

void Module::init(Simulation& simulation)
{
    // Nothing to do
}

/* ************************************************************************ */

void Module::terminate(Simulation& simulation)
{
    // Nothing to do
}

/* ************************************************************************ */

void Module::update(Simulation& simulation, units::Time dt)
{
    // Nothing to do
}

/* ************************************************************************ */

void Module::loadConfig(Simulation& simulation, const Configuration& config)
{
    // Get module priority
    setPriority(config.get("priority", getPriority()));

#if ENABLE_RENDER
    setZOrder(config.get("z-order", getZOrder()));
#endif
}

/* ************************************************************************ */

void Module::storeConfig(Simulation& simulation, Configuration& config)
{
    // Store module priority
    config.set("priority", getPriority());

#if ENABLE_RENDER
    config.set("z-order", getZOrder());
#endif
}

/* ************************************************************************ */

#if ENABLE_RENDER
void Module::draw(const Simulation& simulation, render::Context& context)
{
    // Nothing to do
}
#endif

/* ************************************************************************ */

}
}

/* ************************************************************************ */

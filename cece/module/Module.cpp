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

// Declaration
#include "cece/module/Module.hpp"

// CeCe
#include "cece/config/Configuration.hpp"

/* ************************************************************************ */

namespace cece {
namespace module {

/* ************************************************************************ */

Module::Module(simulator::Simulation& simulation)
    : m_simulation(simulation)
{
    // Nothing to do
}

/* ************************************************************************ */

Module::~Module() = default;

/* ************************************************************************ */

void Module::loadConfig(const config::Configuration& config)
{
    // Get module priority
    setPriority(config.get("priority", getPriority()));

#ifdef CECE_ENABLE_RENDER
    setZOrder(config.get("z-order", getZOrder()));
#endif
}

/* ************************************************************************ */

void Module::storeConfig(config::Configuration& config) const
{
    // Store module priority
    config.set("priority", getPriority());

#ifdef CECE_ENABLE_RENDER
    config.set("z-order", getZOrder());
#endif
}

/* ************************************************************************ */

void Module::init(AtomicBool& termFlag)
{
    // Forward without termFlag
    init();
}

/* ************************************************************************ */

void Module::init()
{
    // Nothing to do
}

/* ************************************************************************ */

void Module::update()
{
    // Nothing to do
}

/* ************************************************************************ */

void Module::terminate()
{
    // Nothing to do
}

/* ************************************************************************ */

#ifdef CECE_ENABLE_RENDER
void Module::draw(render::Context& context)
{
    // Nothing to do
}
#endif

/* ************************************************************************ */

}
}

/* ************************************************************************ */

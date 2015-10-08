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

// Simulator
#include "simulator/Object.hpp"
#include "simulator/Module.hpp"
#include "simulator/Simulation.hpp"
#include "simulator/Configuration.hpp"

// Plugin
#include "plugins/python/TypePtr.hpp"

/* ************************************************************************ */

DECLARE_PYTHON_CLASS(simulator::Object);
DECLARE_PYTHON_CLASS(simulator::Module);
DECLARE_PYTHON_CLASS(simulator::Simulation);
DECLARE_PYTHON_CLASS(simulator::Configuration);

/* ************************************************************************ */

namespace plugin {
namespace python {

/* ************************************************************************ */

/**
 * @brief Initialize simulator namespace types.
 */
PyMODINIT_FUNC init_simulator();

/* ************************************************************************ */

}
}

/* ************************************************************************ */

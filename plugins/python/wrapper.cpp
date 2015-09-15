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
#include "wrapper.hpp"

// C++
#include <cassert>

// Simulator
#include "core/Map.hpp"
#include "core/Log.hpp"

/* ************************************************************************ */

namespace plugin {
namespace python {

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

Map<std::type_index, PyTypeObject*> g_dynamicTypes;

/* ************************************************************************ */

}

/* ************************************************************************ */

void registerDynamic(const std::type_info& info, PyTypeObject* type)
{
    // Type is registered
    if (g_dynamicTypes.find(std::type_index(info)) != g_dynamicTypes.end())
        Log::warning("Replacing python type object: ", type->tp_name);

    g_dynamicTypes.emplace(info, type);
}

/* ************************************************************************ */

PyTypeObject* findDynamic(const std::type_info& info)
{
    auto it = g_dynamicTypes.find(std::type_index(info));
    return it != g_dynamicTypes.end() ? it->second : nullptr;
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

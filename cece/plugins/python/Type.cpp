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
#include "cece/plugins/python/Type.hpp"

// CeCe
#include "cece/core/Assert.hpp"
#include "cece/core/Map.hpp"
#include "cece/core/Log.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace python {

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

Map<std::type_index, PyTypeObject*> g_dynamicTypes;

/* ************************************************************************ */

}

/* ************************************************************************ */

void registerType(const std::type_info& info, PyTypeObject* type)
{
    // Type is registered
    if (g_dynamicTypes.find(std::type_index(info)) != g_dynamicTypes.end())
        Log::warning("Replacing python type object: ", type->tp_name);

    g_dynamicTypes.emplace(info, type);
    Log::debug("Register type: ", info.name());
}

/* ************************************************************************ */

PyTypeObject* findType(const std::type_info& info)
{
    Log::debug("Searching for type: ", info.name());

    auto it = g_dynamicTypes.find(std::type_index(info));

    if (it != g_dynamicTypes.end())
        return it->second;

    Log::debug("Type not found");
    return nullptr;
}

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */

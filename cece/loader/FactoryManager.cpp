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
#include "cece/loader/FactoryManager.hpp"

// CeCe
#include "cece/loader/Loader.hpp"

/* ************************************************************************ */

namespace cece {
namespace loader {

/* ************************************************************************ */

ViewPtr<Factory> FactoryManager::get(const StringView& name) const noexcept
{
    auto it = m_factories.find(String(name));

    return it != m_factories.end() ? it->second.get() : nullptr;
}

/* ************************************************************************ */

UniquePtr<Loader> FactoryManager::create(const StringView& name) const
{
    auto factory = get(name);

    if (factory)
        return factory->create();

    throw FactoryNotFoundException("Loader factory not found: " + String(name));
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

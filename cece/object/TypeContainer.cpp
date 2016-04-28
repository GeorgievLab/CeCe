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
#include "cece/object/TypeContainer.hpp"

// C++
#include <algorithm>

// Declaration
#include "cece/object/Type.hpp"

/* ************************************************************************ */

namespace cece {
namespace object {

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

/**
 * @brief Find parameter in container.
 *
 * @param data
 *
 * @return
 */
template<typename Container>
auto find(Container& data, StringView name) noexcept -> decltype(&*data.begin())
{
    auto it = std::find_if(data.begin(), data.end(),
        [name](const Type& p) {
            return p.name == name;
        }
    );

    return it != data.end() ? &*it : nullptr;
}

/* ************************************************************************ */

}

/* ************************************************************************ */

bool TypeContainer::exists(StringView name) const noexcept
{
    return find(m_types, name) != nullptr;
}

/* ************************************************************************ */

ViewPtr<const Type> TypeContainer::get(StringView name) const noexcept
{
    return find(m_types, name);
}

/* ************************************************************************ */

void TypeContainer::add(Type type)
{
    auto ptr = find(m_types, type.name);

    if (ptr)
    {
        *ptr = std::move(type);
    }
    else
    {
        m_types.emplace_back(std::move(type));
    }
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

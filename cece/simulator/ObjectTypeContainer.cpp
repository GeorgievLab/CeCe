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
#include "cece/simulator/ObjectTypeContainer.hpp"

// C++
#include <algorithm>

/* ************************************************************************ */

namespace cece {
namespace simulator {

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
auto find(Container& data, const StringView& name) noexcept -> decltype(&*data.begin())
{
    auto it = std::find_if(data.begin(), data.end(),
        [&name](const ObjectType& p) {
            return p.name == name;
        }
    );

    return it != data.end() ? &*it : nullptr;
}

/* ************************************************************************ */

}

/* ************************************************************************ */

bool ObjectTypeContainer::exists(const StringView& name) const noexcept
{
    return find(m_data, name) != nullptr;
}

/* ************************************************************************ */

ViewPtr<const ObjectType> ObjectTypeContainer::get(const StringView& name) const noexcept
{
    return find(m_data, name);
}

/* ************************************************************************ */

void ObjectTypeContainer::add(ObjectType type)
{
    auto ptr = find(m_data, type.name);

    if (ptr)
    {
        *ptr = std::move(type);
    }
    else
    {
        m_data.emplace_back(std::move(type));
    }
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

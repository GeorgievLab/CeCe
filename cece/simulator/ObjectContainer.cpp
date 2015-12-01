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
#include "cece/simulator/ObjectContainer.hpp"

// C++
#include <algorithm>

/* ************************************************************************ */

namespace cece {
namespace simulator {

/* ************************************************************************ */

ObjectContainer::SizeType ObjectContainer::getCountByType(const StringView& typeName) const noexcept
{
    SizeType res = 0ul;

    for (const auto& obj : m_data)
    {
        if (obj.ptr->getTypeName() == typeName)
            ++res;
    }

    return res;
}

/* ************************************************************************ */

DynamicArray<ViewPtr<Object>> ObjectContainer::getByType(const StringView& typeName) const noexcept
{
    DynamicArray<ViewPtr<Object>> objects;
    objects.reserve(m_data.size());

    for (const auto& obj : m_data)
    {
        if (obj.ptr->getTypeName() == typeName)
            objects.push_back(obj.ptr);
    }

    return objects;
}

/* ************************************************************************ */

void ObjectContainer::deleteObject(ViewPtr<Object> object)
{
    for (auto& obj : m_data)
    {
        if (obj.ptr.get() == object)
        {
            obj.deleted = true;
            break;
        }
    }
}

/* ************************************************************************ */

void ObjectContainer::removeDeleted() noexcept
{
    // Delete objects
    m_data.erase(std::remove_if(m_data.begin(), m_data.end(), [](const Record& rec) {
        return rec.deleted;
    }), m_data.end());
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

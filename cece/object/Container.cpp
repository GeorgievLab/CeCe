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
#include "cece/object/Container.hpp"

// C++
#include <algorithm>

// CeCe
#include "cece/object/Object.hpp"

/* ************************************************************************ */

namespace cece {
namespace object {

/* ************************************************************************ */

Container::SizeType Container::getCountByType(StringView typeName) const noexcept
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

DynamicArray<ViewPtr<Object>> Container::getByType(StringView typeName) const noexcept
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

void Container::deleteObject(ViewPtr<Object> object)
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

void Container::removeDeleted() noexcept
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

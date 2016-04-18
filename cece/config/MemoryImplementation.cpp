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
#include "cece/config/MemoryImplementation.hpp"

/* ************************************************************************ */

namespace cece {
namespace config {

/* ************************************************************************ */

DynamicArray<String> MemoryImplementation::getNames() const noexcept
{
    DynamicArray<String> names;
    names.reserve(m_data->values.size());

    for (const auto& p : m_data->values)
        names.push_back(std::move(p.first));

    return names;
}

/* ************************************************************************ */

DynamicArray<UniquePtr<Implementation>> MemoryImplementation::getSubs(StringView name) const noexcept
{
    DynamicArray<UniquePtr<Implementation>> res;

    auto it = m_data->data.find(name.getData());

    if (it == m_data->data.end())
        return res;

    for (const auto& ptr : it->second)
        res.push_back(makeUnique<MemoryImplementation>(ptr));

    return res;
}

/* ************************************************************************ */

DynamicArray<String> MemoryImplementation::getSubNames() const noexcept
{
    DynamicArray<String> names;
    names.reserve(m_data->data.size());

    for (const auto& p : m_data->data)
        names.push_back(p.first);

    return names;
}

/* ************************************************************************ */

UniquePtr<Implementation> MemoryImplementation::addSub(StringView name) noexcept
{
    auto data = makeShared<Data>();

    // Register data
    m_data->data[name.getData()].push_back(data);

    return makeUnique<MemoryImplementation>(std::move(data));
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

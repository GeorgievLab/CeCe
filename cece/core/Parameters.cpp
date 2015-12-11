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
#include "cece/core/Parameters.hpp"

// C++
#include <algorithm>

/* ************************************************************************ */

namespace cece {
inline namespace core {

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
auto find(Container& data, const Parameters::KeyViewType& name) noexcept -> decltype(&(data.begin()->second))
{
    auto it = std::find_if(data.begin(), data.end(),
        [&name](const Pair<Parameters::KeyType, Parameters::ValueType>& p) {
            return p.first == name;
        }
    );

    return it != data.end() ? &(it->second) : nullptr;
}

/* ************************************************************************ */

}

/* ************************************************************************ */

bool Parameters::exists(const KeyViewType& name) const noexcept
{
    return find(m_data, name) != nullptr;
}

/* ************************************************************************ */

Parameters::ValueType Parameters::get(const KeyViewType& name) const
{
    auto ptr = find(m_data, name);

    if (ptr)
        return *ptr;

    throw MissingParameterException("Cannot find parameter: " + String(name));
}

/* ************************************************************************ */

Parameters::ValueType& Parameters::get(const KeyViewType& name)
{
    auto ptr = find(m_data, name);

    if (ptr)
        return *ptr;

    // Insert
    m_data.emplace_back(KeyType(name), ValueType{});

    return m_data.back().second;
}

/* ************************************************************************ */

Parameters::ValueType Parameters::get(const KeyViewType& name, ValueType def) const noexcept
{
    auto ptr = find(m_data, name);

    if (ptr)
        return *ptr;

    return def;
}

/* ************************************************************************ */

void Parameters::set(KeyType name, ValueType value)
{
    auto ptr = find(m_data, name);

    if (ptr)
        *ptr = value;
    else
        m_data.push_back(makePair(name, value));
}

/* ************************************************************************ */

void Parameters::merge(const Parameters& parameters)
{
    for (const auto& param : parameters.m_data)
        set(param.first, param.second);
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

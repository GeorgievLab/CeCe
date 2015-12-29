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
#include "cece/program/NamedContainer.hpp"

// C++
#include <algorithm>

// CeCe
#include "cece/program/Program.hpp"

/* ************************************************************************ */

namespace cece {
namespace program {

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
auto find(Container& data, StringView name) noexcept -> decltype(&(data.begin()->second))
{
    auto it = std::find_if(data.begin(), data.end(),
        [&name](const Pair<String, UniquePtr<Program>>& p) {
            return p.first == name;
        }
    );

    return it != data.end() ? &(it->second) : nullptr;
}

/* ************************************************************************ */

}

/* ************************************************************************ */

NamedContainer::~NamedContainer()
{
    // Nothing to do
}

/* ************************************************************************ */

bool NamedContainer::exists(StringView name) const noexcept
{
    return find(m_programs, name) != nullptr;
}

/* ************************************************************************ */

ViewPtr<Program> NamedContainer::get(StringView name) const noexcept
{
    auto ptr = find(m_programs, name);

    if (ptr)
        return *ptr;

    return nullptr;
}

/* ************************************************************************ */

void NamedContainer::add(String name, UniquePtr<Program> program)
{
    auto ptr = find(m_programs, name);

    if (ptr)
    {
        *ptr = std::move(program);
    }
    else
    {
        m_programs.emplace_back(std::move(name), std::move(program));
    }
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

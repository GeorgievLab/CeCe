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
#include "cece/init/Container.hpp"

// CeCe
#include "cece/init/Initializer.hpp"

/* ************************************************************************ */

namespace cece {
namespace init {

/* ************************************************************************ */

Container::Container(const Container& rhs)
{
    m_initializers.reserve(rhs.m_initializers.size());

    // Clone initializers
    for (const auto& initializer : rhs.m_initializers)
        m_initializers.push_back(initializer->clone());
}

/* ************************************************************************ */

Container::Container(Container&& rhs) noexcept = default;

/* ************************************************************************ */

Container::~Container() = default;

/* ************************************************************************ */

Container& Container::operator=(const Container& rhs)
{
    m_initializers.clear();
    m_initializers.reserve(rhs.m_initializers.size());

    // Clone initializers
    for (const auto& initializer : rhs.m_initializers)
        m_initializers.push_back(initializer->clone());

    return *this;
}

/* ************************************************************************ */

Container& Container::operator=(Container&& rhs) = default;

/* ************************************************************************ */

void Container::clear()
{
    m_initializers.clear();
}

/* ************************************************************************ */

void Container::call(simulator::Simulation& simulation)
{
    // Invoke all stored initializers
    for (auto& initializer : m_initializers)
        initializer->call(simulation);
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

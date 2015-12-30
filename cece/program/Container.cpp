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
#include "cece/program/Container.hpp"

// CeCe
#include "cece/program/Program.hpp"

/* ************************************************************************ */

namespace cece {
namespace program {

/* ************************************************************************ */

Container::Container(const Container& rhs)
    : m_programs()
{
    m_programs.reserve(rhs.m_programs.size());

    // Clone programs
    for (const auto& program : rhs.m_programs)
        add(program->clone());
}

/* ************************************************************************ */

Container::Container(Container&& rhs) noexcept = default;

/* ************************************************************************ */

Container::~Container() = default;

/* ************************************************************************ */

Container& Container::operator=(const Container& rhs)
{
    m_programs.clear();
    m_programs.reserve(rhs.m_programs.size());

    // Clone programs
    for (const auto& program : rhs.m_programs)
        add(program->clone());

    return *this;
}

/* ************************************************************************ */

Container& Container::operator=(Container&& rhs) = default;

/* ************************************************************************ */

void Container::clear()
{
    m_programs.clear();
}

/* ************************************************************************ */

void Container::call(simulator::Simulation& simulation, object::Object& object, units::Time dt)
{
    // Invoke all stored programs
    for (auto& program : m_programs)
        program->call(simulation, object, dt);
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

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
#include "cece/simulator/ProgramContainer.hpp"

// CeCe
#include "cece/simulator/Program.hpp"

/* ************************************************************************ */

namespace cece {
namespace simulator {

/* ************************************************************************ */

ProgramContainer::ProgramContainer(const ProgramContainer& rhs)
    : m_data()
{
    m_data.reserve(rhs.m_data.size());

    // Clone programs
    for (const auto& program : rhs.m_data)
        add(program->clone());
}

/* ************************************************************************ */

ProgramContainer::ProgramContainer(ProgramContainer&& rhs) = default;

/* ************************************************************************ */

ProgramContainer::~ProgramContainer()
{
    // Nothing to do
}

/* ************************************************************************ */

ProgramContainer& ProgramContainer::operator=(const ProgramContainer& rhs)
{
    m_data.reserve(rhs.m_data.size());

    // Clone programs
    for (const auto& program : rhs.m_data)
        add(program->clone());

    return *this;
}

/* ************************************************************************ */

ProgramContainer& ProgramContainer::operator=(ProgramContainer&& rhs) = default;

/* ************************************************************************ */

}
}

/* ************************************************************************ */

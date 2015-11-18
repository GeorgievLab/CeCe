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

// This must be first
#include "cece/plugins/python/Python.hpp"

// Declaration
#include "cece/plugins/python/Initializer.hpp"

// Plugin
#include "cece/plugins/python/Utils.hpp"
#include "cece/plugins/python/Exception.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace python {

/* ************************************************************************ */

Initializer::Initializer()
{

}

/* ************************************************************************ */

Initializer::~Initializer()
{

}

/* ************************************************************************ */

void Initializer::operator()(simulator::Simulation& simulation) const
{
    if (!m_call)
        return;

    // Call function
    if (!call(m_call, &simulation))
        throw Exception();
}

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */

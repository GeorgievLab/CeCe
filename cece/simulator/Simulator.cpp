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
#include "cece/simulator/Simulator.hpp"

// C++
#include <cassert>
#include <chrono>
#ifdef __MINGW32__
// MinGW has issue with undefined reference to __impl_nanosleep (nanosleep function)
#undef _GLIBCXX_USE_NANOSLEEP
#endif
#include <thread>

// CeCe
#include "cece/simulator/Simulation.hpp"

/* ************************************************************************ */

namespace cece {
namespace simulator {

/* ************************************************************************ */

void Simulator::start()
{
    m_isRunning = true;

    while (m_isRunning)
        update();
}

/* ************************************************************************ */

bool Simulator::update()
{
    assert(m_simulation);
    return m_simulation->update();
}

/* ************************************************************************ */

#ifdef CECE_ENABLE_RENDER
void Simulator::draw(unsigned int width, unsigned int height)
{
    // Delete old objects
    m_renderContext.deleteReleasedObjects();

    // Start frame
    m_renderContext.frameBegin(width, height);

    assert(m_simulation);
    m_simulation->draw(m_renderContext);

    m_renderContext.frameEnd();
}
#endif

/* ************************************************************************ */

}
}

/* ************************************************************************ */

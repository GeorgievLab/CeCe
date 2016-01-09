/* ************************************************************************ */
/* Georgiev Lab (c) 2016                                                    */
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
#include "Simulator.hpp"

// CeCe
#include "cece/core/Exception.hpp"
#include "cece/plugin/Manager.hpp"

/* ************************************************************************ */

Simulator::Simulator(QObject* parent) : QObject(parent)
{
    // Nothing to do
}

/* ************************************************************************ */

void Simulator::start()
{
    m_running = true;

    while (m_running)
    {
        step();
    }
}

/* ************************************************************************ */

void Simulator::step()
{
    Q_ASSERT(m_simulation);

    // Do a step
    m_simulation->update();

    emit stepped(m_simulation->getIteration());
    emit running(true);
}

/* ************************************************************************ */

void Simulator::pause()
{
    m_running = false;
    emit running(false);
}

/* ************************************************************************ */

void Simulator::reset()
{
    createSimulation(m_source, m_type);
}

/* ************************************************************************ */

void Simulator::createSimulation(QString source, QString type)
{
    m_source = source;
    m_type = type;

    try
    {
        // Create a simulation
        auto simulation = cece::plugin::Manager::s().getContext().createSimulation(
            type.toLocal8Bit().data(), source.toLocal8Bit().data());

        // Create simulation
        m_simulation.reset(simulation.release());

        emit loaded(true);
    }
    catch (const cece::Exception& e)
    {
        emit loadError(e.what());
        emit loaded(false);
    }
}

/* ************************************************************************ */

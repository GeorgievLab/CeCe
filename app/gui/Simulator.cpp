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

// Qt
#include <QMutexLocker>
#include <QThread>

// CeCe
#include "cece/core/Exception.hpp"
#include "cece/plugin/Manager.hpp"

/* ************************************************************************ */

namespace cece {
namespace gui {

/* ************************************************************************ */

Simulator::Simulator(QObject* parent)
    : QObject(parent)
{
    // Nothing to do
}

/* ************************************************************************ */

void Simulator::start()
{
    if (!m_simulation)
        return;

    emit running(m_running = true);
    emit simulationStarted();
}

/* ************************************************************************ */

bool Simulator::step()
{
    Q_ASSERT(m_simulation);
    bool res;

    try
    {
        AtomicBool flag{false};
        QMutexLocker _(&m_mutex);

        if (!m_simulation->isInitialized())
            m_simulation->initialize(flag);

        // Do a step
        res = m_simulation->update();
    }
    catch (const Exception& e)
    {
        emit simulationError(e.what());
        return false;
    }

    emit stepped(static_cast<int>(m_simulation->getIteration()),
        static_cast<int>(m_simulation->getIterations()));

    // Simulation finished
    if (!res)
        emit simulationFinished(m_simulation->getIteration() == m_simulation->getIterations());

    return res;
}

/* ************************************************************************ */

void Simulator::pause()
{
    emit running(m_running = false);
}

/* ************************************************************************ */

void Simulator::reset()
{
    createSimulation(m_source, m_type);
}

/* ************************************************************************ */

void Simulator::createSimulation(QString source, QString type)
{
    if (m_running)
    {
        emit loadError("Simulation is running");
        return;
    }

    try
    {
        QMutexLocker _(&m_mutex);

        // Create a simulation
        auto simulation =
            cece::plugin::Manager::s().getContext().createSimulation(
                type.toLocal8Bit().data(), source.toLocal8Bit().data());

        // Create simulation
        m_simulation.reset(simulation.release());

        // Store after initialization
        m_source = source;
        m_type = type;

        emit loaded(true);
        emit stepped(static_cast<int>(m_simulation->getIteration()),
            static_cast<int>(m_simulation->getIterations()));
    }
    catch (const cece::Exception& e)
    {
        emit loadError(e.what());
        emit loaded(false);
    }
}

/* ************************************************************************ */

void Simulator::simulate()
{
    while (m_running && step())
        QThread::msleep(1000 / 30);

    m_running = false;
    emit simulationFinished(m_simulation->getIteration() == m_simulation->getIterations());
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

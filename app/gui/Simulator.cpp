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
#include "Simulator.hpp"

// Qt
#include <QThread>

// CeCe
#include "cece/core/Exception.hpp"
#include "cece/plugin/Manager.hpp"
#include "cece/simulator/Simulation.hpp"

/* ************************************************************************ */

namespace cece {
namespace gui {

/* ************************************************************************ */

Simulator::Simulator(plugin::Manager& manager, QObject* parent)
    : QObject(parent)
    , m_manager(manager)
{
    // Nothing to do
}

/* ************************************************************************ */

Simulator::~Simulator() = default;

/* ************************************************************************ */

void Simulator::simulationLoad(QString type, QString source)
{
    // Convert to CeCe string
    const String src = source.toStdString();
    const String tp = type.toStdString();

    // Create simulation
    try
    {
        m_simulation = m_manager.getContext().createSimulation(tp, src);
        emit simulationLoaded(m_simulation.get());
    }
    catch (const Exception& e)
    {
        emit simulationError(e.what());
    }
}

/* ************************************************************************ */

void Simulator::start()
{
    if (m_simulation)
    {
        m_running = true;
        emit simulationStarted();
    }
}

/* ************************************************************************ */

bool Simulator::step()
{
    Q_ASSERT(m_simulation);

    // Simulation is not initialized
    if (!m_simulation->isInitialized())
    {
        // Notify about init start
        emit simulationInitStarted();

        try
        {
            // Perform simulation initialization
            m_simulation->initialize(m_initTerm);

            // Terminated by user
            if (m_initTerm)
            {
                emit simulationInitFailed("User terminated");
                return false;
            }

            emit simulationInitialized();
        }
        catch (const Exception& e)
        {
            emit simulationInitFailed(e.what());
            return false;
        }
    }

    try
    {
        // Do a step
        bool res = m_simulation->update();

        emit simulationStepped(static_cast<int>(m_simulation->getIteration()));

        if (!res)
            simulationFinished();

        return res;
    }
    catch (const Exception& e)
    {
        emit simulationError(e.what());
        return false;
    }
}

/* ************************************************************************ */

void Simulator::pause()
{
    m_running = false;
    emit simulationPaused();
}

/* ************************************************************************ */

void Simulator::simulate()
{
    while (m_running && step())
        QThread::msleep(1000 / 30);
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

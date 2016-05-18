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
#include <QMutexLocker>

// CeCe
#include "cece/core/Exception.hpp"
#include "cece/plugin/Manager.hpp"
#include "cece/simulator/Simulation.hpp"

/* ************************************************************************ */

namespace cece {
namespace gui {

/* ************************************************************************ */

Simulator::Simulator(plugin::Manager& manager, QObject* parent) noexcept
    : QObject(parent)
    , m_manager(manager)
{
    qRegisterMetaType<Simulator::Mode>("Mode");
}

/* ************************************************************************ */

Simulator::~Simulator() = default;

/* ************************************************************************ */

void Simulator::simulationLoad(QString type, QString source) noexcept
{
    // Convert to CeCe string
    const String src = source.toStdString();
    const String tp = type.toStdString();

    // Create simulation
    try
    {
        QMutexLocker _(getMutex());

        m_simulation = m_manager.getContext().createSimulation(tp, src);

        emit simulationLoaded(m_simulation.get());
    }
    catch (const Exception& e)
    {
        emit error(Mode::Idle, e.what());
    }
}

/* ************************************************************************ */

void Simulator::start(Mode mode) noexcept
{
    if (m_simulation)
    {
        m_mode = mode;
        emit startRequest();
    }
}

/* ************************************************************************ */

bool Simulator::step() noexcept
{
    Q_ASSERT(m_simulation);

    // Simulation is not initialized
    if (!m_simulation->isInitialized())
        initialize();

    // Don't continue
    if (!m_running)
        return false;

    try
    {
        QMutexLocker _(getMutex());

        // Do a step
        m_running = m_simulation->update();

        emit stepped(m_mode, static_cast<int>(m_simulation->getIteration()));

        return m_running;
    }
    catch (const Exception& e)
    {
        emit error(Mode::Simulate, e.what());
        return false;
    }
}

/* ************************************************************************ */

void Simulator::stop() noexcept
{
    m_running = false;
}

/* ************************************************************************ */

void Simulator::run() noexcept
{
    m_running = true;

    switch (m_mode)
    {
    case Mode::Idle:                        break;
    case Mode::Initialize:  initialize();   break;
    case Mode::Simulate:    simulate();     break;
    }

    m_running = false;
}

/* ************************************************************************ */

void Simulator::initialize() noexcept
{
    try
    {
        QMutexLocker _(getMutex());

        emit started(Mode::Initialize);

        // Perform simulation initialization
        m_simulation->initialize(m_running);

        emit finished(Mode::Initialize);
    }
    catch (const Exception& e)
    {
        emit error(Mode::Initialize, e.what());
    }
}

/* ************************************************************************ */

void Simulator::simulate() noexcept
{
    Q_ASSERT(m_simulation);

    // Simulation is not initialized
    if (!m_simulation->isInitialized())
        initialize();

    emit started(Mode::Simulate);

    while (m_running && step())
        QThread::msleep(1000 / 30);

    emit finished(Mode::Simulate);
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

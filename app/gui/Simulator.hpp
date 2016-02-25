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

#pragma once

/* ************************************************************************ */

// Qt
#include <QScopedPointer>
#include <QObject>
#include <QMutex>

// CeCe
#include "cece/core/ViewPtr.hpp"
#include "cece/simulator/Simulation.hpp"

/* ************************************************************************ */

namespace cece {
namespace gui {

/* ************************************************************************ */

/**
 * @brief GUI simulator class.
 */
class Simulator : public QObject
{
    Q_OBJECT

public:


    /**
     * @brief Constructor.
     * @param parent
     */
    explicit Simulator(QObject* parent = nullptr);


public:


    /**
     * @brief Returns if simulation is running.
     *
     * @return
     */
    bool isRunning() const noexcept
    {
        return m_running;
    }


// Public Accessors
public:


    /**
     * @brief Return currently visualized simulation.
     *
     * @return
     */
    ViewPtr<simulator::Simulation> getSimulation() const noexcept
    {
        return m_simulation.data();
    }


    /**
     * @brief Return syncronization mutex.
     *
     * @return
     */
    QMutex* getMutex() noexcept
    {
        return &m_mutex;
    }


signals:


    /**
     * @brief Simulation started.
     */
    void simulationStarted();


    /**
     * @brief Simulation finished.
     *
     * @param end
     */
    void simulationFinished(bool end);


    /**
     * @brief Simulation error.
     *
     * @param message
     */
    void simulationError(QString message);


    /**
     * @brief Simulation has been loaded.
     *
     * @param flag
     */
    void loaded(bool flag);


    /**
     * @brief Load error.
     *
     * @param message
     */
    void loadError(QString message);


    /**
     * @brief Report if simulation is running.
     *
     * @param flag
     */
    void running(bool flag);


    /**
     * @brief A step has been performed.
     *
     * @param iteration
     * @param iterations
     */
    void stepped(int iteration, int iterations);


public:


    /**
     * @brief Start simulation.
     */
    void start();


    /**
     * @brief Do a simulation step.
     *
     * @return
     */
    bool step();


    /**
     * @brief Pause simulation.
     */
    void pause();


    /**
     * @brief Reset simulation.
     */
    void reset();


    /**
     * @brief Create a simulation from source.
     * @param source
     */
    void createSimulation(QString source, QString type);


    /**
     * @brief Simulate simulation.
     */
    void simulate();


// Private Data Members
private:

    /// Current simulation.
    QScopedPointer<cece::simulator::Simulation> m_simulation;

    /// Mutex.
    QMutex m_mutex;

    /// If simulation is running.
    bool m_running = false;

    /// Current source.
    QString m_source;

    /// Source type.
    QString m_type;
};

/* ************************************************************************ */

}
}

/* ************************************************************************ */

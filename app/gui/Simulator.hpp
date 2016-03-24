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
#include <QObject>

// CeCe
#include "cece/core/ViewPtr.hpp"

/* ************************************************************************ */

namespace cece { namespace simulator { class Simulation; } }

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


// Public Mutators
public slots:


    /**
     * @brief Set visualized simulation.
     *
     * @param simulation
     */
    void setSimulation(simulator::Simulation* simulation) noexcept
    {
        m_simulation = simulation;
        emit simulationChanged(m_simulation);
    }


signals:


    /**
     * @brief Simulator current simulation has been changed.
     *
     * @param simulation Pointer to simulation or nullptr.
     */
    void simulationChanged(simulator::Simulation* simulation);


    /**
     * @brief Simulation started.
     */
    void simulationStarted();


    /**
     * @brief Simulation initialization started.
     */
    void simulationInitStarted();


    /**
     * @brief Simulation has been initialized.
     */
    void simulationInitialized();


    /**
     * @brief Simulation initialization failed.
     *
     * @param message
     */
    void simulationInitFailed(QString message);


    /**
     * @brief Simulation has been paused.
     */
    void simulationPaused();


    /**
     * @brief Simulation finished.
     */
    void simulationFinished();


    /**
     * @brief A step has been performed.
     *
     * @param iteration Current iteration.
     */
    void simulationStepped(int iteration);


    /**
     * @brief Simulation error.
     *
     * @param message
     */
    void simulationError(QString message);


public slots:


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
     * @brief Simulate simulation.
     */
    void simulate();


// Private Data Members
private:

    /// Current simulation.
    ViewPtr<simulator::Simulation> m_simulation;

    /// If simulation is running.
    bool m_running = false;

    /// Initialization termination flag.
    AtomicBool m_initTerm{false};
};

/* ************************************************************************ */

}
}

/* ************************************************************************ */

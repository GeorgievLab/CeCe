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

#pragma once

/* ************************************************************************ */

// Qt
#include <QObject>
#include <QString>

// CeCe
#include "cece/core/ViewPtr.hpp"
#include "cece/core/UniquePtr.hpp"
#include "cece/core/Atomic.hpp"

/* ************************************************************************ */

namespace cece { namespace plugin { class Manager; } }
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
     *
     * @param manager
     * @param parent
     */
    explicit Simulator(plugin::Manager& manager, QObject* parent = nullptr);


    /**
     * @brief Destructor.
     */
    ~Simulator();


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
        return m_simulation.get();
    }


signals:


    /**
     * @brief Simulator current simulation has been changed/loaded.
     *
     * @param simulation Pointer to simulation or nullptr.
     */
    void simulationLoaded(simulator::Simulation* simulation);


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
     * @brief Load simulation.
     *
     * @param type Source type.
     * @param source
     */
    void simulationLoad(QString type, QString source);


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

    /// Plugin manager.
    plugin::Manager& m_manager;

    /// Current simulation.
    UniquePtr<simulator::Simulation> m_simulation;

    /// If simulation is running.
    bool m_running = false;

    /// Initialization termination flag.
    AtomicBool m_initTerm{false};
};

/* ************************************************************************ */

}
}

/* ************************************************************************ */

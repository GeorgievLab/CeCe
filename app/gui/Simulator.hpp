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
#include <QMutex>

// CeCe
#include "cece/core/ViewPtr.hpp"
#include "cece/core/UniquePtr.hpp"
#include "cece/core/Atomic.hpp"
#include "cece/plugin/Context.hpp"

/* ************************************************************************ */

namespace cece { namespace plugin { class Repository; } }
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

// Public Enums
public:


    /**
     * @brief Simulator modes.
     */
    enum class Mode
    {
        Idle,
        Initialize,
        Simulate
    };


// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param repository Plugin repository
     * @param parent
     */
    explicit Simulator(const plugin::Repository& repository, QObject* parent = nullptr) noexcept;


    /**
     * @brief Destructor.
     */
    ~Simulator();


// Public Accessors
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


    /**
     * @brief Returns current mode.
     *
     * @return
     */
    Mode getMode() const noexcept
    {
        return m_mode;
    }


// Public Accessors & Mutators
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


    /**
     * @brief Delete simulation.
     *
     * @return
     */
    void deleteSimulation() noexcept;


    /**
     * @brief Returns synchronization mutex.
     *
     * @return
     */
    QMutex* getMutex() noexcept
    {
        return &m_mutex;
    }


signals:


    /**
     * @brief Simulator current simulation has been changed/loaded.
     *
     * @param simulation Pointer to simulation or nullptr.
     */
    void simulationLoaded(simulator::Simulation* simulation);


    /**
     * @brief Simulation start request.
     */
    void startRequest();


    /**
     * @brief Simulation started in given mode.
     *
     * @param mode Simulation mode.
     */
    void started(Mode mode);


    /**
     * @brief Simulation error.
     *
     * @param mode    Simulation mode.
     * @param message Error message
     */
    void error(Mode mode, QString message);


    /**
     * @brief Simulation finished.
     *
     * @param mode Simulation mode.
     * @param end  If simulation can continue.
     */
    void finished(Mode mode, bool end = true);


    /**
     * @brief A step has been performed.
     *
     * @param mode      Simulation mode.
     * @param iteration Current iteration.
     */
    void stepped(Mode mode, int iteration);


public slots:


    /**
     * @brief Load simulation.
     *
     * @param type     Source type.
     * @param source   Source code.
     * @param filename Name of the source file.
     */
    void simulationLoad(QString type, QString source, QString filename) noexcept;


    /**
     * @brief Start simulation.
     *
     * @param mode Simulation mode.
     */
    void start(Mode mode = Mode::Simulate) noexcept;


    /**
     * @brief Do a simulation step.
     *
     * @return
     */
    bool step() noexcept;


    /**
     * @brief Stop simulation.
     */
    void stop() noexcept;


    /**
     * @brief Run simulator in current mode.
     */
    void run() noexcept;


// Private Operations
private:


    /**
     * @brief Initialize simulation.
     */
    void initialize() noexcept;


    /**
     * @brief Simulate simulation.
     */
    void simulate() noexcept;


// Private Data Members
private:

    /// Plugin context.
    plugin::Context m_context;

    /// Current simulation.
    UniquePtr<simulator::Simulation> m_simulation;

    /// Current simulator mode.
    Mode m_mode = Mode::Idle;

    /// If simulation is running.
    AtomicBool m_running{false};

    /// Simulation access mutex.
    QMutex m_mutex;
};

/* ************************************************************************ */

}
}

/* ************************************************************************ */

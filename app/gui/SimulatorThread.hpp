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

#pragma once

/* ************************************************************************ */

// wxWidgets
#include <wx/string.h>
#include <wx/thread.h>
#include <wx/msgqueue.h>
#include <wx/scopedptr.h>
#include <wx/event.h>
#include <wx/atomic.h>

// Simulator
#include "simulator/Simulation.hpp"
#include "simulator/SimulationLoader.hpp"
#include "simulator/Simulator.hpp"

/* ************************************************************************ */

wxDECLARE_EVENT(EVT_ERROR, wxCommandEvent);

/* ************************************************************************ */

/**
 * @brief Helper thread for simulation.
 */
class SimulatorThread : public wxThreadHelper
{

// Public Structures
public:


    /**
     * @brief Comunitation message structure.
     */
    struct Message
    {
        enum Code { NONE, NEW, LOAD, START, STEP, STOP };

        Message() = default;
        Message(Code code) : code(code) {}
        Message(Code code, const wxString& string) : code(code), string(string) {}

        Code code = NONE;
        wxString string;
    };


// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param handler Event handler.
     * @param loader Simulation loader.
     */
    explicit SimulatorThread(wxEvtHandler* handler, simulator::SimulationLoader* loader) noexcept;


    /**
     * @brief Destructor.
     */
    ~SimulatorThread();


// Public Accessors
public:


    /**
     * @brief If simulation is running.
     *
     * @return
     */
    bool isRunning() const noexcept
    {
        return m_running != 0;
    }


    /**
     * @brief Returns message queue.
     *
     * @return
     */
    wxMessageQueue<Message>& GetQueue() noexcept
    {
        return m_queue;
    }


    /**
     * @brief Returns current simulation.
     *
     * @return
     */
    simulator::Simulation* GetSimulation() noexcept
    {
        return m_simulation.get();
    }


    /**
     * @brief Returns simulator mutex.
     *
     * @return
     */
    wxMutex* GetMutex() noexcept
    {
        return &m_mutex;
    }


// Public Operations
public:


    /**
     * @brief Entry function.
     *
     * @return Thread return value.
     */
    wxThread::ExitCode Entry() noexcept override;


    /**
     * @brief Send new simulation message.
     */
    void SendNew() noexcept;


    /**
     * @brief Send start message.
     */
    void SendStart() noexcept;


    /**
     * @brief Send step message.
     */
    void SendStep() noexcept;


    /**
     * @brief Send stop message.
     */
    void SendStop() noexcept;


    /**
     * @brief Send load message.
     *
     * @param code Source code.
     */
    void SendLoad(const wxString& code) noexcept;


// Protected Operations
protected:


    /**
     * @brief Handle incomming messages.
     */
    void HandleMessages() noexcept;


    /**
     * @brief Perform simulation deleting.
     */
    void DoNew() noexcept;


    /**
     * @brief Perform simulation start.
     */
    void DoStart() noexcept;


    /**
     * @brief Perform simulation step.
     */
    void DoStep() noexcept;


    /**
     * @brief Perform simulation stop.
     */
    void DoStop() noexcept;


    /**
     * @brief Do load.
     *
     * @param code Source code.
     */
    void DoLoad(const wxString& code) noexcept;


    /**
     * @brief Send error message.
     *
     * @param msg Error message.
     */
    void SendError(const wxString& msg) noexcept;


// Private Data Members
private:

    /// Event handler.
    wxEvtHandler* m_handler;

    /// Message queue.
    wxMessageQueue<Message> m_queue;

    /// Simulator mutex.
    wxMutex m_mutex;

    /// Simulator.
    wxScopedPtr<simulator::Simulation> m_simulation;

    /// If simulator should run.
    wxAtomicInt m_running = 0;

    /// World factory
    wxScopedPtr<simulator::SimulationLoader> m_simulationLoader;

};

/* ************************************************************************ */


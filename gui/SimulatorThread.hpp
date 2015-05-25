
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
#include "simulator/SimulationFactory.hpp"
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
        enum Code { NONE, LOAD, START, STEP, STOP };

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
     * @param factory Simulation factory.
     */
    explicit SimulatorThread(wxEvtHandler* handler, simulator::SimulationFactory* factory) NOEXCEPT;


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
    bool isRunning() const NOEXCEPT
    {
        return m_running != 0;
    }


    /**
     * @brief Returns message queue.
     *
     * @return
     */
    wxMessageQueue<Message>& GetQueue() NOEXCEPT
    {
        return m_queue;
    }


    /**
     * @brief Returns current simulation.
     *
     * @return
     */
    simulator::Simulation* GetSimulation() NOEXCEPT
    {
        return m_simulation.get();
    }


    /**
     * @brief Returns simulator mutex.
     *
     * @return
     */
    wxMutex* GetMutex() NOEXCEPT
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
    wxThread::ExitCode Entry() NOEXCEPT override;


    /**
     * @brief Send start message.
     */
    void SendStart() NOEXCEPT;


    /**
     * @brief Send step message.
     */
    void SendStep() NOEXCEPT;


    /**
     * @brief Send stop message.
     */
    void SendStop() NOEXCEPT;


    /**
     * @brief Send load message.
     *
     * @param code Source code.
     */
    void SendLoad(const wxString& code) NOEXCEPT;


// Protected Operations
protected:


    /**
     * @brief Handle incomming messages.
     */
    void HandleMessages() NOEXCEPT;


    /**
     * @brief Perform simulation start.
     */
    void DoStart() NOEXCEPT;


    /**
     * @brief Perform simulation step.
     */
    void DoStep() NOEXCEPT;


    /**
     * @brief Perform simulation stop.
     */
    void DoStop() NOEXCEPT;


    /**
     * @brief Do load.
     *
     * @param code Source code.
     */
    void DoLoad(const wxString& code) NOEXCEPT;


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
    wxScopedPtr<simulator::SimulationFactory> m_simulationFactory;

};

/* ************************************************************************ */



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
    explicit SimulatorThread(wxEvtHandler* handler, simulator::SimulationFactory* factory) noexcept;


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
     * @brief Returns simulator.
     *
     * @return
     */
    simulator::Simulator* GetSimulator() noexcept
    {
        return &m_simulator;
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


// Private Data Members
private:

    /// Event handler.
    wxEvtHandler* m_handler;

    /// Message queue.
    wxMessageQueue<Message> m_queue;

    /// Simulator mutex.
    wxMutex m_mutex;

    /// Simulator.
    simulator::Simulator m_simulator;

    /// If simulator should run.
    wxAtomicInt m_running = 0;

    /// World factory
    wxScopedPtr<simulator::SimulationFactory> m_simulationFactory;

};

/* ************************************************************************ */


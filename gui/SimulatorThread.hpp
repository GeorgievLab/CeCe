
#pragma once

/* ************************************************************************ */

// wxWidgets
#include <wx/string.h>
#include <wx/thread.h>
#include <wx/msgqueue.h>
#include <wx/scopedptr.h>
#include <wx/event.h>

// Simulator
#include "simulator/World.h"
#include "simulator/WorldFactory.h"
#include "simulator/Simulator.h"

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
        enum Code { NONE, LOAD, START, STEP, STOP, RESTART };

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
     * @param factory World factory.
     */
    explicit SimulatorThread(wxEvtHandler* handler, simulator::WorldFactory* factory);


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
        return m_running;
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
     */
    wxThread::ExitCode Entry() override;


    /**
     * @brief Send start message.
     */
    void SendStart();


    /**
     * @brief Send step message.
     */
    void SendStep();


    /**
     * @brief Send stop message.
     */
    void SendStop();


    /**
     * @brief Send re-start message.
     */
    void SendRestart();


    /**
     * @brief Send load message.
     */
    void SendLoad(const wxString& code);


// Protected Operations
protected:


    /**
     * @brief Handle incomming messages.
     */
    void HandleMessages();


    /**
     * @brief Perform simulation start.
     */
    void DoStart();


    /**
     * @brief Perform simulation step.
     */
    void DoStep();


    /**
     * @brief Perform simulation stop.
     */
    void DoStop();


    /**
     * @brief Do load.
     *
     * @param code
     */
    void DoLoad(const wxString& code);


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
    bool m_running = false;

    /// World factory
    wxScopedPtr<simulator::WorldFactory> m_worldFactory;
};

/* ************************************************************************ */


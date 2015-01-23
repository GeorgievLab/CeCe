
#pragma once

/* ************************************************************************ */

// wxWidgets
#include <wx/string.h>
#include <wx/thread.h>
#include <wx/msgqueue.h>
#include <wx/event.h>

// Simulator
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
     */
    explicit SimulatorThread(wxEvtHandler* handler);


    /**
     * @brief Destructor.
     */
    ~SimulatorThread();


// Public Accessors
public:


    /**
     * @brief Returns message queue.
     *
     * @return
     */
    wxMessageQueue<Message>& GetQueue() noexcept
    {
        return m_queue;
    }


// Public Operations
public:


    /**
     * @brief Entry function.
     */
    wxThread::ExitCode Entry() override;


// Protected Operations
protected:


    /**
     * @brief Handle incomming messages.
     */
    void HandleMessages();


// Private Data Members
private:

    /// Event handler.
    wxEvtHandler* m_handler;

    /// Message queue.
    wxMessageQueue<Message> m_queue;

    /// Simulator.
    simulator::Simulator m_simulator;

    /// If simulator should run.
    bool m_running = false;

};

/* ************************************************************************ */


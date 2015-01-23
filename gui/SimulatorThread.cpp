
// Declaration
#include "gui/SimulatorThread.hpp"

// wxWidgets
#include <wx/log.h>

// Simulator
#include "simulator/World.h"

/* ************************************************************************ */

SimulatorThread::SimulatorThread(wxEvtHandler* handler)
    : m_handler(handler)
{
    if (CreateThread(wxTHREAD_JOINABLE) != wxTHREAD_NO_ERROR)
    {
        wxLogError("Could not create the worker thread!");
        return;
    }

    if (GetThread()->Run() != wxTHREAD_NO_ERROR)
    {
        wxLogError("Could not run the worker thread!");
        return;
    }
}

/* ************************************************************************ */

SimulatorThread::~SimulatorThread()
{
    if (GetThread() && GetThread()->IsRunning())
        GetThread()->Wait();
}

/* ************************************************************************ */

wxThread::ExitCode SimulatorThread::Entry()
{
    // Check if thread is still alive
    while (!GetThread()->TestDestroy())
    {
        HandleMessages();

        if (m_running)
            m_simulator.step();
    }

    return (wxThread::ExitCode) 0;
}

/* ************************************************************************ */

void SimulatorThread::HandleMessages()
{
    Message msg;

    while (true)
    {
        // Get message
        if (m_queue.ReceiveTimeout(100, msg) == wxMSGQUEUE_TIMEOUT)
            break;

        switch (msg.code)
        {
        case Message::LOAD:
            wxASSERT(m_simulator.getWorld());
            m_simulator.getWorld()->load(msg.string.To8BitData().data());
            break;

        case Message::START:
            m_running = true;
            break;

        case Message::STEP:
            m_simulator.step();
            break;

        case Message::STOP:
            m_running = false;
            break;

        case Message::RESTART:
            m_running = false;
            //m_simulator.reset();
            break;

        default:
            break;
        }
    }
}

/* ************************************************************************ */

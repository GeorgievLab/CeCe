
// Declaration
#include "gui/SimulatorThread.hpp"

// wxWidgets
#include <wx/log.h>

// Simulator
#include "simulator/World.h"

/* ************************************************************************ */

SimulatorThread::SimulatorThread(wxEvtHandler* handler, simulator::WorldFactory* factory)
    : m_handler(handler)
    , m_worldFactory(factory)
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
    {
        GetThread()->Delete();
        GetThread()->Wait();
    }
}

/* ************************************************************************ */

wxThread::ExitCode SimulatorThread::Entry()
{
    // Create new world
    m_simulator.setWorld(m_worldFactory->createWorld());

    // Check if thread is still alive
    while (!GetThread()->TestDestroy())
    {
        HandleMessages();

        if (m_running)
            DoStep();

        // Send repaint event
        //wxQueueEvent(m_handler, new wxPaintEvent());
    }

    return (wxThread::ExitCode) 0;
}

/* ************************************************************************ */

void SimulatorThread::SendStart()
{
    m_queue.Post({Message::START});
}

/* ************************************************************************ */

void SimulatorThread::SendStep()
{
    m_queue.Post({Message::STEP});
}

/* ************************************************************************ */

void SimulatorThread::SendStop()
{
    m_queue.Post({Message::STOP});
}

/* ************************************************************************ */

void SimulatorThread::SendRestart()
{
    m_queue.Post({Message::RESTART});
}

/* ************************************************************************ */

void SimulatorThread::SendLoad(const wxString& code)
{
    m_queue.Post({Message::LOAD, code});
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
            DoLoad(msg.string);
            break;

        case Message::START:
            DoStart();
            break;

        case Message::STEP:
            DoStep();
            break;

        case Message::STOP:
            DoStop();
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

void SimulatorThread::DoStart()
{
    m_running = true;
}

/* ************************************************************************ */

void SimulatorThread::DoStep()
{
    wxMutexLocker lock(m_mutex);

    m_simulator.step();
}

/* ************************************************************************ */

void SimulatorThread::DoStop()
{
    m_running = false;
}

/* ************************************************************************ */

void SimulatorThread::DoLoad(const wxString& code)
{
    wxMutexLocker lock(m_mutex);

    wxASSERT(m_simulator.getWorld());
    m_simulator.getWorld()->load(code.To8BitData().data());
}

/* ************************************************************************ */


// Declaration
#include "gui/SimulatorThread.hpp"

// wxWidgets
#include <wx/scopedptr.h>
#include <wx/log.h>

// Simulator
#include "simulator/World.hpp"

/* ************************************************************************ */

wxDEFINE_EVENT(EVT_UPDATED, wxCommandEvent);
wxDEFINE_EVENT(EVT_ERROR, wxCommandEvent);
wxDEFINE_EVENT(EVT_LOG, wxCommandEvent);

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
    // Create dummy world
    m_simulator.setWorld(m_worldFactory->createWorld());

    // Check if thread is still alive
    while (!GetThread()->TestDestroy())
    {
        bool update = HandleMessages();

        if (m_running)
        {
            DoStep();
            update = true;
        }

        // Send repaint event
        if (update)
            wxQueueEvent(m_handler, new wxCommandEvent(EVT_UPDATED));
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

bool SimulatorThread::HandleMessages()
{
    Message msg;
    bool result = false;

    while (true)
    {
        // Get message
        if (m_queue.ReceiveTimeout(100, msg) == wxMSGQUEUE_TIMEOUT)
            break;

        switch (msg.code)
        {
        case Message::LOAD:
            DoLoad(msg.string);
            result = true;
            break;

        case Message::START:
            DoStart();
            result = true;
            break;

        case Message::STEP:
            DoStep();
            result = true;
            break;

        case Message::STOP:
            DoStop();
            result = true;
            break;

        case Message::RESTART:
            m_running = false;
            m_simulator.reset();
            result = true;
            break;

        default:
            break;
        }
    }

    return result;
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

    try
    {
        // Create new world from source
        m_simulator.setWorld(m_worldFactory->fromSource(code.To8BitData().data()));

        // Setup log function
        m_simulator.getWorld()->setLogFunction([this](const std::string& msg) {
            wxScopedPtr<wxCommandEvent> event(new wxCommandEvent(EVT_LOG));
            event->SetString(msg);
            wxQueueEvent(m_handler, event.release());
        });
    }
    catch (const std::exception& e)
    {
        wxScopedPtr<wxCommandEvent> event(new wxCommandEvent(EVT_ERROR));
        event->SetString(e.what());
        wxQueueEvent(m_handler, event.release());
    }
}

/* ************************************************************************ */

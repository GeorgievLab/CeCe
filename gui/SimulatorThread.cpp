
// Declaration
#include "gui/SimulatorThread.hpp"

// wxWidgets
#include <wx/scopedptr.h>
#include <wx/log.h>

/* ************************************************************************ */

wxDEFINE_EVENT(EVT_ERROR, wxCommandEvent);

/* ************************************************************************ */

SimulatorThread::SimulatorThread(wxEvtHandler* handler, simulator::SimulationFactory* factory) NOEXCEPT
    : m_handler(handler)
    , m_simulationFactory(factory)
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

wxThread::ExitCode SimulatorThread::Entry() NOEXCEPT
{
    // Check if thread is still alive
    while (!GetThread()->TestDestroy())
    {
        HandleMessages();

        if (isRunning())
            DoStep();
    }

    return (wxThread::ExitCode) 0;
}

/* ************************************************************************ */

void SimulatorThread::SendStart() NOEXCEPT
{
    m_queue.Post({Message::START});
}

/* ************************************************************************ */

void SimulatorThread::SendStep() NOEXCEPT
{
    m_queue.Post({Message::STEP});
}

/* ************************************************************************ */

void SimulatorThread::SendStop() NOEXCEPT
{
    m_queue.Post({Message::STOP});
}

/* ************************************************************************ */

void SimulatorThread::SendLoad(const wxString& code) NOEXCEPT
{
    m_queue.Post({Message::LOAD, code});
}

/* ************************************************************************ */

void SimulatorThread::HandleMessages() NOEXCEPT
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

        default:
            break;
        }
    }
}

/* ************************************************************************ */

void SimulatorThread::DoStart() NOEXCEPT
{
    wxAtomicInc(m_running);
}

/* ************************************************************************ */

void SimulatorThread::DoStep() NOEXCEPT
{
    try
    {
        wxMutexLocker lock(m_mutex);

        // Simulator step
        m_simulator.step();
    }
    catch (const std::exception& e)
    {
        wxScopedPtr<wxCommandEvent> event(new wxCommandEvent(EVT_ERROR));
        event->SetString(e.what());
        wxQueueEvent(m_handler, event.release());
    }
}

/* ************************************************************************ */

void SimulatorThread::DoStop() NOEXCEPT
{
    wxAtomicDec(m_running);
}

/* ************************************************************************ */

void SimulatorThread::DoLoad(const wxString& code) NOEXCEPT
{
    try
    {
        wxMutexLocker lock(m_mutex);

        // Create new world from source
        m_simulator.setSimulation(m_simulationFactory->fromSource(code.To8BitData().data()));
    }
    catch (const std::exception& e)
    {
        wxScopedPtr<wxCommandEvent> event(new wxCommandEvent(EVT_ERROR));
        event->SetString(e.what());
        wxQueueEvent(m_handler, event.release());
    }
}

/* ************************************************************************ */

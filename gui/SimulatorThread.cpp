
// Declaration
#include "gui/SimulatorThread.hpp"

// wxWidgets
#include <wx/scopedptr.h>
#include <wx/log.h>

/* ************************************************************************ */

wxDEFINE_EVENT(EVT_ERROR, wxCommandEvent);
wxDEFINE_EVENT(EVT_OLD_SIMULATION, wxCommandEvent);

/* ************************************************************************ */

SimulatorThread::SimulatorThread(wxEvtHandler* handler, simulator::SimulationFactory* factory) noexcept
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

wxThread::ExitCode SimulatorThread::Entry() noexcept
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

void SimulatorThread::SendStart() noexcept
{
    m_queue.Post({Message::START});
}

/* ************************************************************************ */

void SimulatorThread::SendStep() noexcept
{
    m_queue.Post({Message::STEP});
}

/* ************************************************************************ */

void SimulatorThread::SendStop() noexcept
{
    m_queue.Post({Message::STOP});
}

/* ************************************************************************ */

void SimulatorThread::SendLoad(const wxString& code) noexcept
{
    m_queue.Post({Message::LOAD, code});
}

/* ************************************************************************ */

void SimulatorThread::HandleMessages() noexcept
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

void SimulatorThread::DoStart() noexcept
{
    wxAtomicInc(m_running);
}

/* ************************************************************************ */

void SimulatorThread::DoStep() noexcept
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

void SimulatorThread::DoStop() noexcept
{
    wxAtomicDec(m_running);
}

/* ************************************************************************ */

void SimulatorThread::DoLoad(const wxString& code) noexcept
{
    try
    {
        wxMutexLocker lock(m_mutex);

        // Create new world from source
        auto old = m_simulator.setAndGetSimulation(m_simulationFactory->fromSource(code.To8BitData().data()));

        // Send old simulation
        if (old)
        {
            wxScopedPtr<wxCommandEvent> event(new wxCommandEvent(EVT_OLD_SIMULATION));
            event->SetClientData(old.release());
            wxQueueEvent(m_handler, event.release());
        }
    }
    catch (const std::exception& e)
    {
        wxScopedPtr<wxCommandEvent> event(new wxCommandEvent(EVT_ERROR));
        event->SetString(e.what());
        wxQueueEvent(m_handler, event.release());
    }
}

/* ************************************************************************ */

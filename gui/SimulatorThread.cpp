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

// Declaration
#include "gui/SimulatorThread.hpp"

// wxWidgets
#include <wx/scopedptr.h>
#include <wx/log.h>

/* ************************************************************************ */

wxDEFINE_EVENT(EVT_ERROR, wxCommandEvent);

/* ************************************************************************ */

SimulatorThread::SimulatorThread(wxEvtHandler* handler, simulator::SimulationLoader* loader) noexcept
    : m_handler(handler)
    , m_simulationLoader(loader)
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
        wxLogNull no;

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

void SimulatorThread::SendNew() noexcept
{
    m_queue.Post({Message::NEW});
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
        case Message::NEW:
            DoNew();
            break;

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

void SimulatorThread::DoNew() noexcept
{
    // Stop previous simulation
    DoStop();

    // Delete the old one
    wxMutexLocker lock(m_mutex);
    m_simulation.reset();
}

/* ************************************************************************ */

void SimulatorThread::DoStart() noexcept
{
    // Do not start simulation when there is none
    if (!m_simulation)
        return;

    if (!isRunning())
        wxAtomicInc(m_running);
}

/* ************************************************************************ */

void SimulatorThread::DoStep() noexcept
{
    // No simulation
    if (!m_simulation)
        return;

    try
    {
        wxMutexLocker lock(m_mutex);

        // Update simulation
        wxASSERT(m_simulation.get());
        m_simulation->update();
    }
    catch (const std::exception& e)
    {
        // Stop simulation
        DoStop();
        SendError(e.what());
    }
}

/* ************************************************************************ */

void SimulatorThread::DoStop() noexcept
{
    if (isRunning())
        wxAtomicDec(m_running);
}

/* ************************************************************************ */

void SimulatorThread::DoLoad(const wxString& code) noexcept
{
    try
    {
        // Create new simulation from source
        auto ptr = m_simulationLoader->fromSource(code.To8BitData().data());

        // Set simulation
        wxMutexLocker lock(m_mutex);
        m_simulation.reset(ptr.release());
    }
    catch (const std::exception& e)
    {
        // Stop simulation
        DoStop();
        SendError(e.what());
    }
}

/* ************************************************************************ */

void SimulatorThread::SendError(const wxString& msg) noexcept
{
    wxScopedPtr<wxCommandEvent> event(new wxCommandEvent(EVT_ERROR));
    event->SetString(msg);
    wxQueueEvent(m_handler, event.release());
}

/* ************************************************************************ */

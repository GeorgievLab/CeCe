
/* ************************************************************************ */

// Declaration
#include "Simulator.h"

// wxWidgets
#include <wx/log.h>
#include <wx/utils.h>

// Core
#include "World.h"

/* ************************************************************************ */

wxDEFINE_EVENT(EVT_SIMULATION_UPDATE, wxThreadEvent);

/* ************************************************************************ */

void Simulator::Start()
{
    if (!m_world)
    {
        wxLogError("Cannot simulate without World!");
        return;
    }

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

void Simulator::Stop()
{
    if (GetThread() && GetThread()->IsRunning())
        GetThread()->Delete();
}

/* ************************************************************************ */

void Simulator::Step()
{
    wxASSERT(m_world);
    m_world->Update();
}

/* ************************************************************************ */

wxThread::ExitCode Simulator::Entry()
{
    while (!GetThread()->TestDestroy())
    {
        Step();

        wxQueueEvent(m_handler, new wxThreadEvent(EVT_SIMULATION_UPDATE));

        wxMilliSleep(30);
    }

    return (wxThread::ExitCode) 0;
}

/* ************************************************************************ */

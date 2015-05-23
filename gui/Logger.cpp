
// Declaration
#include "gui/Logger.hpp"

// wxWidgets
#include <wx/scopedptr.h>

/* ************************************************************************ */

wxDEFINE_EVENT(EVT_LOG, wxCommandEvent);

/* ************************************************************************ */

Logger::Buffer::Buffer(wxEvtHandler* handler)
    : m_handler(handler)
{
    setp(m_buffer.data(), m_buffer.data() + m_buffer.size() - 1);
}

/* ************************************************************************ */

int Logger::Buffer::flushBuffer()
{
    int num = pptr() - pbase();

    wxScopedPtr<wxCommandEvent> event(new wxCommandEvent(EVT_LOG));
    event->SetString(wxString(m_buffer.data(), num));
    wxQueueEvent(m_handler, event.release());

    pbump(-num);
    return num;
}

/* ************************************************************************ */

int Logger::Buffer::overflow(int c)
{
    if (c != EOF)
    {
        *pptr() = c;
        pbump(1);
    }

    if (flushBuffer() == EOF)
        return EOF;

    return c;
}

/* ************************************************************************ */

int Logger::Buffer::sync()
{
    if (flushBuffer() == EOF)
        return -1;

    return 0;
}

/* ************************************************************************ */

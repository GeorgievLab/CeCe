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

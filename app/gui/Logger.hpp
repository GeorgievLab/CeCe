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

#pragma once

/* ************************************************************************ */

// C++
#include <streambuf>
#include <array>
#include <ostream>

// wxWidgets
#include <wx/event.h>

/* ************************************************************************ */

wxDECLARE_EVENT(EVT_LOG, wxCommandEvent);

/* ************************************************************************ */

/**
 * @brief Stream buffer logger.
 */
class Logger : public std::ostream
{

// Public Structures
public:


    struct Buffer : public std::streambuf
    {
        explicit Buffer(wxEvtHandler* handler);
        int flushBuffer();
        int overflow(int c = EOF) override;
        int sync() override;

    private:

        std::array<char, 1024> m_buffer;
        wxEvtHandler* m_handler;
    };


// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param handler
     */
    explicit Logger(wxEvtHandler* handler)
        : std::ostream(&m_buffer)
        , m_buffer(handler)
    {
        // Nothing to do
    }


    /**
     * @brief Returns output buffer.
     *
     * @return
     */
    Buffer& GetBuffer() noexcept
    {
        return m_buffer;
    }


// Private Data Members
private:


    /// Output buffer.
    Buffer m_buffer;

};

/* ************************************************************************ */


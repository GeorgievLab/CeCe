
#pragma once

/* ************************************************************************ */

// C++
#include <streambuf>
#include <array>

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
    Buffer& GetBuffer() NOEXCEPT
    {
        return m_buffer;
    }


// Private Data Members
private:


    /// Output buffer.
    Buffer m_buffer;

};

/* ************************************************************************ */


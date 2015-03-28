
/* ************************************************************************ */

#include <wx/app.h>
#include <wx/event.h>
#include <wx/image.h>
#include <wx/cmdline.h>
#include <wx/scopedptr.h>

// GUI
#include "MainFrame.h"

/* ************************************************************************ */

// Define the MainApp
class MainApp : public wxApp
{
public:

    bool OnInit() override
    {
        if (!wxApp::OnInit())
            return false;

        // Add the common image handlers
        wxImage::AddHandler(new wxPNGHandler);
        wxImage::AddHandler(new wxJPEGHandler);

        SetAppName("cell-sim");
        SetAppDisplayName("Cell simulator");

        wxScopedPtr<MainFrame> frame(new MainFrame());

        // Load init file
        if (m_initFileName.IsOk())
            frame->LoadFile(m_initFileName);

        SetTopWindow(frame.release());
        return GetTopWindow()->Show();
    }

    void OnInitCmdLine(wxCmdLineParser& parser) override
    {
        parser.AddParam("Simulation file", wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);
    }

    bool OnCmdLineParsed(wxCmdLineParser& parser) override
    {
        if (parser.GetParamCount() > 0)
            m_initFileName = wxFileName::FileName(parser.GetParam(0));

        return true;
    }

// Private Data Members
private:

    wxFileName m_initFileName;

};

/* ************************************************************************ */

DECLARE_APP(MainApp)
IMPLEMENT_APP(MainApp)

/* ************************************************************************ */

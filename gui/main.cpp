
/* ************************************************************************ */

#include <wx/app.h>
#include <wx/event.h>
#include <wx/image.h>

// GUI
#include "MainFrame.h"

/* ************************************************************************ */

// Define the MainApp
class MainApp : public wxApp
{
public:

    bool OnInit() override
    {
        // Add the common image handlers
        wxImage::AddHandler(new wxPNGHandler);
        wxImage::AddHandler(new wxJPEGHandler);

        SetAppName("cell-sim");
        SetAppDisplayName("Cell simulator");

        SetTopWindow(new MainFrame());
        return GetTopWindow()->Show();
    }
};

/* ************************************************************************ */

DECLARE_APP(MainApp)
IMPLEMENT_APP(MainApp)

/* ************************************************************************ */

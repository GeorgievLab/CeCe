#include <wx/app.h>
#include <wx/event.h>
#include <wx/image.h>

#include "MainFrame.h"

// Define the MainApp
class MainApp : public wxApp
{
public:

    bool OnInit() override
    {
        // Add the common image handlers
        wxImage::AddHandler( new wxPNGHandler );
        wxImage::AddHandler( new wxJPEGHandler );

        SetTopWindow(new MainFrame());
        return GetTopWindow()->Show();
    }
};

DECLARE_APP(MainApp)
IMPLEMENT_APP(MainApp)

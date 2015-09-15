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

// wxWidgets
#include <wx/app.h>
#include <wx/event.h>
#include <wx/image.h>
#include <wx/cmdline.h>
#include <wx/scopedptr.h>
#include <wx/filename.h>

// GUI
#include "MainFrame.h"

// Simulator
#include "core/String.hpp"
#include "simulator/PluginManager.hpp"

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

/**
 * @brief Returns plugins directory.
 *
 * @return
 */
String getPluginsDirectory() noexcept
{
#if _WIN32
    const wxFileName path = wxStandardPaths::GetExecutablePath();
    path.SetFullName("");
    path.AppendDir(DIR_PLUGINS);
    return path.GetFullPath().ToStdString();
#else
    // Absolute path to plugins directory on Linux
    return DIR_PLUGINS;
#endif
}

/* ************************************************************************ */

}

/* ************************************************************************ */

/**
 * @brief GUI main application.
 */
class MainApp : public wxApp
{

// Public Operations
public:


    /**
     * @brief Initialize application.
     *
     * @return
     */
    bool OnInit() override
    {
        if (!wxApp::OnInit())
            return false;

        // Register plugins directory
        simulator::PluginManager::addDirectory(getPluginsDirectory());
        simulator::PluginManager::rescanDirectories();

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


    /**
     * @brief On exit handler.
     *
     * @return
     */
    int OnExit() override
    {
        simulator::PluginManager::releasePlugins();
        return OnExit();
    }


    /**
     * @brief Initialize command line parser.
     *
     * @param parser
     */
    void OnInitCmdLine(wxCmdLineParser& parser) override
    {
        parser.AddParam("Simulation file", wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);
    }


    /**
     * @brief Process parsed command line.
     *
     * @param parser
     *
     * @return
     */
    bool OnCmdLineParsed(wxCmdLineParser& parser) override
    {
        if (parser.GetParamCount() > 0)
        {
            m_initFileName = wxFileName::FileName(parser.GetParam(0));
            m_initFileName.MakeAbsolute();
        }

        return true;
    }


// Private Data Members
private:


    /// Startup file name.
    wxFileName m_initFileName;

};

/* ************************************************************************ */

DECLARE_APP(MainApp)
IMPLEMENT_APP(MainApp)

/* ************************************************************************ */

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
#include "MainFrame.h"

// wxWidgets
#include <wx/filedlg.h>
#include <wx/msgdlg.h>
#include <wx/config.h>
#include <wx/wfstream.h>
#include <wx/txtstrm.h>
#include <wx/image.h>
#include <wx/sstream.h>

// Simulator
#include "core/Log.hpp"

// Loaders
#include "loaders/xml/SimulationLoader.hpp"

// GUI
#include "AboutDialog.h"
#include "PluginsDialog.h"

/* ************************************************************************ */

MainFrame::MainFrame(wxWindow* parent)
    : MainFrameBaseClass(parent)
    , m_simulatorThread(m_glCanvasView, new loader::xml::SimulationLoader())
    , m_logger(this)
{
    // Register logger into Log
    Log::setOutput(&m_logger);
    Log::setError(&m_logger);

    std::cout.rdbuf(&m_logger.GetBuffer());
    std::cerr.rdbuf(&m_logger.GetBuffer());

    m_fileHistory.UseMenu(m_menuFileRecent);

    // Set simulator
    wxASSERT(m_glCanvasView);
    m_glCanvasView->SetSimulator(&m_simulatorThread);

    // Bind events
    Bind(wxEVT_MENU, &MainFrame::OnFileOpenRecent, this, wxID_FILE1, wxID_FILE9);
    Bind(EVT_ERROR, &MainFrame::OnSimulationError, this);
    Bind(REPORT_FPS, &MainFrame::OnRenderTime, this);
    Bind(EVT_LOG, &MainFrame::OnLogMessage, this);

    // Initialize subwindows
    InitCodeViewer();

    {
        int widths[] = {-1, 150};
        m_statusBar->SetStatusWidths(sizeof(widths) / sizeof(widths[0]), widths);
    }

    // Load configuration
    LoadConfig();
}

/* ************************************************************************ */

MainFrame::~MainFrame()
{
    // Store current file
    StoreCurrentFileToHistory();

    // Store configuration
    StoreConfig();

    Log::setOutput(&std::cout);
    Log::setError(&std::cerr);
}

/* ************************************************************************ */

void MainFrame::OnFileNew(wxCommandEvent& event)
{
    // Store current file
    StoreCurrentFileToHistory();

    m_fileName.Clear();
    m_stcCode->ClearAll();
    m_simulatorThread.SendNew();
}

/* ************************************************************************ */

void MainFrame::OnFileOpen(wxCommandEvent& event)
{
    const wxString selection = wxFileSelector(
        wxFileSelectorPromptStr,
        wxEmptyString,
        wxEmptyString,
        wxEmptyString,
        "XML (*.xml)|*.xml",
        wxFD_OPEN | wxFD_FILE_MUST_EXIST
    );

    if (selection.IsEmpty())
        return;

    // Load file
    LoadFile(selection);
}

/* ************************************************************************ */

void MainFrame::OnFileSave(wxCommandEvent& event)
{
    if (!m_fileName.IsOk())
    {
        wxString selection = wxFileSelector(
            wxFileSelectorPromptStr,
            wxEmptyString,
            wxEmptyString,
            wxEmptyString,
            "XML (*.xml)|*.xml",
            wxFD_SAVE | wxFD_OVERWRITE_PROMPT
        );

        if (selection.IsEmpty())
            return;

        m_fileName = selection;
    }

    m_stcCode->SaveFile(m_fileName.GetFullPath());
}

/* ************************************************************************ */

void MainFrame::OnFileSaveAs(wxCommandEvent& event)
{
    wxString selection = wxFileSelector(
        wxFileSelectorPromptStr,
        wxEmptyString,
        wxEmptyString,
        wxEmptyString,
        "XML (*.xml)|*.xml",
        wxFD_SAVE | wxFD_OVERWRITE_PROMPT
    );

    if (selection.IsEmpty())
        return;

    m_fileName = selection;
    m_stcCode->SaveFile(m_fileName.GetFullPath());
}

/* ************************************************************************ */

void MainFrame::OnFileOpenRecent(wxCommandEvent& event)
{
    LoadFile(m_fileHistory.GetHistoryFile(event.GetId() - wxID_FILE1));
}

/* ************************************************************************ */

void MainFrame::OnFileExit(wxCommandEvent& event)
{
    wxUnusedVar(event);
    Close();
}

/* ************************************************************************ */

void MainFrame::OnViewReset(wxCommandEvent& event)
{
    m_glCanvasView->ViewReset();
}

/* ************************************************************************ */

void MainFrame::OnViewCodeCheck(wxCommandEvent& event)
{
    static int last_pos = 0;

    if (!event.IsChecked())
    {
        last_pos = UnsplitCode();
    }
    else
    {
        SplitCode(last_pos);
    }
}

/* ************************************************************************ */

void MainFrame::OnViewCodeChecked(wxUpdateUIEvent& event)
{
    event.Check(m_splitterTop->IsSplit());
}

/* ************************************************************************ */

void MainFrame::OnViewLogCheck(wxCommandEvent& event)
{
    static int last_pos = 0;

    if (!event.IsChecked())
    {
        last_pos = UnsplitLog();
    }
    else
    {
        SplitLog(last_pos);
    }
}

/* ************************************************************************ */

void MainFrame::OnViewLogChecked(wxUpdateUIEvent& event)
{
    event.Check(m_splitterMain->IsSplit());
}

/* ************************************************************************ */

void MainFrame::OnSimulationStart(wxCommandEvent& event)
{
    // Use current simulation code
    wxASSERT(m_stcCode);
    if (m_stcCode->IsModified() || m_isModified)
    {
        LoadText(m_stcCode->GetText());
        m_isModified = false;
    }

    // Start simulation
    m_simulatorThread.SendStart();
}

/* ************************************************************************ */

void MainFrame::OnSimulationPause(wxCommandEvent& event)
{
    m_simulatorThread.SendStop();
}

/* ************************************************************************ */

void MainFrame::OnSimulationStep(wxCommandEvent& event)
{
    m_simulatorThread.SendStep();
}

/* ************************************************************************ */

void MainFrame::OnSimulationRestart(wxCommandEvent& event)
{
    // Create simulation from updated code
    wxASSERT(m_stcCode);
    m_simulatorThread.SendLoad(m_stcCode->GetText());

    // Clear output log
    wxASSERT(m_textCtrlLog);
    m_textCtrlLog->Clear();
}

/* ************************************************************************ */

void MainFrame::OnSimulationScreenshot(wxCommandEvent& event)
{
    if (!m_simulatorThread.GetSimulation())
    {
        event.Skip();
        return;
    }

    // "BMP files (*.bmp)|*.bmp|GIF files (*.gif)|*.gif"
    wxString wildcard;

    for (const auto& obj : wxImage::GetHandlers())
    {
        auto handler = wxDynamicCast(obj, wxImageHandler);
        wildcard << handler->GetName() <<  " "
            "(*." << handler->GetExtension();

        for (const auto& ext : handler->GetAltExtensions())
            wildcard << ", *." << ext;

        wildcard << ")|"
            "*." << handler->GetExtension();

        for (const auto& ext : handler->GetAltExtensions())
            wildcard << ";*." << ext;

        wildcard << "|";
    }

    // Remove last |
    wildcard.RemoveLast();

    const wxString selection = wxFileSelector(
        wxFileSelectorPromptStr,
        wxEmptyString,
        "screenshot.png",
        wxEmptyString,
        wildcard, //"PNG (*.png)|*.png",
        wxFD_SAVE | wxFD_OVERWRITE_PROMPT
    );

    if (selection.IsEmpty())
        return;

    // Get pixel data
    auto data = m_glCanvasView->GetRenderContext().getData();

    // Create image
    wxImage image(wxSize(data.size.getWidth(), data.size.getHeight()),
        const_cast<unsigned char*>(data.data.data()), true);

    // Store image
    if (!image.SaveFile(selection))
    {
        wxMessageBox(wxString::Format(_("Unable to save screenshot as '%s'!"), selection),
            wxMessageBoxCaptionStr, wxOK | wxCENTER | wxICON_ERROR, this
        );
    }
}

/* ************************************************************************ */

void MainFrame::OnHelpPlugins(wxCommandEvent& event)
{
    PluginsDialog(this).ShowModal();
}

/* ************************************************************************ */

void MainFrame::OnHelpAbout(wxCommandEvent& event)
{
    AboutDialog(this).ShowModal();
}

/* ************************************************************************ */

void MainFrame::OnSimulationRunningUpdateUi(wxUpdateUIEvent& event)
{
    event.Enable(m_simulatorThread.isRunning());
}

/* ************************************************************************ */

void MainFrame::OnSimulationNotRunningUpdateUi(wxUpdateUIEvent& event)
{
    event.Enable(!m_simulatorThread.isRunning());
}

/* ************************************************************************ */

void MainFrame::OnSimulationError(wxCommandEvent& event)
{
    wxMessageBox(event.GetString(), wxMessageBoxCaptionStr, wxOK | wxCENTER | wxICON_ERROR, this);
}

/* ************************************************************************ */

void MainFrame::OnRenderTime(wxCommandEvent& event)
{
    wxASSERT(m_statusBar);
    m_statusBar->SetStatusText(wxString::Format("%d FPS", event.GetInt()), 1);
}

/* ************************************************************************ */

void MainFrame::OnCodeUpdateUi(wxUpdateUIEvent& event)
{
    event.Enable(!m_simulatorThread.isRunning());
}

/* ************************************************************************ */

void MainFrame::OnLogMessage(wxCommandEvent& event) noexcept
{
    // Append text
    wxASSERT(m_textCtrlLog);
    m_textCtrlLog->AppendText(event.GetString());
}

/* ************************************************************************ */

void MainFrame::InitCodeViewer() noexcept
{
    wxASSERT(m_stcCode);
    wxFont font = m_stcCode->GetFont();
    font.SetFamily(wxFONTFAMILY_TELETYPE);
    m_stcCode->StyleSetForeground(wxSTC_H_DOUBLESTRING,     wxColour(255,0,0));
    m_stcCode->StyleSetForeground(wxSTC_H_SINGLESTRING,     wxColour(255,0,0));
    m_stcCode->StyleSetForeground(wxSTC_H_ENTITY,           wxColour(255,0,0));
    m_stcCode->StyleSetForeground(wxSTC_H_TAG,              wxColour(0,150,0));
    m_stcCode->StyleSetForeground(wxSTC_H_TAGUNKNOWN,       wxColour(0,150,0));
    m_stcCode->StyleSetForeground(wxSTC_H_ATTRIBUTE,        wxColour(0,0,150));
    m_stcCode->StyleSetForeground(wxSTC_H_ATTRIBUTEUNKNOWN, wxColour(0,0,150));
    m_stcCode->StyleSetForeground(wxSTC_H_COMMENT,          wxColour(150,150,150));
}

/* ************************************************************************ */

void MainFrame::LoadFile(const wxFileName& path)
{
    if (!path.Exists())
    {
        wxMessageBox(wxString::Format(_("File '%s' not found!"), path.GetFullPath()),
            wxMessageBoxCaptionStr, wxOK | wxCENTER | wxICON_ERROR, this
        );

        return;
    }

    // Store last file
    StoreCurrentFileToHistory();

    m_fileName = path;

    wxFileInputStream input(m_fileName.GetFullPath());
    wxStringOutputStream text;
    input.Read(text);

    // Load source code to simulator
    LoadText(text.GetString());
}

/* ************************************************************************ */

void MainFrame::LoadText(const wxString& code)
{
    // Load text to editor
    wxASSERT(m_stcCode);
    m_stcCode->SetText(code);
    m_stcCode->SetModified(false);

    // Create simulation from updated code
    m_simulatorThread.SendLoad(code);

    // Clear output log
    wxASSERT(m_textCtrlLog);
    m_textCtrlLog->Clear();
}

/* ************************************************************************ */

void MainFrame::LoadConfig()
{
    wxConfigBase* config = wxConfig::Get();

    m_fileHistory.Load(*config);

    // Get sash positions
    int pos;
    if (config->Read("sash-main", &pos))
        m_splitterMain->SetSashPosition(pos);

    if (config->Read("sash-top", &pos))
        m_splitterTop->SetSashPosition(pos);

    int flag;
    if (config->Read("show-code", &flag) && !flag)
        UnsplitCode();

    if (config->Read("show-log", &flag) && !flag)
        UnsplitLog();
}

/* ************************************************************************ */

void MainFrame::StoreConfig()
{
    wxConfigBase* config = wxConfig::Get();

    m_fileHistory.Save(*config);

    // Store sash position
    config->Write("sash-main", m_splitterMain->GetSashPosition());
    config->Write("sash-top", m_splitterTop->GetSashPosition());

    // Display flags
    config->Write("show-code", m_menuItemViewCode->IsChecked());
    config->Write("show-log", m_menuItemViewLog->IsChecked());
}

/* ************************************************************************ */

void MainFrame::StoreCurrentFileToHistory()
{
    if (m_fileName.IsOk())
        m_fileHistory.AddFileToHistory(m_fileName.GetFullPath());
}

/* ************************************************************************ */

void MainFrame::SplitLog(int pos)
{
    m_splitterMain->SplitHorizontally(m_splitterPageTop, m_splitterPageBottom, pos);
}

/* ************************************************************************ */

void MainFrame::SplitCode(int pos)
{
    m_splitterTop->SplitVertically(m_splitterPageView, m_splitterPageCode, pos);
}

/* ************************************************************************ */

int MainFrame::UnsplitLog()
{
    int pos = m_splitterMain->GetSashPosition();
    m_splitterMain->Unsplit(m_splitterPageBottom);

    return pos;
}

/* ************************************************************************ */

int MainFrame::UnsplitCode()
{
    int pos = m_splitterTop->GetSashPosition();
    m_splitterTop->Unsplit(m_splitterPageCode);

    return pos;
}

/* ************************************************************************ */

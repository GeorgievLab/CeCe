#include "MainFrame.h"

// wxWidgets
#include <wx/aboutdlg.h>
#include <wx/filedlg.h>
#include <wx/msgdlg.h>
#include <wx/config.h>
#include <wx/wfstream.h>
#include <wx/txtstrm.h>

// Simulator
#include "parser/SimulationFactory.hpp"

// GUI
#ifdef PNG_SUPPORT
#include "PngImage.h"
#endif

/* ************************************************************************ */

/**
 * @brief Converts string into wxPoint.
 *
 * @param str   Source string.
 * @param point Result point.
 *
 * @return If conversion was successful.
 */
inline bool wxFromString(const wxString& str, wxPoint*& point)
{
    return sscanf(str.c_str(), "%d:%d", &(point->x), &(point->y)) == 2;
}

/* ************************************************************************ */

/**
 * @brief Converts string into wxSize.
 *
 * @param str  Source string.
 * @param size Result size.
 *
 * @return If conversion was successful.
 */
inline bool wxFromString(const wxString& str, wxSize*& size)
{
    int width, height;
    if (sscanf(str.c_str(), "%d:%d", &width, &height) != 2)
        return false;

    size->Set(width, height);
    return true;
}

/* ************************************************************************ */

/**
 * @brief Converts wxPoint into string.
 *
 * @param point Source point.
 *
 * @return Result string.
 */
inline wxString wxToString(const wxPoint& point)
{
    return wxString::Format("%d:%d", point.x, point.y);
}

/* ************************************************************************ */

/**
 * @brief Converts wxSize into string.
 *
 * @param size Source size.
 *
 * @return Result string.
 */
inline wxString wxToString(const wxSize& size)
{
    return wxString::Format("%d:%d", size.GetWidth(), size.GetHeight());
}

/* ************************************************************************ */

MainFrame::MainFrame(wxWindow* parent)
    : MainFrameBaseClass(parent)
    , m_simulatorThread(m_glCanvasView, new parser::SimulationFactory())
{
    m_fileHistory.UseMenu(m_menuFileRecent);

    // Set simulator
    m_glCanvasView->SetSimulator(m_simulatorThread.GetSimulator(), m_simulatorThread.GetMutex());

    Bind(wxEVT_MENU, &MainFrame::OnFileOpenRecent, this, wxID_FILE1, wxID_FILE9);
    Bind(EVT_ERROR, &MainFrame::OnSimulationError, this);
    Bind(REPORT_FPS, &MainFrame::OnRenderTime, this);

    LoadConfig();

    {
        int widths[] = {-1, 150};
        m_statusBar->SetStatusWidths(sizeof(widths) / sizeof(widths[0]), widths);
    }
}

/* ************************************************************************ */

MainFrame::~MainFrame()
{
    if (m_fileName.IsOk())
        m_fileHistory.AddFileToHistory(m_fileName.GetFullPath());

    StoreConfig();
}

/* ************************************************************************ */

void MainFrame::OnExit(wxCommandEvent& event)
{
    wxUnusedVar(event);
    Close();
}

/* ************************************************************************ */

void MainFrame::OnAbout(wxCommandEvent& event)
{
    wxUnusedVar(event);
    wxAboutDialogInfo info;
    info.SetCopyright(_("2015"));
    info.SetLicence(_("TBA"));
    info.SetDescription(_("Cell Simulator"));
    info.AddDeveloper(wxT("Jiří Fatka"));
    info.AddDeveloper(wxT("Hynek Kasl"));
    ::wxAboutBox(info);
}

/* ************************************************************************ */

void MainFrame::OnSimulationUpdate(wxThreadEvent& evt)
{
    m_glCanvasView->Update();
}

/* ************************************************************************ */

void MainFrame::OnSimulationError(wxCommandEvent& evt)
{
    wxMessageBox(evt.GetString(), wxMessageBoxCaptionStr, wxOK | wxCENTER | wxICON_ERROR, this);
}

/* ************************************************************************ */

void MainFrame::OnRenderTime(wxCommandEvent& evt)
{
    int fps = evt.GetInt();

    m_statusBar->SetStatusText(
        wxString::Format("%d FPS", fps),
        1
    );
}

/* ************************************************************************ */

void MainFrame::OnFileNew(wxCommandEvent& event)
{
    if (m_fileName.IsOk())
        m_fileHistory.AddFileToHistory(m_fileName.GetFullPath());

    m_fileName.Clear();
}

/* ************************************************************************ */

void MainFrame::OnFileOpen(wxCommandEvent& event)
{
    wxString selection = wxFileSelector(
        wxFileSelectorPromptStr,
        wxEmptyString,
        wxEmptyString,
        wxEmptyString,
        "XML (*.xml)|*.xml",
        wxFD_OPEN | wxFD_FILE_MUST_EXIST
    );

    if (selection.IsEmpty())
        return;

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

    // TODO save file
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

    LoadFile(selection);
}

/* ************************************************************************ */

void MainFrame::OnFileOpenRecent(wxCommandEvent& event)
{
    wxString selection = m_fileHistory.GetHistoryFile(event.GetId() - wxID_FILE1);

    LoadFile(selection);
}

/* ************************************************************************ */

void MainFrame::OnSimulationStart(wxCommandEvent& event)
{
    // Start simulation
    m_simulatorThread.SendStart();
}

/* ************************************************************************ */

void MainFrame::OnSimulationStop(wxCommandEvent& event)
{
    m_simulatorThread.SendStop();
}

/* ************************************************************************ */

void MainFrame::OnSimulationRestart(wxCommandEvent& event)
{
    m_simulatorThread.SendRestart();
}

/* ************************************************************************ */

void MainFrame::OnSimulationStep(wxCommandEvent& event)
{
    m_simulatorThread.SendStep();
}

/* ************************************************************************ */

void MainFrame::OnSimulationNotRunningUpdateUi(wxUpdateUIEvent& event)
{
    event.Enable(!m_simulatorThread.isRunning());
}

/* ************************************************************************ */

void MainFrame::OnSimulationRunningUpdateUi(wxUpdateUIEvent& event)
{
    event.Enable(m_simulatorThread.isRunning());
}

/* ************************************************************************ */

void MainFrame::LoadFile(const wxFileName& path)
{
    if (m_fileName.IsOk())
        m_fileHistory.AddFileToHistory(m_fileName.GetFullPath());

    m_fileName = path;

    wxFileInputStream input(m_fileName.GetFullPath());
    wxTextInputStream text(input);

    wxString code;

    while (input.IsOk() && !input.Eof())
    {
        code += text.ReadLine();
    }

    LoadText(code);
}

/* ************************************************************************ */

void MainFrame::LoadText(const wxString& code)
{
    // Update world
    m_simulatorThread.SendLoad(code);
}

/* ************************************************************************ */

void MainFrame::LoadConfig()
{
    wxConfigBase* config = wxConfig::Get();

    bool maximized = false;
    if (config->Read("maximized", &maximized))
        Maximize(maximized);

    if (!maximized)
    {
        wxSize size;
        if (config->Read("size", &size))
            SetSize(size);

        wxPoint point;
        if (config->Read("position", &point))
            SetPosition(point);
    }

    m_fileHistory.Load(*config);
}

/* ************************************************************************ */

void MainFrame::StoreConfig()
{
    wxConfigBase* config = wxConfig::Get();

    config->Write("maximized", IsMaximized());

    if (!IsMaximized())
    {
        config->Write("size", GetSize());
        config->Write("position", GetPosition());
    }

    m_fileHistory.Save(*config);
}

/* ************************************************************************ */

void MainFrame::OnViewReset(wxCommandEvent& event)
{
    m_glCanvasView->ViewReset();
}

/* ************************************************************************ */

void MainFrame::OnViewGrid(wxCommandEvent& event)
{
    m_glCanvasView->SetViewGrid(m_menuItemViewGrid->IsChecked());
}

/* ************************************************************************ */

void MainFrame::OnViewVelocity(wxCommandEvent& event)
{
    m_glCanvasView->SetViewVelocity(m_menuItemViewVelocity->IsChecked());
}

/* ************************************************************************ */

void MainFrame::OnViewGridUpdateUi(wxUpdateUIEvent& event)
{
    event.Check(m_glCanvasView->IsViewGrid());
}

/* ************************************************************************ */

void MainFrame::OnViewVelocityUpdateUi(wxUpdateUIEvent& event)
{
    event.Check(m_glCanvasView->IsViewVelocity());
}

/* ************************************************************************ */

void MainFrame::OnSimulationScreenshot(wxCommandEvent& event)
{
    wxString selection = wxFileSelector(
        wxFileSelectorPromptStr,
        wxEmptyString,
        "screenshot.png",
        wxEmptyString,
        "PNG (*.png)|*.png",
        wxFD_SAVE | wxFD_OVERWRITE_PROMPT
    );

    if (selection.IsEmpty())
        return;

    auto& renderer = m_glCanvasView->GetRenderer();

    // Get image data
    auto data = renderer.getData();
#ifdef PNG_SUPPORT
    if (!SavePNG(wxFileName::FileName(selection), data.first, data.second.getWidth(), data.second.getHeight()))
#endif
    {
        wxMessageBox("Unable to save image", wxMessageBoxCaptionStr, wxOK | wxCENTER | wxICON_ERROR, this);
    }
}

/* ************************************************************************ */

void MainFrame::OnViewInterpolate(wxCommandEvent& event)
{
    m_glCanvasView->SetViewInterpolation(m_menuItemViewInterpolate->IsChecked());
}

/* ************************************************************************ */

void MainFrame::OnViewInterpolateUpdateUi(wxUpdateUIEvent& event)
{
    event.Check(m_glCanvasView->IsViewInterpolation());
}

/* ************************************************************************ */

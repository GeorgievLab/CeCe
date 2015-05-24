#include "MainFrame.h"

// wxWidgets
#include <wx/aboutdlg.h>
#include <wx/filedlg.h>
#include <wx/msgdlg.h>
#include <wx/config.h>
#include <wx/wfstream.h>
#include <wx/txtstrm.h>

// Simulator
#include "core/Log.hpp"
#include "parser-xml/SimulationFactory.hpp"

// GUI
#if ENABLE_SCREENSHOT
#include <Magick++.h>
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
    , m_simulatorThread(m_glCanvasView, new parser::xml::SimulationFactory())
    , m_logger(this)
{
    // Register logger into Log
	core::Log::setOutput(&m_logger);
	core::Log::setError(&m_logger);

    m_fileHistory.UseMenu(m_menuFileRecent);

    // Set simulator
    wxASSERT(m_glCanvasView);
    m_glCanvasView->SetSimulator(m_simulatorThread.GetSimulator(), m_simulatorThread.GetMutex());

    // Bind events
    Bind(wxEVT_MENU, &MainFrame::OnFileOpenRecent, this, wxID_FILE1, wxID_FILE9);
    Bind(EVT_ERROR, &MainFrame::OnSimulationError, this);
    Bind(REPORT_FPS, &MainFrame::OnRenderTime, this);
    Bind(EVT_LOG, &MainFrame::OnLogMessage, this);

    // Load configuration
    LoadConfig();

    {
        int widths[] = {-1, 150};
        m_statusBar->SetStatusWidths(sizeof(widths) / sizeof(widths[0]), widths);
    }
}

/* ************************************************************************ */

MainFrame::~MainFrame()
{
    // Store current file
    StoreCurrentFileToHistory();

    // Store configuration
    StoreConfig();

	core::Log::setOutput(&std::cout);
    core::Log::setError(&std::cerr);
}

/* ************************************************************************ */

void MainFrame::OnFileNew(wxCommandEvent& event)
{
    // Store current file
    StoreCurrentFileToHistory();

    m_fileName.Clear();
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

    // TODO: save file
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

    // TODO: save file
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
    if (m_stcCode->IsModified())
    {
        m_textCtrlLog->Clear();
        m_simulatorThread.SendLoad(m_stcCode->GetText());
        m_stcCode->SetModified(false);
    }

    // Start simulation
    m_simulatorThread.SendStart();
}

/* ************************************************************************ */

void MainFrame::OnSimulationStop(wxCommandEvent& event)
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
    wxASSERT(m_stcCode);

    // Reload source
    LoadText(m_stcCode->GetText());
}

/* ************************************************************************ */

void MainFrame::OnSimulationScreenshot(wxCommandEvent& event)
{
#if ENABLE_SCREENSHOT
    if (!m_simulatorThread.GetSimulator())
    {
        event.Skip();
        return;
    }

    const wxString selection = wxFileSelector(
        wxFileSelectorPromptStr,
        wxEmptyString,
        "screenshot.png",
        wxEmptyString,
        "PNG (*.png)|*.png",
        wxFD_SAVE | wxFD_OVERWRITE_PROMPT
    );

    if (selection.IsEmpty())
        return;

    // Get pixel data
    auto data = m_simulatorThread.GetSimulator()->getRenderContext().getData();
    const auto& imageData = data.first;
    auto imageSize = data.second;

    // Create image
    Magick::Image image(Magick::Geometry(imageSize.getWidth(), imageSize.getHeight()), Magick::Color());

    for (unsigned i = 0; i < imageSize.getWidth(); ++i)
    {
        for (unsigned j = 0; j < imageSize.getHeight(); ++j)
        {
            auto base = 3 * (i + imageSize.getWidth() * j);
            image.pixelColor(i, j, Magick::ColorRGB(
                imageData[base] / 255.f,
                imageData[base + 1] / 255.f,
                imageData[base + 2] / 255.f
            ));
        }
    }

    // Store image
    image.write(selection.c_str().AsChar());
#endif
}

/* ************************************************************************ */

void MainFrame::OnSimulationScreenshotUpdateUi(wxUpdateUIEvent& event)
{
#if ENABLE_SCREENSHOT
    event.Enable(true);
#else
    event.Enable(false);
#endif
}

/* ************************************************************************ */

void MainFrame::OnHelpAbout(wxCommandEvent& event)
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

void MainFrame::OnLogMessage(wxCommandEvent& event) NOEXCEPT
{
    // Append text
    m_textCtrlLog->AppendText(event.GetString());
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
    wxTextInputStream text(input);

    wxString code;

    while (input.IsOk() && !input.Eof())
    {
        code += text.ReadLine();
        code += "\n";
    }

    // Load source code to simulator
    LoadText(code);

    // Load text to editor
    m_stcCode->SetText(code);
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

    config->Write("maximized", IsMaximized());

    if (!IsMaximized())
    {
        config->Write("size", GetSize());
        config->Write("position", GetPosition());
    }

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

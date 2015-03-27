#include "MainFrame.h"

// wxWidgets
#include <wx/aboutdlg.h>
#include <wx/filedlg.h>
#include <wx/msgdlg.h>
#include <wx/config.h>

// Simulator
#include "parser/WorldFactory.hpp"

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
    , m_simulatorThread(m_glCanvasView, new parser::WorldFactory())
{
    m_fileHistory.UseMenu(m_menuFileRecent);

    // Set simulator
    m_glCanvasView->SetSimulator(m_simulatorThread.GetSimulator(), m_simulatorThread.GetMutex());

    wxFont font = m_stcSource->GetFont();
    font.SetFamily(wxFONTFAMILY_TELETYPE);
    m_stcSource->StyleSetFont(wxSTC_C_DEFAULT, font);
    m_stcSource->StyleSetFont(wxSTC_C_CHARACTER, font);
    m_stcSource->StyleSetFont(wxSTC_C_COMMENT, font);
    m_stcSource->StyleSetFont(wxSTC_C_COMMENTDOC, font);
    m_stcSource->StyleSetFont(wxSTC_C_COMMENTDOCKEYWORD, font);
    m_stcSource->StyleSetFont(wxSTC_C_COMMENTDOCKEYWORDERROR, font);
    m_stcSource->StyleSetFont(wxSTC_C_COMMENTLINE, font);
    m_stcSource->StyleSetFont(wxSTC_C_COMMENTLINEDOC, font);
    m_stcSource->StyleSetFont(wxSTC_C_GLOBALCLASS, font);
    m_stcSource->StyleSetFont(wxSTC_C_HASHQUOTEDSTRING, font);
    m_stcSource->StyleSetFont(wxSTC_C_IDENTIFIER, font);
    m_stcSource->StyleSetFont(wxSTC_C_NUMBER, font);
    m_stcSource->StyleSetFont(wxSTC_C_OPERATOR, font);
    m_stcSource->StyleSetFont(wxSTC_C_REGEX, font);
    m_stcSource->StyleSetFont(wxSTC_C_STRING, font);
    m_stcSource->StyleSetFont(wxSTC_C_STRINGEOL, font);
    m_stcSource->StyleSetFont(wxSTC_C_STRINGRAW, font);
    m_stcSource->StyleSetFont(wxSTC_C_WORD, font);
    m_stcSource->StyleSetFont(wxSTC_C_WORD2, font);

    // https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Lexical_grammar
    m_stcSource->StyleSetForeground(wxSTC_C_IDENTIFIER, *wxBLACK);
    m_stcSource->StyleSetForeground(wxSTC_C_WORD, wxColor(0, 0x77, 0xAA));
    m_stcSource->StyleSetForeground(wxSTC_C_WORD2, wxColor(0x99, 0x99, 0x99));
    m_stcSource->StyleSetForeground(wxSTC_C_STRING, wxColor(0x66, 0x99, 0));
    m_stcSource->StyleSetForeground(wxSTC_C_COMMENT, wxColor(0x70, 0x80, 0x90));
    m_stcSource->StyleSetForeground(wxSTC_C_COMMENTDOC, wxColor(0x70, 0x80, 0x90));
    m_stcSource->StyleSetForeground(wxSTC_C_COMMENTDOCKEYWORD, wxColor(0x70, 0x80, 0x90));
    m_stcSource->StyleSetForeground(wxSTC_C_COMMENTDOCKEYWORDERROR, wxColor(0x70, 0x80, 0x90));
    m_stcSource->StyleSetForeground(wxSTC_C_COMMENTLINE, wxColor(0x70, 0x80, 0x90));
    m_stcSource->StyleSetForeground(wxSTC_C_COMMENTLINEDOC, wxColor(0x70, 0x80, 0x90));
    m_stcSource->StyleSetForeground(wxSTC_C_NUMBER, wxColor(0x99, 0, 0x55));

    // ECMAScript 6
    m_stcSource->SetKeyWords(0,
        "abstract arguments boolean break byte case catch char class const "
        "continue debugger default delete do "
        "double else enum eval export "
        "extends false final finally float "
        "for function goto if implements "
        "import in instanceof int interface "
        "let long native new null "
        "package private protected public return "
        "short static super switch synchronized "
        "this throw throws transient true "
        "try typeof var void volatile "
        "while with yield");

    m_stcSource->SetKeyWords(1, "Array Date eval function hasOwnProperty "
        "Infinity isFinite isNaN isPrototypeOf length "
        "Math NaN name Number Object "
        "prototype String toString undefined valueOf");

    //Bind(EVT_SIMULATION_UPDATE, &MainFrame::OnSimulationUpdate, this);

    Bind(wxEVT_MENU, &MainFrame::OnFileOpenRecent, this, wxID_FILE1, wxID_FILE9);
    Bind(EVT_ERROR, &MainFrame::OnSimulationError, this);
    Bind(EVT_LOG, &MainFrame::OnSimulationLog, this);
    Bind(REPORT_RENDER_TIME, &MainFrame::OnRenderTime, this);

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
    info.SetCopyright(_("2014"));
    info.SetLicence(_("GPL v2 or later"));
    info.SetDescription(_("Yeast Simulator"));
    info.AddDeveloper(wxT("Jiří Fatka"));
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

void MainFrame::OnSimulationLog(wxCommandEvent& evt)
{
    m_textCtrlConsole->AppendText(evt.GetString() + "\n");
}

/* ************************************************************************ */

void MainFrame::OnRenderTime(wxCommandEvent& evt)
{
    long time = evt.GetExtraLong();
    float fps = 0;

    if (time)
        fps = (1000000.f / time);

    m_statusBar->SetStatusText(
        wxString::Format("%.3f ms / %.3f FPS", time / 1000.f, fps),
        1
    );
}

/* ************************************************************************ */

void MainFrame::OnFileNew(wxCommandEvent& event)
{
    if (m_fileName.IsOk())
        m_fileHistory.AddFileToHistory(m_fileName.GetFullPath());

    m_stcSource->ClearAll();
    m_fileName.Clear();
    m_textCtrlConsole->Clear();
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

    m_stcSource->SaveFile(m_fileName.GetFullPath());
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
    m_textCtrlConsole->Clear();
}

/* ************************************************************************ */

void MainFrame::OnSimulationStart(wxCommandEvent& event)
{
    m_textCtrlConsole->Clear();

    wxASSERT(m_stcSource);

    // If source code is modified, load modified version
    if (m_stcSource->IsModified() || m_sourceChanged)
    {
        if (m_stcSource->IsEmpty())
        {
            wxMessageBox("Source script is empty, nothing to simulate!", wxMessageBoxCaptionStr, wxOK | wxCENTER | wxICON_ERROR, this);
            return;
        }

        LoadText(m_stcSource->GetText());

        // Source is not modified
        m_stcSource->SetModified(false);
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

void MainFrame::OnSimulationRestart(wxCommandEvent& event)
{
    m_textCtrlConsole->Clear();

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

void MainFrame::OnSourceChange(wxStyledTextEvent& event)
{
    m_sourceChanged = true;
}

/* ************************************************************************ */

void MainFrame::LoadFile(const wxFileName& path)
{
    if (m_fileName.IsOk())
        m_fileHistory.AddFileToHistory(m_fileName.GetFullPath());

    m_fileName = path;
    m_stcSource->LoadFile(path.GetFullPath());

    if (m_stcSource->IsEmpty())
    {
        wxMessageBox("Source script is empty, nothing to simulate!", wxMessageBoxCaptionStr, wxOK | wxCENTER | wxICON_ERROR, this);
        return;
    }

    LoadText(m_stcSource->GetText());
}

/* ************************************************************************ */

void MainFrame::LoadText(const wxString& code)
{
    // Update world
    m_simulatorThread.SendLoad(code);
    m_sourceChanged = false;
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
    auto flags = m_glCanvasView->GetWorld()->getRenderFlags();

    if (m_menuItemViewGrid->IsChecked())
        flags |= simulator::World::RENDER_GRID;
    else
        flags &= ~simulator::World::RENDER_GRID;

    m_glCanvasView->GetWorld()->setRenderFlags(flags);
}

/* ************************************************************************ */

void MainFrame::OnViewVelocity(wxCommandEvent& event)
{
    auto flags = m_glCanvasView->GetWorld()->getRenderFlags();

    if (m_menuItemViewVelocity->IsChecked())
        flags |= simulator::World::RENDER_VELOCITY;
    else
        flags &= ~simulator::World::RENDER_VELOCITY;

    m_glCanvasView->GetWorld()->setRenderFlags(flags);
}

/* ************************************************************************ */

#include "MainFrame.h"

// wxWidgets
#include <wx/aboutdlg.h>
#include <wx/filedlg.h>
#include <wx/msgdlg.h>

// JavaScript
#include "javascript/WorldFactory.h"

/* ************************************************************************ */

MainFrame::MainFrame(wxWindow* parent)
    : MainFrameBaseClass(parent)
    , m_simulator()
{
    // Set simulator
    m_glCanvasView->SetSimulator(&m_simulator);

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

    m_worldFactory.reset(new javascript::WorldFactory());

    //Bind(EVT_SIMULATION_UPDATE, &MainFrame::OnSimulationUpdate, this);
}

/* ************************************************************************ */

MainFrame::~MainFrame()
{
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

void MainFrame::OnFileNew(wxCommandEvent& event)
{
    m_stcSource->ClearAll();
}

/* ************************************************************************ */

void MainFrame::OnFileOpen(wxCommandEvent& event)
{
    wxString selection = wxFileSelector(
        wxFileSelectorPromptStr,
        wxEmptyString,
        wxEmptyString,
        wxEmptyString,
        "JavaScript (*.js)|*.js",
        wxFD_OPEN | wxFD_FILE_MUST_EXIST
    );

    if (selection.IsEmpty())
        return;

    m_fileName = selection;
    m_stcSource->LoadFile(selection);
}

/* ************************************************************************ */

void MainFrame::OnFileSave(wxCommandEvent& event)
{
    if (m_fileName.IsEmpty())
    {
        wxString selection = wxFileSelector(
            wxFileSelectorPromptStr,
            wxEmptyString,
            wxEmptyString,
            wxEmptyString,
            "JavaScript (*.js)|*.js",
            wxFD_SAVE | wxFD_OVERWRITE_PROMPT
        );

        if (selection.IsEmpty())
            return;

        m_fileName = selection;
    }

    m_stcSource->SaveFile(m_fileName);
}

/* ************************************************************************ */

void MainFrame::OnFileSaveAs(wxCommandEvent& event)
{
    wxString selection = wxFileSelector(
        wxFileSelectorPromptStr,
        wxEmptyString,
        wxEmptyString,
        wxEmptyString,
        "JavaScript (*.js)|*.js",
        wxFD_SAVE | wxFD_OVERWRITE_PROMPT
    );

    if (selection.IsEmpty())
        return;

    m_fileName = selection;
    m_stcSource->SaveFile(selection);
}

/* ************************************************************************ */

void MainFrame::OnSimulationStart(wxCommandEvent& event)
{
    // If source code is modified, load modified version
    if ((m_stcSource->IsModified() && !m_stcSource->IsEmpty()) || !m_simulator.getWorld())
    {
        try
        {
            // Create a new world
            m_simulator.setWorld(m_worldFactory->createWorldFromSource(m_stcSource->GetText().To8BitData().data()));

            // Source is not modified
            m_stcSource->SetModified(false);
        }
        catch (const std::exception& e)
        {
            wxMessageBox(e.what(), wxMessageBoxCaptionStr, wxOK | wxCENTER | wxICON_ERROR, this);
            return;
        }
    }
    else if (m_stcSource->IsEmpty())
    {
        wxMessageBox("Source script is empty, nothing to simulate!", wxMessageBoxCaptionStr, wxOK | wxCENTER | wxICON_ERROR, this);
        return;
    }

    wxASSERT(!m_simulator.isRunning());
    m_simulator.start();
}

/* ************************************************************************ */

void MainFrame::OnSimulationStop(wxCommandEvent& event)
{
    wxASSERT(m_simulator.isRunning());
    m_simulator.stop();
}

/* ************************************************************************ */

void MainFrame::OnSimulationRestart(wxCommandEvent& event)
{
    m_simulator.setWorld(nullptr);
    m_glCanvasView->Update();
}

/* ************************************************************************ */

void MainFrame::OnSimulationStep(wxCommandEvent& event)
{
    wxASSERT(!m_simulator.isRunning());
    m_simulator.step();
    m_glCanvasView->Update();
}

/* ************************************************************************ */

void MainFrame::OnSimulationNotRunningUpdateUi(wxUpdateUIEvent& event)
{
    event.Enable(!m_simulator.isRunning());
}

/* ************************************************************************ */

void MainFrame::OnSimulationRunningUpdateUi(wxUpdateUIEvent& event)
{
    event.Enable(m_simulator.isRunning());
}

/* ************************************************************************ */

void MainFrame::OnViewIsometric(wxCommandEvent& event)
{
    m_glCanvasView->SetProjection(CanvasWidget::Projection::Isometric);
}

/* ************************************************************************ */

void MainFrame::OnViewTop(wxCommandEvent& event)
{
    m_glCanvasView->SetProjection(CanvasWidget::Projection::Top);
}

/* ************************************************************************ */

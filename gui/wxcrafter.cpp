//////////////////////////////////////////////////////////////////////
// This file was auto-generated by codelite's wxCrafter Plugin
// wxCrafter project file: wxcrafter.wxcp
// Do not modify this file by hand!
//////////////////////////////////////////////////////////////////////

#include "wxcrafter.h"


// Declare the bitmap loading function
extern void wxC9ED9InitBitmapResources();

static bool bBitmapLoaded = false;


MainFrameBaseClass::MainFrameBaseClass(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
    : wxFrame(parent, id, title, pos, size, style)
{
    if ( !bBitmapLoaded ) {
        // We need to initialise the default bitmap handler
        wxXmlResource::Get()->AddHandler(new wxBitmapXmlHandler);
        wxC9ED9InitBitmapResources();
        bBitmapLoaded = true;
    }
    
    m_menuBar = new wxMenuBar(0);
    this->SetMenuBar(m_menuBar);
    
    m_menuFile = new wxMenu();
    m_menuBar->Append(m_menuFile, _("&File"));
    
    m_menuItemFileNew = new wxMenuItem(m_menuFile, wxID_NEW, _("&New\tCtrl+N"), wxT(""), wxITEM_NORMAL);
    m_menuFile->Append(m_menuItemFileNew);
    
    m_menuItemFileOpen = new wxMenuItem(m_menuFile, wxID_OPEN, _("&Open...\tCtrl+O"), wxT(""), wxITEM_NORMAL);
    m_menuFile->Append(m_menuItemFileOpen);
    
    m_menuItemFileSave = new wxMenuItem(m_menuFile, wxID_SAVE, _("&Save\tCtrl+S"), wxT(""), wxITEM_NORMAL);
    m_menuFile->Append(m_menuItemFileSave);
    
    m_menuItemFileSaveAs = new wxMenuItem(m_menuFile, wxID_SAVEAS, _("Save &As...\tCtrl+Shift+S"), wxT(""), wxITEM_NORMAL);
    m_menuFile->Append(m_menuItemFileSaveAs);
    
    m_menuFile->AppendSeparator();
    
    m_menuFileRecent = new wxMenu();
    m_menuFile->AppendSubMenu(m_menuFileRecent, _("Recent"));
    
    m_menuFile->AppendSeparator();
    
    m_menuItemFileExit = new wxMenuItem(m_menuFile, wxID_EXIT, _("&Exit\tCtrl+Q"), _("Quit"), wxITEM_NORMAL);
    m_menuFile->Append(m_menuItemFileExit);
    
    m_menuView = new wxMenu();
    m_menuBar->Append(m_menuView, _("&View"));
    
    m_menuItemViewReset = new wxMenuItem(m_menuView, wxID_ANY, _("Reset"), wxT(""), wxITEM_NORMAL);
    m_menuView->Append(m_menuItemViewReset);
    
    m_menuView->AppendSeparator();
    
    m_menuItemViewLog = new wxMenuItem(m_menuView, wxID_ANY, _("Log"), _("Show or hide log"), wxITEM_CHECK);
    m_menuView->Append(m_menuItemViewLog);
    m_menuItemViewLog->Check();
    
    m_menuItemViewCode = new wxMenuItem(m_menuView, wxID_ANY, _("Code"), _("Show or hide simulation code"), wxITEM_CHECK);
    m_menuView->Append(m_menuItemViewCode);
    m_menuItemViewCode->Check();
    
    m_menuSimulation = new wxMenu();
    m_menuBar->Append(m_menuSimulation, _("&Simulation"));
    
    m_menuItemSimulationStart = new wxMenuItem(m_menuSimulation, ID_START, _("&Start\tF5"), wxT(""), wxITEM_NORMAL);
    m_menuSimulation->Append(m_menuItemSimulationStart);
    
    m_menuItemSimulationStop = new wxMenuItem(m_menuSimulation, ID_STOP, _("Sto&p\tF6"), wxT(""), wxITEM_NORMAL);
    m_menuSimulation->Append(m_menuItemSimulationStop);
    
    m_menuItemSimulationStep = new wxMenuItem(m_menuSimulation, ID_STEP, _("S&tep\tF7"), wxT(""), wxITEM_NORMAL);
    m_menuSimulation->Append(m_menuItemSimulationStep);
    
    m_menuItemSimulationRestart = new wxMenuItem(m_menuSimulation, ID_RESTART, _("&Restart\tF8"), wxT(""), wxITEM_NORMAL);
    m_menuSimulation->Append(m_menuItemSimulationRestart);
    
    m_menuSimulation->AppendSeparator();
    
    m_menuItemSimulationScreenshot = new wxMenuItem(m_menuSimulation, wxID_ANY, _("Screenshot"), wxT(""), wxITEM_NORMAL);
    m_menuSimulation->Append(m_menuItemSimulationScreenshot);
    
    m_menuHelp = new wxMenu();
    m_menuBar->Append(m_menuHelp, _("&Help"));
    
    m_menuItemHelpModules = new wxMenuItem(m_menuHelp, wxID_ANY, _("Build-in modules..."), _("Show a list of build-in modules"), wxITEM_NORMAL);
    m_menuHelp->Append(m_menuItemHelpModules);
    
    m_menuHelp->AppendSeparator();
    
    m_menuItemAbout = new wxMenuItem(m_menuHelp, wxID_ABOUT, _("&About..."), wxT(""), wxITEM_NORMAL);
    m_menuHelp->Append(m_menuItemAbout);
    
    m_mainToolbar = this->CreateToolBar(wxTB_TEXT|wxTB_HORIZONTAL|wxTB_FLAT, wxID_ANY);
    m_mainToolbar->SetToolBitmapSize(wxSize(24,24));
    
    m_mainToolbar->AddTool(ID_START, _("Start"), wxXmlResource::Get()->LoadBitmap(wxT("play-24")), wxNullBitmap, wxITEM_NORMAL, wxT(""), wxT(""), NULL);
    
    m_mainToolbar->AddTool(ID_STOP, _("Stop"), wxXmlResource::Get()->LoadBitmap(wxT("stop-24")), wxNullBitmap, wxITEM_NORMAL, wxT(""), wxT(""), NULL);
    
    m_mainToolbar->AddTool(ID_STEP, _("Step"), wxXmlResource::Get()->LoadBitmap(wxT("step-24")), wxNullBitmap, wxITEM_NORMAL, wxT(""), wxT(""), NULL);
    
    m_mainToolbar->AddTool(ID_RESTART, _("Restart"), wxXmlResource::Get()->LoadBitmap(wxT("restart-24")), wxNullBitmap, wxITEM_NORMAL, wxT(""), wxT(""), NULL);
    m_mainToolbar->Realize();
    
    wxBoxSizer* boxSizerMain = new wxBoxSizer(wxHORIZONTAL);
    this->SetSizer(boxSizerMain);
    
    m_splitterMain = new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition, wxSize(-1,-1), wxSP_3D);
    m_splitterMain->SetSashGravity(0.8);
    m_splitterMain->SetMinimumPaneSize(10);
    
    boxSizerMain->Add(m_splitterMain, 1, wxEXPAND, 5);
    
    m_splitterPageTop = new wxPanel(m_splitterMain, wxID_ANY, wxDefaultPosition, wxSize(-1,-1), wxTAB_TRAVERSAL);
    
    wxBoxSizer* boxSizerTop = new wxBoxSizer(wxVERTICAL);
    m_splitterPageTop->SetSizer(boxSizerTop);
    
    m_splitterTop = new wxSplitterWindow(m_splitterPageTop, wxID_ANY, wxDefaultPosition, wxSize(-1,-1), wxSP_3D);
    m_splitterTop->SetSashGravity(0.8);
    m_splitterTop->SetMinimumPaneSize(10);
    
    boxSizerTop->Add(m_splitterTop, 1, wxLEFT|wxRIGHT|wxEXPAND, 5);
    
    m_splitterPageView = new wxPanel(m_splitterTop, wxID_ANY, wxDefaultPosition, wxSize(-1,-1), wxTAB_TRAVERSAL);
    
    wxBoxSizer* boxSizerView = new wxBoxSizer(wxVERTICAL);
    m_splitterPageView->SetSizer(boxSizerView);
    
    int *m_glCanvasViewAttr = NULL;
    m_glCanvasView = new CanvasWidget(m_splitterPageView, wxID_ANY, m_glCanvasViewAttr, wxDefaultPosition, wxSize(-1,-1), wxFULL_REPAINT_ON_RESIZE);
    wxDELETEA( m_glCanvasViewAttr );
    
    boxSizerView->Add(m_glCanvasView, 1, wxEXPAND, 5);
    
    m_splitterPageCode = new wxPanel(m_splitterTop, wxID_ANY, wxDefaultPosition, wxSize(-1,-1), wxTAB_TRAVERSAL);
    m_splitterTop->SplitVertically(m_splitterPageView, m_splitterPageCode, 0);
    
    wxBoxSizer* boxSizerCode = new wxBoxSizer(wxVERTICAL);
    m_splitterPageCode->SetSizer(boxSizerCode);
    
    m_stcCode = new wxStyledTextCtrl(m_splitterPageCode, wxID_ANY, wxDefaultPosition, wxSize(-1,-1), 0);
    // Configure the fold margin
    m_stcCode->SetMarginType     (4, wxSTC_MARGIN_SYMBOL);
    m_stcCode->SetMarginMask     (4, wxSTC_MASK_FOLDERS);
    m_stcCode->SetMarginSensitive(4, true);
    m_stcCode->SetMarginWidth    (4, 16);
    
    m_stcCode->SetProperty(wxT("fold"),wxT("1"));
    m_stcCode->MarkerDefine(wxSTC_MARKNUM_FOLDEROPEN,    wxSTC_MARK_ARROWDOWN);
    m_stcCode->MarkerDefine(wxSTC_MARKNUM_FOLDER,        wxSTC_MARK_ARROW);
    m_stcCode->MarkerDefine(wxSTC_MARKNUM_FOLDERSUB,     wxSTC_MARK_BACKGROUND);
    m_stcCode->MarkerDefine(wxSTC_MARKNUM_FOLDERTAIL,    wxSTC_MARK_BACKGROUND);
    m_stcCode->MarkerDefine(wxSTC_MARKNUM_FOLDEREND,     wxSTC_MARK_ARROW);
    m_stcCode->MarkerDefine(wxSTC_MARKNUM_FOLDEROPENMID, wxSTC_MARK_ARROWDOWN);
    m_stcCode->MarkerDefine(wxSTC_MARKNUM_FOLDERMIDTAIL, wxSTC_MARK_BACKGROUND);
    // Configure the tracker margin
    m_stcCode->SetMarginWidth(1, 0);
    
    // Configure the symbol margin
    m_stcCode->SetMarginType (2, wxSTC_MARGIN_SYMBOL);
    m_stcCode->SetMarginMask (2, ~(wxSTC_MASK_FOLDERS));
    m_stcCode->SetMarginWidth(2, 0);
    m_stcCode->SetMarginSensitive(2, true);
    
    // Configure the line numbers margin
    int m_stcCode_PixelWidth = 4 + 5 *m_stcCode->TextWidth(wxSTC_STYLE_LINENUMBER, wxT("9"));
    m_stcCode->SetMarginType(0, wxSTC_MARGIN_NUMBER);
    m_stcCode->SetMarginWidth(0,m_stcCode_PixelWidth);
    
    // Configure the line symbol margin
    m_stcCode->SetMarginType(3, wxSTC_MARGIN_FORE);
    m_stcCode->SetMarginMask(3, 0);
    m_stcCode->SetMarginWidth(3,0);
    // Select the lexer
    m_stcCode->SetLexer(wxSTC_LEX_XML);
    // Set default font / styles
    m_stcCode->StyleClearAll();
    m_stcCode->SetWrapMode(0);
    m_stcCode->SetIndentationGuides(0);
    m_stcCode->SetKeyWords(0, wxT(""));
    m_stcCode->SetKeyWords(1, wxT(""));
    m_stcCode->SetKeyWords(2, wxT(""));
    m_stcCode->SetKeyWords(3, wxT(""));
    m_stcCode->SetKeyWords(4, wxT(""));
    
    boxSizerCode->Add(m_stcCode, 1, wxEXPAND, 5);
    
    m_splitterPageBottom = new wxPanel(m_splitterMain, wxID_ANY, wxDefaultPosition, wxSize(-1,-1), wxTAB_TRAVERSAL);
    m_splitterMain->SplitHorizontally(m_splitterPageTop, m_splitterPageBottom, 0);
    
    wxBoxSizer* boxSizerBottom = new wxBoxSizer(wxVERTICAL);
    m_splitterPageBottom->SetSizer(boxSizerBottom);
    
    m_textCtrlLog = new wxTextCtrl(m_splitterPageBottom, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(-1,-1), wxTE_READONLY|wxTE_MULTILINE);
    
    boxSizerBottom->Add(m_textCtrlLog, 1, wxLEFT|wxRIGHT|wxEXPAND, 5);
    
    m_statusBar = new wxStatusBar(this, wxID_ANY, wxSTB_DEFAULT_STYLE);
    m_statusBar->SetFieldsCount(2);
    this->SetStatusBar(m_statusBar);
    
    SetName(wxT("MainFrameBaseClass"));
    SetSizeHints(800,600);
    if ( GetSizer() ) {
         GetSizer()->Fit(this);
    }
    CentreOnParent(wxBOTH);
    // Connect events
    this->Connect(m_menuItemFileNew->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnFileNew), NULL, this);
    this->Connect(m_menuItemFileOpen->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnFileOpen), NULL, this);
    this->Connect(m_menuItemFileSave->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnFileSave), NULL, this);
    this->Connect(m_menuItemFileSaveAs->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnFileSaveAs), NULL, this);
    this->Connect(m_menuItemFileExit->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnFileExit), NULL, this);
    this->Connect(m_menuItemViewReset->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnViewReset), NULL, this);
    this->Connect(m_menuItemViewLog->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnViewLogCheck), NULL, this);
    this->Connect(m_menuItemViewLog->GetId(), wxEVT_UPDATE_UI, wxUpdateUIEventHandler(MainFrameBaseClass::OnViewLogChecked), NULL, this);
    this->Connect(m_menuItemViewCode->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnViewCodeCheck), NULL, this);
    this->Connect(m_menuItemViewCode->GetId(), wxEVT_UPDATE_UI, wxUpdateUIEventHandler(MainFrameBaseClass::OnViewCodeChecked), NULL, this);
    this->Connect(m_menuItemSimulationStart->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnSimulationStart), NULL, this);
    this->Connect(m_menuItemSimulationStart->GetId(), wxEVT_UPDATE_UI, wxUpdateUIEventHandler(MainFrameBaseClass::OnSimulationNotRunningUpdateUi), NULL, this);
    this->Connect(m_menuItemSimulationStop->GetId(), wxEVT_UPDATE_UI, wxUpdateUIEventHandler(MainFrameBaseClass::OnSimulationRunningUpdateUi), NULL, this);
    this->Connect(m_menuItemSimulationStop->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnSimulationStop), NULL, this);
    this->Connect(m_menuItemSimulationStep->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnSimulationStep), NULL, this);
    this->Connect(m_menuItemSimulationStep->GetId(), wxEVT_UPDATE_UI, wxUpdateUIEventHandler(MainFrameBaseClass::OnSimulationNotRunningUpdateUi), NULL, this);
    this->Connect(m_menuItemSimulationRestart->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnSimulationRestart), NULL, this);
    this->Connect(m_menuItemSimulationRestart->GetId(), wxEVT_UPDATE_UI, wxUpdateUIEventHandler(MainFrameBaseClass::OnSimulationNotRunningUpdateUi), NULL, this);
    this->Connect(m_menuItemSimulationScreenshot->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnSimulationScreenshot), NULL, this);
    this->Connect(m_menuItemHelpModules->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnHelpModules), NULL, this);
    this->Connect(m_menuItemAbout->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnHelpAbout), NULL, this);
    this->Connect(ID_STEP, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MainFrameBaseClass::OnSimulationStep), NULL, this);
    this->Connect(ID_RESTART, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MainFrameBaseClass::OnSimulationRestart), NULL, this);
    m_stcCode->Connect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(MainFrameBaseClass::OnCodeUpdateUi), NULL, this);
    m_stcCode->Connect(wxEVT_STC_CHANGE, wxStyledTextEventHandler(MainFrameBaseClass::OnCodeChange), NULL, this);
    
}

MainFrameBaseClass::~MainFrameBaseClass()
{
    this->Disconnect(m_menuItemFileNew->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnFileNew), NULL, this);
    this->Disconnect(m_menuItemFileOpen->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnFileOpen), NULL, this);
    this->Disconnect(m_menuItemFileSave->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnFileSave), NULL, this);
    this->Disconnect(m_menuItemFileSaveAs->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnFileSaveAs), NULL, this);
    this->Disconnect(m_menuItemFileExit->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnFileExit), NULL, this);
    this->Disconnect(m_menuItemViewReset->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnViewReset), NULL, this);
    this->Disconnect(m_menuItemViewLog->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnViewLogCheck), NULL, this);
    this->Disconnect(m_menuItemViewLog->GetId(), wxEVT_UPDATE_UI, wxUpdateUIEventHandler(MainFrameBaseClass::OnViewLogChecked), NULL, this);
    this->Disconnect(m_menuItemViewCode->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnViewCodeCheck), NULL, this);
    this->Disconnect(m_menuItemViewCode->GetId(), wxEVT_UPDATE_UI, wxUpdateUIEventHandler(MainFrameBaseClass::OnViewCodeChecked), NULL, this);
    this->Disconnect(m_menuItemSimulationStart->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnSimulationStart), NULL, this);
    this->Disconnect(m_menuItemSimulationStart->GetId(), wxEVT_UPDATE_UI, wxUpdateUIEventHandler(MainFrameBaseClass::OnSimulationNotRunningUpdateUi), NULL, this);
    this->Disconnect(m_menuItemSimulationStop->GetId(), wxEVT_UPDATE_UI, wxUpdateUIEventHandler(MainFrameBaseClass::OnSimulationRunningUpdateUi), NULL, this);
    this->Disconnect(m_menuItemSimulationStop->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnSimulationStop), NULL, this);
    this->Disconnect(m_menuItemSimulationStep->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnSimulationStep), NULL, this);
    this->Disconnect(m_menuItemSimulationStep->GetId(), wxEVT_UPDATE_UI, wxUpdateUIEventHandler(MainFrameBaseClass::OnSimulationNotRunningUpdateUi), NULL, this);
    this->Disconnect(m_menuItemSimulationRestart->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnSimulationRestart), NULL, this);
    this->Disconnect(m_menuItemSimulationRestart->GetId(), wxEVT_UPDATE_UI, wxUpdateUIEventHandler(MainFrameBaseClass::OnSimulationNotRunningUpdateUi), NULL, this);
    this->Disconnect(m_menuItemSimulationScreenshot->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnSimulationScreenshot), NULL, this);
    this->Disconnect(m_menuItemHelpModules->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnHelpModules), NULL, this);
    this->Disconnect(m_menuItemAbout->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnHelpAbout), NULL, this);
    this->Disconnect(ID_STEP, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MainFrameBaseClass::OnSimulationStep), NULL, this);
    this->Disconnect(ID_RESTART, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MainFrameBaseClass::OnSimulationRestart), NULL, this);
    m_stcCode->Disconnect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(MainFrameBaseClass::OnCodeUpdateUi), NULL, this);
    m_stcCode->Disconnect(wxEVT_STC_CHANGE, wxStyledTextEventHandler(MainFrameBaseClass::OnCodeChange), NULL, this);
    
}

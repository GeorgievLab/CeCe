#ifndef MAINFRAME_H
#define MAINFRAME_H

/* ************************************************************************ */

// wxWidgets
#include <wx/scopedptr.h>

// wxCrafter
#include "wxcrafter.h"

// Simulator
#include "simulator/World.h"
#include "simulator/Simulator.h"
#include "simulator/WorldFactory.h"

/* ************************************************************************ */

class MainFrame : public MainFrameBaseClass
{
public:
    MainFrame(wxWindow* parent = nullptr);
    virtual ~MainFrame();

    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

    void OnSimulationUpdate(wxThreadEvent& evt);

protected:

    virtual void OnSimulationStart(wxCommandEvent& event);
    virtual void OnSimulationStop(wxCommandEvent& event);
    virtual void OnSimulationNotRunningUpdateUi(wxUpdateUIEvent& event);
    virtual void OnSimulationRunningUpdateUi(wxUpdateUIEvent& event);
    virtual void OnFileNew(wxCommandEvent& event);
    virtual void OnFileOpen(wxCommandEvent& event);
    virtual void OnFileSave(wxCommandEvent& event);
    virtual void OnFileSaveAs(wxCommandEvent& event);
    virtual void OnSimulationRestart(wxCommandEvent& event);
    virtual void OnSimulationStep(wxCommandEvent& event);

private:

    /// Current loaded file name.
    wxString m_fileName;

    /// Simulator.
    simulator::Simulator m_simulator;

    /// World factory
    wxScopedPtr<simulator::WorldFactory> m_worldFactory;

};

/* ************************************************************************ */

#endif // MAINFRAME_H

#ifndef MAINFRAME_H
#define MAINFRAME_H

/* ************************************************************************ */

// wxWidgets
#include <wx/scopedptr.h>
#include <wx/filehistory.h>

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
    virtual void OnViewIsometric(wxCommandEvent& event);
    virtual void OnViewTop(wxCommandEvent& event);

    virtual void OnSimulationStart(wxCommandEvent& event);
    virtual void OnSimulationStop(wxCommandEvent& event);
    virtual void OnSimulationNotRunningUpdateUi(wxUpdateUIEvent& event);
    virtual void OnSimulationRunningUpdateUi(wxUpdateUIEvent& event);
    virtual void OnFileNew(wxCommandEvent& event);
    virtual void OnFileOpen(wxCommandEvent& event);
    virtual void OnFileSave(wxCommandEvent& event);
    virtual void OnFileSaveAs(wxCommandEvent& event);
    void OnFileOpenRecent(wxCommandEvent& event);
    virtual void OnSimulationRestart(wxCommandEvent& event);
    virtual void OnSimulationStep(wxCommandEvent& event);


// Protected Operations
protected:


    /**
     * @brief Load configuration.
     */
    void LoadConfig();


    /**
     * @brief Store configuration.
     */
    void StoreConfig();


private:

    /// Current loaded file name.
    wxString m_fileName;

    /// Simulator.
    simulator::Simulator m_simulator;

    /// World factory
    wxScopedPtr<simulator::WorldFactory> m_worldFactory;

    /// File history
    wxFileHistory m_fileHistory;
};

/* ************************************************************************ */

#endif // MAINFRAME_H

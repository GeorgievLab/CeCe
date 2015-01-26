
#pragma once

/* ************************************************************************ */

// wxWidgets
#include <wx/filename.h>
#include <wx/filehistory.h>

// wxCrafter
#include "wxcrafter.h"

// Simulator
#include "simulator/World.h"
#include "simulator/Simulator.h"
#include "simulator/WorldFactory.h"

// GUI
#include "gui/SimulatorThread.hpp"

/* ************************************************************************ */

class MainFrame : public MainFrameBaseClass
{
public:
    MainFrame(wxWindow* parent = nullptr);
    virtual ~MainFrame();

    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

    void OnSimulationUpdate(wxThreadEvent& evt);

    /**
     * @brief Event when source code is not able compile.
     *
     * @param evt
     */
    void OnSimulationError(wxCommandEvent& evt);

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
     * @brief Load file and create world.
     *
     * @param path Path to file.
     */
    void LoadFile(const wxFileName& path);


    /**
     * @brief Load text into world.
     *
     * @param code Simulation code.
     */
    void LoadText(const wxString& code);


    /**
     * @brief Load configuration.
     */
    void LoadConfig();


    /**
     * @brief Store configuration.
     */
    void StoreConfig();


// Private Data Members
private:

    /// Current loaded file name.
    wxFileName m_fileName;

    /// Simulator thread.
    SimulatorThread m_simulatorThread;

    /// File history
    wxFileHistory m_fileHistory;
};

/* ************************************************************************ */

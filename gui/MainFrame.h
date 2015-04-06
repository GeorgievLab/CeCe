
#pragma once

/* ************************************************************************ */

// wxWidgets
#include <wx/filename.h>
#include <wx/filehistory.h>

// wxCrafter
#include "wxcrafter.h"

// Simulator
#include "simulator/World.hpp"
#include "simulator/Simulator.hpp"
#include "simulator/WorldFactory.hpp"

// GUI
#include "gui/SimulatorThread.hpp"

/* ************************************************************************ */

/**
 * @brief Main frame.
 */
class MainFrame : public MainFrameBaseClass
{

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @return
     */
    MainFrame(wxWindow* parent = nullptr);


    /**
     * @brief Destructor.
     */
    virtual ~MainFrame();


// Public Accessors
public:


    /**
     * @brief Returns current world.
     *
     * @return
     */
    simulator::World* GetWorld() const noexcept
    {
        return m_glCanvasView->GetWorld();
    }


// Public Operations
public:


    /**
     * @brief On exit event.
     *
     * @param event
     */
    void OnExit(wxCommandEvent& event);


    /**
     * @brief On about event.
     *
     * @param event
     */
    void OnAbout(wxCommandEvent& event);


    /**
     * @brief Simulation step event.
     *
     * @param evt
     */
    void OnSimulationUpdate(wxThreadEvent& evt);


    /**
     * @brief Event when source code is not able compile.
     *
     * @param evt
     */
    void OnSimulationError(wxCommandEvent& evt);


    /**
     * @brief Handle event reporting render time.
     *
     * @param evt
     */
    void OnRenderTime(wxCommandEvent& evt);


protected:
    virtual void OnViewInterpolate(wxCommandEvent& event);
    virtual void OnViewInterpolateUpdateUi(wxUpdateUIEvent& event);
    virtual void OnSimulationScreenshot(wxCommandEvent& event);

    void OnViewGridUpdateUi(wxUpdateUIEvent& event) override;
    void OnViewVelocityUpdateUi(wxUpdateUIEvent& event) override;
    void OnViewGrid(wxCommandEvent& event) override;
    void OnViewVelocity(wxCommandEvent& event) override;
    void OnViewReset(wxCommandEvent& event) override;

    void OnSimulationStart(wxCommandEvent& event) override;
    void OnSimulationStop(wxCommandEvent& event) override;
    void OnSimulationNotRunningUpdateUi(wxUpdateUIEvent& event) override;
    void OnSimulationRunningUpdateUi(wxUpdateUIEvent& event) override;
    void OnFileNew(wxCommandEvent& event) override;
    void OnFileOpen(wxCommandEvent& event) override;
    void OnFileSave(wxCommandEvent& event) override;
    void OnFileSaveAs(wxCommandEvent& event) override;
    void OnFileOpenRecent(wxCommandEvent& event);
    void OnSimulationRestart(wxCommandEvent& event) override;
    void OnSimulationStep(wxCommandEvent& event) override;


// Public Operations
public:


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

    /// Flag when source is changed.
    bool m_sourceChanged = false;
};

/* ************************************************************************ */

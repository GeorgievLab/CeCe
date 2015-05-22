
#pragma once

/* ************************************************************************ */

// wxWidgets
#include <wx/filename.h>
#include <wx/filehistory.h>

// wxCrafter
#include "wxcrafter.h"

// Simulator
#include "simulator/Simulation.hpp"
#include "simulator/Simulator.hpp"
#include "simulator/SimulationFactory.hpp"

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
     * @brief Returns current simulation.
     *
     * @return
     */
    simulator::Simulation* GetSimulation() const noexcept
    {
        return m_glCanvasView->GetSimulation();
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
    void OnCodeUpdateUi(wxUpdateUIEvent& event) override;
    void OnViewCodeCheck(wxCommandEvent& event) override;
    void OnViewCodeChecked(wxUpdateUIEvent& event) override;
    void OnViewLogCheck(wxCommandEvent& event) override;
    void OnViewLogChecked(wxUpdateUIEvent& event) override;

#if ENABLE_IMAGES
    void OnSimulationScreenshot(wxCommandEvent& event) override;
#endif

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


    /**
     * @brief Save current file into history list.
     */
    void StoreCurrentFileToHistory();


    /**
     * @brief Split log splitter.
     */
    void SplitLog(int pos);


    /**
     * @brief Split code splitter.
     */
    void SplitCode(int pos);


    /**
     * @brief Unsplit log splitter.
     *
     * @return Sash position.
     */
    int UnsplitLog();


    /**
     * @brief Unsplit code splitter.
     *
     * @return Sash position.
     */
    int UnsplitCode();


// Private Data Members
private:

    /// Current loaded file name.
    wxFileName m_fileName;

    /// Simulator thread.
    SimulatorThread m_simulatorThread;

    /// File history
    wxFileHistory m_fileHistory;

    /// Log redirector
    //wxStreamToTextRedirector m_logRedirector;
};

/* ************************************************************************ */

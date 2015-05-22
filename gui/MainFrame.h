
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
     * @param parent
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


// Protected Event Handlers
protected:


    /**
     * @brief Create new simulation.
     *
     * @param event
     */
    void OnFileNew(wxCommandEvent& event) override;


    /**
     * @brief Open existing simulation file.
     *
     * @param event
     */
    void OnFileOpen(wxCommandEvent& event) override;


    /**
     * @brief Save currently open simulation file.
     *
     * @param event
     */
    void OnFileSave(wxCommandEvent& event) override;


    /**
     * @brief Save currently open simulation file as.
     *
     * @param event
     */
    void OnFileSaveAs(wxCommandEvent& event) override;


    /**
     * @brief Open recently open simulation file.
     *
     * @param event
     */
    void OnFileOpenRecent(wxCommandEvent& event);


    /**
     * @brief On exit event.
     *
     * @param event
     */
    void OnFileExit(wxCommandEvent& event) override;


    /**
     * @brief Reset current view.
     *
     * @param event
     */
    void OnViewReset(wxCommandEvent& event) override;


    /**
     * @brief Check or uncheck display code widget.
     *
     * @param event
     */
    void OnViewCodeCheck(wxCommandEvent& event) override;


    /**
     * @brief If menu item for display code widget should be checked.
     *
     * @param event
     */
    void OnViewCodeChecked(wxUpdateUIEvent& event) override;

    /**
     * @brief Check or uncheck display log widget.
     *
     * @param event
     */
    void OnViewLogCheck(wxCommandEvent& event) override;


    /**
     * @brief If menu item for display log widget should be checked.
     *
     * @param event
     */
    void OnViewLogChecked(wxUpdateUIEvent& event) override;


    /**
     * @brief Start simulation.
     *
     * @param event
     */
    void OnSimulationStart(wxCommandEvent& event) override;


    /**
     * @brief Stop simulation.
     *
     * @param event
     */
    void OnSimulationStop(wxCommandEvent& event) override;


    /**
     * @brief Perform simulation step.
     *
     * @param event
     */
    void OnSimulationStep(wxCommandEvent& event) override;


    /**
     * @brief Restart simulation.
     *
     * @param event
     */
    void OnSimulationRestart(wxCommandEvent& event) override;


#if ENABLE_SCREENSHOOT
    /**
     * @brief Save screenshoot of current simulation step.
     *
     * @param event
     */
    void OnSimulationScreenshot(wxCommandEvent& event) override;
#endif


    /**
     * @brief On about event.
     *
     * @param event
     */
    void OnHelpAbout(wxCommandEvent& event) override;


    /**
     * @brief Get information if simulation is running.
     *
     * @param event
     */
    void OnSimulationRunningUpdateUi(wxUpdateUIEvent& event) override;


    /**
     * @brief Get information if simulation is not running.
     *
     * @param event
     */
    void OnSimulationNotRunningUpdateUi(wxUpdateUIEvent& event) override;


    /**
     * @brief Error handler.
     *
     * @param event
     */
    void OnSimulationError(wxCommandEvent& event);


    /**
     * @brief Handle event reporting render time.
     *
     * @param evt
     */
    void OnRenderTime(wxCommandEvent& evt);


    /**
     * @brief Update UI of source code witget.
     *
     * @param event
     */
    void OnCodeUpdateUi(wxUpdateUIEvent& event) override;


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

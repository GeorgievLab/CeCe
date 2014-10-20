#ifndef MAINFRAME_H
#define MAINFRAME_H

#include <memory>

#include "wxcrafter.h"

#include "Simulator.h"
#include "WorldLoader.h"

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
    wxString m_fileName;

    /// Simulator.
    Simulator m_simulator;

    std::unique_ptr<WorldLoader> m_worldLoader;

};
#endif // MAINFRAME_H


#pragma once

/* ************************************************************************ */

// C++
#include <cassert>
#include <chrono>
#include <memory>
#include <vector>

// wxWidgets
#include <wx/scopedptr.h>
#include <wx/glcanvas.h>
#include <wx/timer.h>

// Simulator
#include "simulator/Simulation.hpp"
#include "simulator/Simulator.hpp"

/* ************************************************************************ */

/**
 * @brief Report render time
 */
wxDECLARE_EVENT(REPORT_FPS, wxCommandEvent);

/* ************************************************************************ */

/**
 * @brief Canvas widget.
 */
class CanvasWidget : public wxGLCanvas
{

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param parent
     * @param id
     * @param attribList
     * @param pos
     * @param size
     * @param style
     * @param name
     * @param palette
     */
    CanvasWidget(wxWindow* parent, wxWindowID id = wxID_ANY,
                 const int* attribList = nullptr,
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& size = wxDefaultSize,
                 long style = 0, const wxString& name = "GLCanvas",
                 const wxPalette& palette = wxNullPalette);


// Public Accessors
public:


    /**
     * @brief Return OpenGL context.
     *
     * @return
     */
    wxGLContext* GetContext() const NOEXCEPT
    {
        return m_context.get();
    }


    /**
     * @brief Returns current simulation.
     *
     * @return
     */
    simulator::Simulation* GetSimulation() const NOEXCEPT
    {
        assert(m_simulator);
        return m_simulator->getSimulation();
    }


// Public Mutators
public:


    /**
     * @brief Set current simulator.
     *
     * @param simulator Pointer to simulator.
     * @param mutex
     */
    void SetSimulator(simulator::Simulator* simulator, wxMutex* mutex) NOEXCEPT
    {
        m_simulator = simulator;
        m_mutex = mutex;
    }


// Public Operations
public:


    /**
     * @brief Update view.
     */
    void Update() NOEXCEPT;


    /**
     * @brief Reset view.
     */
    void ViewReset() NOEXCEPT;


// Public Event Handlers
public:


    /**
     * @brief On paint event.
     *
     * @param event
     */
    void OnPaint(wxPaintEvent& WXUNUSED(event))
    {
        Render();
    }


    /**
     * @brief On resize event.
     *
     * @param event
     */
    void OnResize(wxSizeEvent& event);


    /**
     * @brief Process zoom event.
     *
     * @param event
     */
    void OnZoom(wxMouseEvent& event) NOEXCEPT;


    /**
     * @brief On update paint event.
     *
     * @param event
     */
    void OnUpdated(wxCommandEvent& WXUNUSED(event))
    {
        Update();
    }


    /**
     * @brief On timer event.
     *
     * @param event
     */
    void OnTimer(wxTimerEvent& WXUNUSED(event))
    {
        Update();
    }


    /**
     * @brief Process mouse move event.
     *
     * @param event
     */
    void OnMouseMove(wxMouseEvent& event);


    /**
     * @brief Process mouse events.
     *
     * @param event
     */
    void OnMouseDown(wxMouseEvent& event);


    /**
     * @brief Process mouse events.
     *
     * @param event
     */
    void OnMouseMotion(wxMouseEvent& event);


    /**
     * @brief Process mouse events.
     *
     * @param event
     */
    void OnMouseUp(wxMouseEvent& event);


    /**
     * @brief On key down event.
     *
     * @param event
     */
    void OnKeyDown(wxKeyEvent& event);


// Protected Operations
protected:


    /**
     * @brief Render content.
     */
    void Render() NOEXCEPT;


    /**
     * @brief Update render FPS.
     */
    void UpdateFps() NOEXCEPT;


// Private Data Members
private:

    // GL canvas.
    wxScopedPtr<wxGLContext> m_context;

    /// Simulator pointer.
    simulator::Simulator* m_simulator = nullptr;

    /// Simulator mutex.
    wxMutex* m_mutex = nullptr;

    // Redraw timer.
    wxTimer m_timer;

    /// Position of dragging start.
    wxPoint m_dragStart;

    /// Time when the last frame was rendered.
    std::chrono::high_resolution_clock::time_point m_renderTime;

    /// Number of rendered frames.
    unsigned long m_renderFrames = 0;

    /// Time from last FPS recalculation.
    std::chrono::milliseconds m_renderFpsRecalc;

    /// Base zoom.
    float m_baseZoom = 1.f;
};

/* ************************************************************************ */

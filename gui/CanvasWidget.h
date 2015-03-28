
#pragma once

/* ************************************************************************ */

// C++
#include <cassert>
#include <chrono>

// wxWidgets
#include <wx/scopedptr.h>
#include <wx/glcanvas.h>
#include <wx/timer.h>

// Simulator
#include "simulator/World.hpp"
#include "simulator/Simulator.hpp"

// Render
#include "render/Context.hpp"

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
    wxGLContext* getContext() const noexcept
    {
        return m_context.get();
    }


    /**
     * @brief Returns current world.
     *
     * @return
     */
    simulator::World* GetWorld() const noexcept
    {
        assert(m_simulator);
        return m_simulator->getWorld();
    }


    /**
     * @brief If grid is rendered.
     *
     * @return
     */
    bool IsViewGrid() const noexcept;


    /**
     * @brief If velocity is rendered.
     *
     * @param flag
     */
    bool IsViewVelocity() const noexcept;


// Public Mutators
public:


    /**
     * @brief Set current simulator.
     *
     * @return
     */
    void SetSimulator(simulator::Simulator* simulator, wxMutex* mutex) noexcept
    {
        m_simulator = simulator;
        m_mutex = mutex;
    }


    /**
     * @brief Show or hide grid render.
     *
     * @param flag
     */
    void SetViewGrid(bool flag) noexcept;


    /**
     * @brief Show or hide velocity render.
     *
     * @param flag
     */
    void SetViewVelocity(bool flag) noexcept;


// Public Operations
public:

    /**
     * @brief Update view.
     */
    void Update() noexcept;


    /**
     * @brief Reset view.
     */
    void ViewReset() noexcept;


    /**
     * @brief On paint event.
     */
    void OnPaint(wxPaintEvent& WXUNUSED(event))
    {
        Render();
    }


    /**
     * @brief On resize event.
     */
    void OnResize(wxSizeEvent& event)
    {
        //Update();
    }


    /**
     * @brief Process zoom event.
     *
     * @param event
     */
    void OnZoom(wxMouseEvent& event) noexcept;


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


// Protected Operations
protected:


    /**
     * @brief Setup projection matrix.
     */
    void SetupProjection() noexcept;


    /**
     * @brief Render content.
     */
    void Render() noexcept;


// Private Data Members
private:

    // GL canvas.
    wxScopedPtr<wxGLContext> m_context;

    // Redraw timer.
    wxTimer m_timer;

    /// Simulator pointer.
    simulator::Simulator* m_simulator;

    /// Simulator mutex.
    wxMutex* m_mutex;

    /// Render context.
    render::Context m_renderer;

    /// Position of dragging start.
    wxPoint m_dragStart;

    /// Time when the last frame was rendered.
    std::chrono::high_resolution_clock::time_point m_renderTime;

    /// Number of rendered frames.
    unsigned long m_renderFrames = 0;

    /// Time from last FPS recalculation.
    std::chrono::milliseconds m_renderFpsRecalc;

    /// Render flags.
    simulator::World::RenderFlagsType m_renderFlags = simulator::World::RENDER_NONE;

};

/* ************************************************************************ */

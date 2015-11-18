/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/*                                                                          */
/* This file is part of CeCe.                                               */
/*                                                                          */
/* CeCe is free software: you can redistribute it and/or modify             */
/* it under the terms of the GNU General Public License as published by     */
/* the Free Software Foundation, either version 3 of the License, or        */
/* (at your option) any later version.                                      */
/*                                                                          */
/* CeCe is distributed in the hope that it will be useful,                  */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of           */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            */
/* GNU General Public License for more details.                             */
/*                                                                          */
/* You should have received a copy of the GNU General Public License        */
/* along with CeCe.  If not, see <http://www.gnu.org/licenses/>.            */
/*                                                                          */
/* ************************************************************************ */

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
#include "render/Context.hpp"

// GUI
#include "gui/SimulatorThread.hpp"

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
    wxGLContext* GetContext() const noexcept
    {
        return m_context.get();
    }


    /**
     * @brief Return render context.
     *
     * @return
     */
    render::Context& GetRenderContext() noexcept
    {
        return m_renderContext;
    }


    /**
     * @brief Returns current simulation.
     *
     * @return
     */
    simulator::Simulation* GetSimulation() const noexcept
    {
        assert(m_simulator);
        return m_simulator->GetSimulation();
    }


// Public Mutators
public:


    /**
     * @brief Set current simulator.
     *
     * @param simulator Pointer to simulator thread.
     */
    void SetSimulator(SimulatorThread* simulator) noexcept
    {
        m_simulator = simulator;
    }


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
    void Render() noexcept;


    /**
     * @brief Update render FPS.
     */
    void UpdateFps() noexcept;


// Private Data Members
private:

    /// GL canvas.
    wxScopedPtr<wxGLContext> m_context;

    /// Rendering context.
    render::Context m_renderContext;

    /// Simulator pointer.
    SimulatorThread* m_simulator = nullptr;

    // Redraw timer.
    wxTimer m_timer;

    /// Position of dragging start.
    wxPoint m_dragStart;

    /// Time when the last frame was rendered.
    std::chrono::high_resolution_clock::time_point m_renderTime;

    /// Number of rendered frames.
    unsigned long m_renderFrames = 0;

    /// Time from last FPS recalculation.
    std::chrono::milliseconds m_renderFpsRecalc{0};

    /// Base zoom.
    float m_baseZoom = 1.f;
};

/* ************************************************************************ */

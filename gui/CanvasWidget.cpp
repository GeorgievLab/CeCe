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

// Declaration
#include "CanvasWidget.hpp"

// C++
#include <algorithm>

// wxWidgets
#include <wx/dcclient.h>

// Simulator
#include "simulator/Simulation.hpp"
#include "render/Context.hpp"

// GUI
#include "gui/SimulatorThread.hpp"

/* ************************************************************************ */

wxDEFINE_EVENT(REPORT_FPS, wxCommandEvent);

/* ************************************************************************ */

static const int g_attribList[] = {
    WX_GL_RGBA,
    WX_GL_DOUBLEBUFFER,
    WX_GL_SAMPLE_BUFFERS, GL_TRUE, // Multi-sampling
    WX_GL_SAMPLES, 4,
    WX_GL_DEPTH_SIZE, 16,
    0, 0
};

/* ************************************************************************ */

CanvasWidget::CanvasWidget(wxWindow* parent, wxWindowID id,
                           const int* attribList, const wxPoint& pos,
                           const wxSize& size, long style,
                           const wxString& name, const wxPalette& palette)
    : wxGLCanvas(parent, id, g_attribList, pos, size, style, name, palette)
    , m_timer(this)
    , m_renderTime(std::chrono::high_resolution_clock::now())
{
    m_context.reset(new wxGLContext(this));

    Bind(wxEVT_PAINT, &CanvasWidget::OnPaint, this);
    Bind(wxEVT_SIZE, &CanvasWidget::OnResize, this);
    Bind(wxEVT_MOUSEWHEEL, &CanvasWidget::OnZoom, this);
    Bind(wxEVT_LEFT_DOWN, &CanvasWidget::OnMouseDown, this);
    Bind(wxEVT_MOTION, &CanvasWidget::OnMouseMotion, this);
    Bind(wxEVT_LEFT_UP, &CanvasWidget::OnMouseUp, this);
    //Bind(EVT_UPDATED, &CanvasWidget::OnUpdated, this);
    Bind(wxEVT_KEY_DOWN, &CanvasWidget::OnKeyDown, this);

    // Bind timer
    Bind(wxEVT_TIMER, &CanvasWidget::OnTimer, this);

    // Start timer
    m_timer.Start(1000 / 60);
}

/* ************************************************************************ */

void CanvasWidget::Update() noexcept
{
    Refresh(false);
}

/* ************************************************************************ */

void CanvasWidget::ViewReset() noexcept
{
    auto& camera = m_renderContext.getCamera();
    camera.setZoom(m_baseZoom);
    camera.setPosition(Zero);

    // Refresh view
    Update();
}

/* ************************************************************************ */

void CanvasWidget::OnResize(wxSizeEvent& event)
{
    if (!(m_simulator && m_simulator->GetSimulation()))
    {
        event.Skip();
        return;
    }

    auto size = m_simulator->GetSimulation()->getWorldSize();

    m_baseZoom = std::max(
        size.getWidth().value() / event.GetSize().GetWidth(),
        size.getHeight().value() / event.GetSize().GetHeight()
    );

    auto& camera = m_renderContext.getCamera();
    auto zoom = camera.getZoom();

    zoom = std::min(std::max(0.05f * m_baseZoom, zoom), m_baseZoom);

    camera.setZoom(zoom);

    Update();
}

/* ************************************************************************ */

void CanvasWidget::OnZoom(wxMouseEvent& event) noexcept
{
    if (event.GetWheelAxis() != wxMOUSE_WHEEL_VERTICAL)
    {
        event.Skip();
        return;
    }

    auto& camera = m_renderContext.getCamera();
    auto zoom = camera.getZoom();

    zoom -= m_baseZoom * 0.001 * event.GetWheelRotation();
    zoom = std::min(std::max(0.05f * m_baseZoom, zoom), m_baseZoom);

    // Set camera zoom
    camera.setZoom(zoom);

    // Refresh view
    Update();
}

/* ************************************************************************ */

void CanvasWidget::OnMouseDown(wxMouseEvent& event)
{
    m_dragStart = event.GetPosition();
}

/* ************************************************************************ */

void CanvasWidget::OnMouseMotion(wxMouseEvent& event)
{
    if (!event.LeftIsDown())
    {
        event.Skip();
        return;
    }

    // TODO: move camera
    auto& camera = m_renderContext.getCamera();
    auto pos = camera.getPosition();

    // Change vector
    auto change = event.GetPosition() - m_dragStart;

    pos.x() += units::um(change.x);
    pos.y() -= units::um(change.y);

    camera.setPosition(pos);

    m_dragStart = event.GetPosition();

    // Refresh view
    Update();
}

/* ************************************************************************ */

void CanvasWidget::OnMouseUp(wxMouseEvent& event)
{
    m_dragStart = wxPoint{};
}

/* ************************************************************************ */

void CanvasWidget::OnKeyDown(wxKeyEvent& event)
{
    // Nothing to do
}

/* ************************************************************************ */

void CanvasWidget::Render() noexcept
{
    static bool s_error = false;

    if (s_error)
        return;

    if (!IsShown())
        return;

    wxASSERT(m_simulator);

    // Set current context
    SetCurrent(*m_context);

    // Initialize drawing
    if (!m_renderContext.isInitialized())
        m_renderContext.init();

    wxPaintDC dc(this);

    // Draw simulation
    try
    {
        wxMutexLocker lock(*m_simulator->GetMutex());

        m_renderContext.deleteReleasedObjects();
        m_renderContext.frameBegin(GetSize().GetWidth(), GetSize().GetHeight());

        if (m_simulator->GetSimulation())
        {
            m_simulator->GetSimulation()->draw(m_renderContext);
        }

        m_renderContext.frameEnd();
    }
    catch (const std::exception& e)
    {
        wxScopedPtr<wxCommandEvent> event(new wxCommandEvent(EVT_ERROR));
        event->SetString(e.what());
        wxQueueEvent(GetParent(), event.release());
        s_error = true;
        return;
    }

    // Swap front and back buffer
    SwapBuffers();

    // Calculate FPS
    UpdateFps();
}

/* ************************************************************************ */

void CanvasWidget::UpdateFps() noexcept
{
    using namespace std::chrono;

    // Increase a number of rendered frames
    m_renderFrames++;

    // Calculate time that takes to render time
    // Even render function call
    m_renderFpsRecalc += duration_cast<milliseconds>(high_resolution_clock::now() - m_renderTime);

    if (m_renderFpsRecalc > milliseconds(1000))
    {
        int fps = m_renderFrames * 1000.f / m_renderFpsRecalc.count();

        // Report render time
        wxCommandEvent event(REPORT_FPS);
        event.SetInt(fps);
        wxPostEvent(this, event);

        m_renderFpsRecalc = milliseconds(0);
        m_renderFrames = 0;
    }

    m_renderTime = high_resolution_clock::now();
}

/* ************************************************************************ */

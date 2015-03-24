
/* ************************************************************************ */

// Declaration
#include "CanvasWidget.h"

// C++
#include <algorithm>

// wxWidgets
#include <wx/dcclient.h>

// Simulator
#include "simulator/World.hpp"

// Render
#include "render/Context.hpp"

// GUI
#include "gui/SimulatorThread.hpp"

/* ************************************************************************ */

const int g_attribList[] = {
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
{
    m_context.reset(new wxGLContext(this));

    Bind(wxEVT_PAINT, &CanvasWidget::OnPaint, this);
    Bind(wxEVT_SIZE, &CanvasWidget::OnResize, this);
    Bind(wxEVT_MOUSEWHEEL, &CanvasWidget::OnZoom, this);
    Bind(wxEVT_LEFT_DOWN, &CanvasWidget::OnMouseDown, this);
    Bind(wxEVT_MOTION, &CanvasWidget::OnMouseMotion, this);
    Bind(wxEVT_LEFT_UP, &CanvasWidget::OnMouseUp, this);
    Bind(EVT_UPDATED, &CanvasWidget::OnUpdated, this);
}

/* ************************************************************************ */

void CanvasWidget::Update() noexcept
{
    Refresh(false);
}

/* ************************************************************************ */

void CanvasWidget::ViewReset() noexcept
{
    // Refresh view
    Update();
}

/* ************************************************************************ */

void CanvasWidget::Init() noexcept
{
    m_renderer.init();
}

/* ************************************************************************ */

void CanvasWidget::Render() noexcept
{
    if (!IsShown())
        return;

    SetCurrent(*m_context);
    wxPaintDC dc(this);

    if (!m_init)
    {
        Init();
        m_init = true;
    }

    m_renderer.frameBegin(GetSize().GetWidth(), GetSize().GetHeight());

    {
        wxASSERT(m_mutex);
        wxMutexLocker lock(*m_mutex);

        // Render world
        wxASSERT(m_simulator);
        auto world = m_simulator->getWorld();
        if (world)
        {
            world->render(m_renderer);
        }
    }

    m_renderer.frameEnd();
    SwapBuffers();
}

/* ************************************************************************ */

void CanvasWidget::OnZoom(wxMouseEvent& event) noexcept
{
    if (event.GetWheelAxis() != wxMOUSE_WHEEL_VERTICAL)
    {
        event.Skip();
        return;
    }

    auto& camera = m_renderer.getCamera();
    auto zoom = camera.getZoom();

    zoom -= 0.001 * event.GetWheelRotation();
    zoom = std::min(std::max(0.1f, zoom), 10.f);

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

    // Refresh view
    Update();
}

/* ************************************************************************ */

void CanvasWidget::OnMouseUp(wxMouseEvent& event)
{
    m_dragStart = wxPoint{};
}

/* ************************************************************************ */

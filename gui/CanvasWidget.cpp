
/* ************************************************************************ */

// Declaration
#include "CanvasWidget.h"

// C++
#include <algorithm>

// wxWidgets
#include <wx/dcclient.h>

// Simulator
#include "simulator/World.h"

// Render
#include "render/Context.h"

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

    // Repeat run timer
    m_timer.Bind(wxEVT_TIMER, [this](wxTimerEvent& event) { Update(); });

    // Run at 30 FPS
    m_timer.Start(1000 / 30.f);
}

/* ************************************************************************ */

void CanvasWidget::Update() noexcept
{
    Refresh(false);
}

/* ************************************************************************ */

void CanvasWidget::Init() noexcept
{
    m_renderer.init();

    // Initial projection
    SetupProjection();
}

/* ************************************************************************ */

void CanvasWidget::SetupProjection() noexcept
{
    switch (m_projectionType)
    {
    default:
    case Projection::Top:
        m_renderer.setTopView(GetSize().GetWidth(), GetSize().GetHeight(), m_zoom);
        break;

    case Projection::Isometric:
        m_renderer.setIsometricView(GetSize().GetWidth(), GetSize().GetHeight(), m_zoom);
        break;
    }
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

    m_renderer.frameBegin();

    // Render world
    wxASSERT(m_simulator);
    if (m_simulator->getWorld())
    {
        std::lock_guard<std::mutex> _(m_simulator->getMuttex());

        m_renderer.drawWorld(*m_simulator->getWorld());
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

    m_zoom += 0.001 * event.GetWheelRotation();
    m_zoom = std::min(std::max(0.1f, m_zoom), 10.f);

    // Refresh view
    SetupProjection();
    Update();
}

/* ************************************************************************ */

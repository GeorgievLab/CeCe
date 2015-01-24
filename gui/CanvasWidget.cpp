
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
    auto& camera = m_renderer.getCamera();

    switch (m_projectionType)
    {
    default:
    case Projection::Top:
        camera.setMode(render::Camera::Mode::Top);
        break;

    case Projection::Isometric:
        camera.setMode(render::Camera::Mode::Isometric);
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

    m_renderer.frameBegin(GetSize().GetWidth(), GetSize().GetHeight());

    {
        wxASSERT(m_mutex);
        wxMutexLocker lock(*m_mutex);

        // Render world
        wxASSERT(m_simulator);
        if (m_simulator->getWorld())
        {
            m_renderer.drawWorld(*m_simulator->getWorld());
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


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

CanvasWidget::CanvasWidget(wxWindow* parent, wxWindowID id,
                           const int* attribList, const wxPoint& pos,
                           const wxSize& size, long style,
                           const wxString& name, const wxPalette& palette)
    : wxGLCanvas(parent, id, attribList, pos, size, style, name, palette)
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
    // Clear color.
    glClearColor(1.f, 1.f, 1.f, 1.f);

    // Initial projection
    SetupProjection();
}

/* ************************************************************************ */

void CanvasWidget::SetupProjection() noexcept
{
    const int width = GetSize().GetWidth();
    const int height = GetSize().GetHeight();
    const GLdouble wh = width / 2.0;
    const GLdouble hh = height / 2.0;

    // Setup viewport (whole window)
    glViewport(0, 0, width, height);

    // Setup projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-wh, wh, -hh, hh, -1, 1);

    // Apply zoom matrix
    glScalef(m_zoom, m_zoom, m_zoom);
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

    // Clear
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render world
    if (m_simulator->getWorld())
    {
        render::Context context;

        m_simulator->getWorld()->draw(context);
    }

    glFlush();
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

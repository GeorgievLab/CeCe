
/* ************************************************************************ */

// Declaration
#include "CanvasWidget.h"

// C++
#include <algorithm>

// wxWidgets
#include <wx/dcclient.h>

// Simulator
#include "simulator/World.hpp"
#include "simulator/StreamlinesModule.hpp"
#include "render/Context.hpp"

// GUI
#include "gui/SimulatorThread.hpp"

/* ************************************************************************ */

wxDEFINE_EVENT(REPORT_FPS, wxCommandEvent);

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

bool CanvasWidget::IsViewGrid() const noexcept
{
    return m_renderFlags & simulator::World::RENDER_GRID;
}

/* ************************************************************************ */

bool CanvasWidget::IsViewVelocity() const noexcept
{
    return m_renderFlags & simulator::World::RENDER_VELOCITY;
}

/* ************************************************************************ */

void CanvasWidget::SetViewGrid(bool flag) noexcept
{
    if (flag)
        m_renderFlags |= simulator::World::RENDER_GRID;
    else
        m_renderFlags &= ~simulator::World::RENDER_GRID;

    Update();
}

/* ************************************************************************ */

void CanvasWidget::SetViewVelocity(bool flag) noexcept
{
    if (flag)
        m_renderFlags |= simulator::World::RENDER_VELOCITY;
    else
        m_renderFlags &= ~simulator::World::RENDER_VELOCITY;

    Update();
}

/* ************************************************************************ */

void CanvasWidget::Update() noexcept
{
    Refresh(false);
}

/* ************************************************************************ */

void CanvasWidget::ViewReset() noexcept
{
    auto& camera = m_renderer.getCamera();
    camera.setZoom(m_baseZoom);
    camera.setPosition({0, 0});

    // Refresh view
    Update();
}

/* ************************************************************************ */

void CanvasWidget::Render() noexcept
{
    if (!IsShown())
        return;

    if (m_error)
        return;

    // Init renderer
    if (!m_renderer.isInit())
    {
        SetCurrent(*m_context);
        m_renderer.init();

        wxASSERT(m_simulator);
        m_simulator->renderInit(m_renderer);
    }

    wxPaintDC dc(this);

    m_renderer.frameBegin(GetSize().GetWidth(), GetSize().GetHeight());

    if (m_simulator->getWorld())
    {
        wxASSERT(m_mutex);
        wxMutexLocker lock(*m_mutex);

        // Render world
        try
        {
            wxASSERT(m_simulator);
            m_simulator->render(m_renderer);
        }
        catch (const std::exception& e)
        {
            wxScopedPtr<wxCommandEvent> event(new wxCommandEvent(EVT_ERROR));
            event->SetString(e.what());
            wxQueueEvent(GetParent(), event.release());
            m_error = true;
            return;
        }
    }

    m_renderer.frameEnd();
    SwapBuffers();

    {
        using namespace std::chrono;

        // Increase a number of rendered frames
        m_renderFrames++;

        // Calculate time that takes to render time
        // Even call to render function.
        m_renderFpsRecalc += duration_cast<milliseconds>(high_resolution_clock::now() - m_renderTime);

        if (m_renderFpsRecalc > milliseconds(1000))
        {
            int fps = m_renderFrames * 1000 / m_renderFpsRecalc.count();

            // Report render time
            wxCommandEvent event(REPORT_FPS);
            event.SetInt(fps);
            wxPostEvent(this, event);

            m_renderFpsRecalc = milliseconds(0);
        }
    }
}

/* ************************************************************************ */

void CanvasWidget::OnResize(wxSizeEvent& event)
{
    if (!(m_simulator && m_simulator->getWorld()))
    {
        event.Skip();
        return;
    }

    auto size = m_simulator->getWorld()->getSize();

    m_baseZoom = std::max(
        size.getWidth() / event.GetSize().GetWidth(),
        size.getHeight() / event.GetSize().GetHeight()
    );

    auto& camera = m_renderer.getCamera();
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

    auto& camera = m_renderer.getCamera();
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
    auto& camera = m_renderer.getCamera();
    auto pos = camera.getPosition();

    // Change vector
    auto change = event.GetPosition() - m_dragStart;

    pos.getX() += change.x;
    pos.getY() -= change.y;

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
    simulator::StreamlinesModule* module = nullptr;

    for (auto& mod : m_simulator->getModules())
    {
        auto ptr = dynamic_cast<simulator::StreamlinesModule*>(mod.get());

        if (ptr)
        {
            module = ptr;
            break;
        }
    }

    if (!module)
        return;

    auto pos = module->getMainCellPosition();
    auto speed = module->getFlowSpeed();

    bool pos_changed = false;
    bool speed_changed = false;

    switch (event.GetKeyCode())
    {
    default: break;
    case 'W':
    case 'w': pos.getY() += units::um(1); pos_changed = true; break;
    case 'S':
    case 's': pos.getY() -= units::um(1); pos_changed = true; break;
    case 'A':
    case 'a': pos.getX() -= units::um(1); pos_changed = true; break;
    case 'D':
    case 'd': pos.getX() += units::um(1); pos_changed = true; break;
    case WXK_NUMPAD_ADD:
    case '+': speed += 1; speed_changed = true; break;
    case WXK_NUMPAD_SUBTRACT:
    case '-': speed -= 1; speed_changed = true; break;
    }

    //if (speed < 0)
    //    speed = 0;

    if (pos_changed)
        module->setMainCellPosition(pos);

    if (speed_changed)
        module->setFlowSpeed(speed);

    Update();
}

/* ************************************************************************ */

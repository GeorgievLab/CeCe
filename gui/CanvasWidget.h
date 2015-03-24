
#ifndef _GUI_CANVAS_WIDGET_H_
#define _GUI_CANVAS_WIDGET_H_

/* ************************************************************************ */

// C++
#include <cassert>

// wxWidgets
#include <wx/scopedptr.h>
#include <wx/glcanvas.h>

// Simulator
#include "simulator/World.hpp"
#include "simulator/Simulator.hpp"

// Render
#include "render/Context.hpp"

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
     * @brief Returns current world.
     *
     * @return
     */
    simulator::World* GetWorld() const noexcept
    {
        assert(m_simulator);
        return m_simulator->getWorld();
    }


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
        m_renderer.setView(event.GetSize().GetWidth(), event.GetSize().GetHeight());
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
     * @brief Init OpenGL.
     */
    void Init() noexcept;


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

    /// If is initialized
    bool m_init = false;

    /// Simulator pointer.
    simulator::Simulator* m_simulator;

    /// Simulator mutex.
    wxMutex* m_mutex;

    /// Render context.
    render::Context m_renderer;

    /// Position of dragging start.
    wxPoint m_dragStart;

};

/* ************************************************************************ */

#endif // _GUI_CANVAS_WIDGET_H_


#ifndef _GUI_CANVAS_WIDGET_H_
#define _GUI_CANVAS_WIDGET_H_

/* ************************************************************************ */

// C++
#include <cassert>

// wxWidgets
#include <wx/scopedptr.h>
#include <wx/glcanvas.h>

// Simulator
#include "simulator/World.h"
#include "simulator/Simulator.h"

/* ************************************************************************ */

/**
 * @brief Canvas widget.
 */
class CanvasWidget : public wxGLCanvas
{
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


public:


    /**
     * @brief Set current simulator.
     *
     * @return
     */
    void SetSimulator(simulator::Simulator* simulator) noexcept
    {
        m_simulator = simulator;
    }


public:


    /**
     * @brief Update view.
     */
    void Update() noexcept;


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
    void OnResize(wxSizeEvent& WXUNUSED(event))
    {
        SetupProjection();
    }


    /**
     * @brief Process zoom event.
     *
     * @param event
     */
    void OnZoom(wxMouseEvent& event) noexcept;


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


private:

    // GL canvas.
    wxScopedPtr<wxGLContext> m_context;

    /// If is initialized
    bool m_init = false;

    /// Simulator pointer.
    simulator::Simulator* m_simulator;

    /// Zoom value.
    float m_zoom = 1.f;

};

/* ************************************************************************ */

#endif // _GUI_CANVAS_WIDGET_H_

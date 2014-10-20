
#ifndef _GUI_CANVAS_WIDGET_H_
#define _GUI_CANVAS_WIDGET_H_

/* ************************************************************************ */

// wxWidgets
#include <wx/scopedptr.h>
#include <wx/glcanvas.h>

/* ************************************************************************ */

class World;

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
    World* GetWorld() const noexcept
    {
        return m_world;
    }


public:


    /**
     * @brief Change current world.
     *
     * @return
     */
    void SetWorld(World* world) noexcept
    {
        m_world = world;
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

    /// Current world.
    World* m_world;

    /// Zoom value.
    float m_zoom = 1.f;

};

/* ************************************************************************ */

#endif // _GUI_CANVAS_WIDGET_H_


#pragma once

/* ************************************************************************ */

// OpenGL
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

// Simulator
#include "render/Position.hpp"
#include "render/Color.hpp"
#include "render/Drawable.hpp"

/* ************************************************************************ */

namespace render {

/* ************************************************************************ */

/**
 * @brief OpenGL grid object.
 */
class Grid : public Drawable
{

// Public Accessors
public:


    /**
     * @brief Returns current grid size.
     *
     * @return
     */
    const Vector<unsigned int>& getSize() const noexcept
    {
        return m_size;
    }


    /**
     * @brief If grid rendering is enabled.
     *
     * @return
     */
    bool isRenderGrid() const noexcept
    {
        return m_renderGrid;
    }


// Public Mutators
public:


    /**
     * @brief Enable or disable grid rendering.
     *
     * @param flag
     */
    void setRenderGrid(bool flag) noexcept
    {
        m_renderGrid = flag;
    }


// Public Operators
public:


    /**
     * @brief Render at current position.
     *
     * @param scale Grid scale.
     * @param color Grid color.
     */
    void render(const Vector<float>& scale, const Color& color) noexcept;


    /**
     * @brief Resize grid.
     *
     * @param size
     */
    void resize(Vector<unsigned int> size) noexcept;


// Private Data Members
private:


    /// Grid size.
    Vector<unsigned int> m_size;

    /// If grid shoudl be rendered.
    bool m_renderGrid = false;

};

/* ************************************************************************ */

}

/* ************************************************************************ */

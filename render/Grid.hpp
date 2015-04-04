
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

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     */
    Grid() noexcept
        : Drawable()
    {
        // Nothing to do
    }


    /**
     * @brief Constructor.
     *
     * @param width
     * @param height
     */
    Grid(unsigned int width, unsigned int height) noexcept
        : Drawable()
    {
        resize(width, height);
    }


// Public Accessors
public:


    /**
     * @brief Returns current grid width.
     *
     * @return
     */
    unsigned int getWidth() const noexcept
    {
        return m_width;
    }


    /**
     * @brief Returns current grid height.
     *
     * @return
     */
    unsigned int getHeight() const noexcept
    {
        return m_height;
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
     * @param width
     * @param y
     */
    void resize(unsigned int width, unsigned int height) noexcept;


// Private Data Members
private:


    /// Grid width.
    unsigned int m_width;

    /// Grid width.
    unsigned int m_height;

};

/* ************************************************************************ */

}

/* ************************************************************************ */

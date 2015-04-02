
#pragma once

/* ************************************************************************ */

// OpenGL
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

// Simulator
#include "render/Position.hpp"
#include "render/Color.hpp"
#include "render/Drawable.hpp"
#include "render/Shader.hpp"

/* ************************************************************************ */

namespace render {

/* ************************************************************************ */

/**
 * @brief OpenGL grid for vectors object.
 */
class GridVector : public Drawable
{

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     */
    GridVector() noexcept
        : Drawable()
    {
        // Nothing to do
    }


    /**
     * @brief Constructor.
     *
     * @param width
     * @param height
     * @param data
     */
    GridVector(unsigned int width, unsigned int height, const Vector<float>* data) noexcept
        : Drawable()
    {
        resize(width, height, data);
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


// Public Operations
public:


    /**
     * @brief Render at current position.
     *
     * @param scale Grid scale.
     */
    void render(const Vector<float>& scale) noexcept;


    /**
     * @brief Resize grid.
     *
     * @param width
     * @param height
     * @param data
     */
    void resize(unsigned int width, unsigned int height, const Vector<float>* data) noexcept;


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

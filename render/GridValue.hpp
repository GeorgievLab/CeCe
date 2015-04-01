
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
 * @brief OpenGL grid for value object.
 */
class GridValue : public Drawable
{

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     */
    GridValue() noexcept;


    /**
     * @brief Constructor.
     *
     * @param width
     * @param height
     * @param data
     */
    GridValue(unsigned int width, unsigned int height, const float* data) noexcept
        : GridValue()
    {
        resize(width, height, data);
    }


    /**
     * @brief Destructor.
     */
    ~GridValue();


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
    void resize(unsigned int width, unsigned int height, const float* data);


// Private Data Members
private:


    /// Grid width.
    unsigned int m_width;

    /// Grid width.
    unsigned int m_height;

    /// Texture.
    GLuint m_texture;
};

/* ************************************************************************ */

}

/* ************************************************************************ */

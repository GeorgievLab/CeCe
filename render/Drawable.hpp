
#pragma once

/* ************************************************************************ */

// OpenGL
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

// Simulator
#include "render/errors.hpp"

/* ************************************************************************ */

namespace render {

/* ************************************************************************ */

/**
 * @brief OpenGL drawable object.
 */
class Drawable
{

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     */
    Drawable() noexcept;


    /**
     * @brief Destructor.
     */
    ~Drawable();


// Public Accessors
public:


    /**
     * @brief Return OpenGL buffer ID.
     *
     * @return
     */
    GLuint getBuffer() const noexcept
    {
        return m_buffer;
    }


// Public Operators
public:


    /**
     * @brief Initialize drawable object.
     */
    void init();


// Private Data Members
private:

    /// OpenGL buffer.
    GLuint m_buffer = 0;

};

/* ************************************************************************ */

}

/* ************************************************************************ */

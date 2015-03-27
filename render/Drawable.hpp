
#pragma once

/* ************************************************************************ */

// OpenGL
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

/* ************************************************************************ */

namespace render {

/* ************************************************************************ */

/**
 * @brief OpenGL drawable object.
 */
class Drawable
{

#if RENDER_USE_VBO
// Public Ctors & Dtors
public:


    /**
     * @brief Constructor
     */
    Drawable() noexcept
    {
        glGenBuffers(1, &m_buffer);
    }


    /**
     * @brief Destructor.
     */
    ~Drawable()
    {
        glDeleteBuffers(1, &m_buffer);
    }


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


// Private Data Members
private:

    /// OpenGL buffer.
    GLuint m_buffer;
#endif

};

/* ************************************************************************ */

}

/* ************************************************************************ */

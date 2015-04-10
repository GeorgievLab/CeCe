
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
 * @brief OpenGL buffer.
 */
class Buffer
{

// Public Ctors & Dtors
public:


    /**
     * @brief Destructor.
     */
    ~Buffer();


// Public Accessors
public:


    /**
     * @brief Return OpenGL buffer ID.
     *
     * @return
     */
    GLuint getId() const noexcept
    {
        return m_id;
    }


// Public Operators
public:


    /**
     * @brief Initialize buffer object.
     */
    void init();


// Private Data Members
private:

    /// OpenGL buffer.
    GLuint m_id = 0;

};

/* ************************************************************************ */

}

/* ************************************************************************ */


#pragma once

/* ************************************************************************ */

// OpenGL
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

// Simulator
#include "render/Shader.hpp"

/* ************************************************************************ */

namespace render {

/* ************************************************************************ */

/**
 * @brief OpenGL program object.
 */
class Program
{

// Public Ctors & Dtors
public:


    /**
     * @brief Destructor.
     */
    ~Program();


// Public Accessors
public:


    /**
     * @brief Return OpenGL program ID.
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
     * @brief Constructor.
     *
     * @param vs Vertex shader.
     * @param fs Fragment shader.
     */
    void init(const Shader& vs, const Shader& fs);


// Private Data Members
private:

    /// OpenGL program object.
    GLuint m_id = 0;

};

/* ************************************************************************ */

}

/* ************************************************************************ */

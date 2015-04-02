
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
     * @brief Constructor.
     *
     * @param vs Vertex shader.
     * @param fs Fragment shader.
     */
    Program(const Shader& vs, const Shader& fs) noexcept;


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


// Private Data Members
private:

    /// OpenGL program object.
    GLuint m_id;

};

/* ************************************************************************ */

}

/* ************************************************************************ */

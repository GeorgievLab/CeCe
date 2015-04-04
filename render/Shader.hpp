
#pragma once

/* ************************************************************************ */

// OpenGL
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

/* ************************************************************************ */

namespace render {

/* ************************************************************************ */

/**
 * @brief OpenGL shader object.
 */
class Shader
{

// Public Enums
public:


    /**
     * @brief Shader types.
     */
    enum class Type
    {
        VERTEX,
        FRAGMENT
    };


// Public Ctors & Dtors
public:


    /**
     * @brief Destructor.
     */
    ~Shader();


// Public Accessors
public:


    /**
     * @brief Return OpenGL shader ID.
     *
     * @return
     */
    GLuint getId() const noexcept
    {
        return m_id;
    }


// Public Operations
public:


    /**
     * @brief Init shader.
     *
     * @param type   Type of the shader.
     * @param source Shader source.
     * @param length Length of the shader source.
     */
    void init(Type type, const char* source, unsigned length);


// Private Data Members
private:

    /// OpenGL shader object.
    GLuint m_id = 0;

};

/* ************************************************************************ */

}

/* ************************************************************************ */

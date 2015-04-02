
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
     * @brief Constructor.
     *
     * @param type
     * @param source
     * @param length
     */
    Shader(Type type, const char* source, unsigned length) noexcept;


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


// Private Data Members
private:

    /// OpenGL shader object.
    GLuint m_id;

};

/* ************************************************************************ */

}

/* ************************************************************************ */

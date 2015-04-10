
#pragma once

/* ************************************************************************ */

// C++
#include <utility>

// OpenGL
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

// Simulator
#include "core/Vector.hpp"
#include "render/Position.hpp"
#include "render/Color.hpp"
#include "render/Drawable.hpp"
#include "render/Shader.hpp"
#include "render/Program.hpp"

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
     * @brief Destructor.
     */
    ~GridValue();


// Public Accessors
public:


    /**
     * @brief Returns current grid size.
     *
     * @return
     */
    const Vector<unsigned int>& getSize() const noexcept
    {
        return m_size;
    }


    /**
     * @brief If interpolation is enabled.
     *
     * @return
     */
    bool isInterpolate() const noexcept
    {
        return m_interpolate;
    }


// Public Mutators
public:


    /**
     * @brief Enable or disable interpolation.
     *
     * @param flag
     */
    void setInterpolate(bool flag) noexcept
    {
        m_interpolate = flag;
    }


// Public Operations
public:


    /**
     * @brief Initialize.
     */
    void init(Vector<unsigned int> size, const float* data);


    /**
     * @brief Render at current position.
     *
     * @param scale Grid scale.
     */
    void render(const Vector<float>& scale) noexcept;


    /**
     * @brief Resize grid.
     *
     * @param size
     * @param data
     */
    void resize(Vector<unsigned int> size, const float* data);


    /**
     * @brief Update data.
     *
     * @param data
     */
    void update(const float* data) noexcept;


// Private Data Members
private:


    /// Grid size.
    Vector<unsigned int> m_size;

    /// Texture.
    GLuint m_texture = 0;

    /// Vertex shader.
    Shader m_vertexShader;

    /// Fragment shader.
    Shader m_fragmentShader;

    /// Program
    Program m_program;

    /// Pointer to shader texture size.
    GLint m_sizePtr;

    /// Pointer to shader interpolate bool.
    GLint m_interpolatePtr;

    /// If interpolation is enabled.
    bool m_interpolate = true;
};

/* ************************************************************************ */

}

/* ************************************************************************ */
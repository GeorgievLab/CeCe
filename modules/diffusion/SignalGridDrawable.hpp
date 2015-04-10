
#pragma once

/* ************************************************************************ */

// OpenGL
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

// Simulator
#include "core/Vector.hpp"
#include "render/Shader.hpp"
#include "render/Program.hpp"
#include "render/Grid.hpp"
#include "render/Buffer.hpp"

// Module
#include "Signal.hpp"
#include "SignalGrid.hpp"

/* ************************************************************************ */

namespace module {
namespace diffusion {

/* ************************************************************************ */

/**
 * @brief Signal grid renderer.
 */
class SignalGridDrawable : public render::Grid
{

// Public Ctors & Dtors
public:


    /**
     * @brief Destructor.
     */
    ~SignalGridDrawable();


// Public Accessors
public:


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
    void init(Vector<unsigned int> size, const Signal* data);


    /**
     * @brief Render at current position.
     *
     * @param scale Grid scale.
     */
    void draw(const Vector<float>& scale) noexcept;


    /**
     * @brief Resize grid.
     *
     * @param size
     * @param data
     */
    void resize(Vector<unsigned int> size, const Signal* data);


    /**
     * @brief Update data.
     *
     * @param data
     */
    void update(const Signal* data) noexcept;


// Private Data Members
private:

    /// Draw buffer
    render::Buffer m_buffer;

    /// Texture.
    GLuint m_texture = 0;

    /// Vertex shader.
    render::Shader m_vertexShader;

    /// Fragment shader.
    render::Shader m_fragmentShader;

    /// Program
    render::Program m_program;

    /// Pointer to shader texture size.
    GLint m_sizePtr;

    /// Pointer to shader interpolate bool.
    GLint m_interpolatePtr;

    /// If interpolation is enabled.
    bool m_interpolate = true;
};

/* ************************************************************************ */

}
}

/* ************************************************************************ */


#pragma once

/* ************************************************************************ */

// OpenGL
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

// C++
#include <array>
#include <vector>

// Simulator
#include "core/Vector.hpp"
#include "render/Context.hpp"
#include "render/Shader.hpp"
#include "render/Program.hpp"
#include "render/Grid.hpp"
#include "render/Buffer.hpp"
#include "render/Color.hpp"

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
class SignalGridDrawable
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


    /**
     * @brief Get signal color.
     *
     * @param pos
     * @param color
     */
    render::Color getColor(unsigned pos) const noexcept
    {
        return m_colors[pos];
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


    /**
     * @brief Set background color.
     *
     * @param color
     */
    void setBackground(render::Color color) noexcept
    {
        m_background = color;
    }


    /**
     * @brief Set signal color.
     *
     * @param pos
     * @param color
     */
    void setColor(unsigned pos, render::Color color) noexcept
    {
        m_colors[pos] = color;
    }


// Public Operations
public:


    /**
     * @brief Initialize.
     */
    void init(render::Context& context, Vector<unsigned int> size, const Signal* data);


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

// Private Operations
private:


    /**
     * @brief Update texture data
     *
     * @param data Source data.
     *
     * @return Texture data
     */
    render::Color* updateTextureData(const Signal* data) noexcept;


// Private Data Members
private:

    /// Draw buffer
    render::Buffer m_buffer;

    /// Grid size.
    Vector<unsigned int> m_size;

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

    /// Background color.
    render::Color m_background = render::colors::BLACK;

    /// Signal colors.
    std::vector<render::Color> m_colors{{
        render::colors::CYAN,
        render::colors::MAGENTA,
        render::colors::YELLOW,
        render::colors::BLUE,
        render::colors::RED,
        render::colors::GREEN,
        render::Color{1, 0.894f, 0.769f}
    }};

    /// Texture data buffer
    std::vector<render::Color> m_textureData;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */

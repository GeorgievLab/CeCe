
#pragma once

/* ************************************************************************ */

// C++
#include <array>
#include <vector>

// Simulator
#include "core/compatibility.hpp"
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
     * @brief Constructor.
     */
    SignalGridDrawable(render::Context& context, core::Vector<unsigned int> size, const Signal* data);


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
    const core::Vector<unsigned int>& getSize() const NOEXCEPT
    {
        return m_size;
    }


    /**
     * @brief If interpolation is enabled.
     *
     * @return
     */
    bool isInterpolate() const NOEXCEPT
    {
        return m_interpolate;
    }


    /**
     * @brief Get signal color.
     *
     * @param pos
     * @param color
     */
    render::Color getColor(unsigned pos) const NOEXCEPT
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
    void setInterpolate(bool flag) NOEXCEPT
    {
        m_interpolate = flag;
    }


    /**
     * @brief Set background color.
     *
     * @param color
     */
    void setBackground(render::Color color) NOEXCEPT
    {
        m_background = color;
    }


    /**
     * @brief Set signal color.
     *
     * @param pos
     * @param color
     */
    void setColor(unsigned pos, render::Color color) NOEXCEPT
    {
        m_colors[pos] = color;
    }


// Public Operations
public:


    /**
     * @brief Render at current position.
     *
     * @param scale Grid scale.
     */
    void draw(render::Context& context) NOEXCEPT;


    /**
     * @brief Resize grid.
     *
     * @param size
     * @param data
     */
    void resize(core::Vector<unsigned int> size, const Signal* data);


    /**
     * @brief Update data.
     *
     * @param data
     */
    void update(const Signal* data) NOEXCEPT;

// Private Operations
private:


    /**
     * @brief Update texture data
     *
     * @param data Source data.
     *
     * @return Texture data
     */
    render::Color* updateTextureData(const Signal* data) NOEXCEPT;


// Private Data Members
private:

    /// Draw buffer
    render::Buffer m_buffer;

    /// Grid size.
    core::Vector<unsigned int> m_size;

    /// Texture.
    unsigned int m_texture = 0;

    /// Vertex shader.
    render::Shader m_vertexShader;

    /// Fragment shader.
    render::Shader m_fragmentShader;

    /// Program
    render::Program m_program;

    /// Pointer to shader texture size.
    render::Program::UniformId m_sizePtr;

    /// Pointer to shader interpolate bool.
    render::Program::UniformId m_interpolatePtr;

    /// If interpolation is enabled.
    bool m_interpolate = true;

    /// Background color.
    render::Color m_background = render::colors::BLACK;

    /// Signal colors.
	std::vector<render::Color> m_colors;

    /// Texture data buffer
    std::vector<render::Color> m_textureData;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */

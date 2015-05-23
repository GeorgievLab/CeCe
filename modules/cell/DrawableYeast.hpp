
#pragma once

/* ************************************************************************ */

// Simulator
#include "core/Units.hpp"
#include "core/VectorUnits.hpp"
#include "render/Buffer.hpp"
#include "render/Shader.hpp"
#include "render/Program.hpp"

/* ************************************************************************ */

namespace render { class Context; }

/* ************************************************************************ */

namespace module {
namespace cell {

/* ************************************************************************ */

/**
 * @brief Rendering object for yeast.
 */
class DrawableYeast
{

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param context Rendering context.
     */
    explicit DrawableYeast(render::Context& context);


// Public Operations
public:


    /**
     * @brief Render at given position with given rotation.
     *
     * @param context
     */
    void draw(render::Context& context, float size, float budSize) noexcept;


// Private Data Members
private:

    /// Buffer object.
    render::Buffer m_buffer;

    /// Vertex shader.
    render::Shader m_vertexShader;

    /// Fragment shader.
    render::Shader m_fragmentShader;

    /// Shader program.
    render::Program m_program;

    /// If bud should be drawn.
    render::Program::UniformId m_uniformHasBud;

    /// Yeast size variable.
    render::Program::UniformId m_uniformSizeMain;

    /// Bud size variable.
    render::Program::UniformId m_uniformSizeBud;
};

/* ************************************************************************ */

}
}

/* ************************************************************************ */

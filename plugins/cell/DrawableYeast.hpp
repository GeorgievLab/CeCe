
#pragma once

/* ************************************************************************ */

// Simulator
#include "core/compatibility.hpp"
#include "core/Units.hpp"
#include "core/VectorUnits.hpp"
#include "render/Color.hpp"
#include "render/Buffer.hpp"
#include "render/Shader.hpp"
#include "render/Program.hpp"

/* ************************************************************************ */

namespace render { class Context; }

/* ************************************************************************ */

namespace plugin {
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
     * @param size
     * @param budSize
     * @param color
     */
    void draw(render::Context& context, float size, float budSize, const render::Color& color) NOEXCEPT;


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

    /// Yeast color (GFP, YFP, RFP).
    render::Program::UniformId m_uniformColor;

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

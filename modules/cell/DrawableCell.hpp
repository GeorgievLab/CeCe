
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
 * @brief Rendering object for cell.
 */
class DrawableCell
{

// Public Operations
public:


    /**
     * @brief Initialize.
     *
     * @param context Rendering context.
     */
    void init(render::Context& context);


    /**
     * @brief Render at given position with given rotation.
     *
     * @param context
     */
    void draw(render::Context& context, float scale) noexcept;


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

    /// Cell size variable.
    render::Program::UniformId m_uniformSize;
};

/* ************************************************************************ */

}
}

/* ************************************************************************ */

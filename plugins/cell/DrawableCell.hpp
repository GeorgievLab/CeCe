/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */

#pragma once

/* ************************************************************************ */

// Simulator
#include "core/Real.hpp"
#include "core/Units.hpp"
#include "core/VectorUnits.hpp"
#include "render/Buffer.hpp"
#include "render/Shader.hpp"
#include "render/Program.hpp"
#include "render/Color.hpp"

/* ************************************************************************ */

namespace render { class Context; }

/* ************************************************************************ */

namespace plugin {
namespace cell {

/* ************************************************************************ */

/**
 * @brief Rendering object for cell.
 */
class DrawableCell
{

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param context Rendering context.
     */
    explicit DrawableCell(render::Context& context);


// Public Operations
public:


    /**
     * @brief Render at given position with given rotation.
     *
     * @param context
     * @param scale
     * @param color
     */
    void draw(render::Context& context, RealType scale, const render::Color& color) noexcept;


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

    /// Cell color (GFP, YFP, RFP).
    render::Program::UniformId m_uniformColor;

    /// Cell size variable.
    render::Program::UniformId m_uniformSize;
};

/* ************************************************************************ */

}
}

/* ************************************************************************ */

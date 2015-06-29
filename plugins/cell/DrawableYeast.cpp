
/* ************************************************************************ */

// Declaration
#include "DrawableYeast.hpp"

// C++
#include <cmath>
#include <array>
#include <cassert>

// Simulator
#include "render/Context.hpp"
#include "render/VertexFormat.hpp"

// Shaders
#include "vs.yeast.hpp"
#include "fs.yeast.hpp"

/* ************************************************************************ */

namespace plugin {
namespace cell {

/* ************************************************************************ */

struct Vertex
{
    float x, y;
    float u, v;
};

/* ************************************************************************ */

static const std::array<Vertex, 4> g_vertices = {{
    { 0.5f,  1.5f, 1.0f, 1.0f},
    { 0.5f, -0.5f, 1.0f, 0.0f},
    {-0.5f, -0.5f, 0.0f, 0.0f},
    {-0.5f,  1.5f, 0.0f, 1.0f}
}};

/* ************************************************************************ */

DrawableYeast::DrawableYeast(render::Context& context)
    : m_buffer(context, g_vertices.size() * sizeof(decltype(g_vertices)::value_type), g_vertices.data())
{
    m_vertexShader.init(render::Shader::Type::VERTEX, g_vertexShader);
    m_fragmentShader.init(render::Shader::Type::FRAGMENT, g_fragmentShader);
    m_program.init(m_vertexShader, m_fragmentShader);

    // Get variables
    m_uniformHasBud = m_program.getUniformId("g_hasBud");
    m_uniformSizeMain = m_program.getUniformId("g_sizeMain");
    m_uniformSizeBud = m_program.getUniformId("g_sizeBud");
}

/* ************************************************************************ */

void DrawableYeast::draw(render::Context& context, float size, float budSize) NOEXCEPT
{
    static render::VertexFormat vformat{
        render::VertexElement(render::VertexElementType::Position, render::DataType::Float, 2),
        render::VertexElement(render::VertexElementType::TexCoord, render::DataType::Float, 2)
    };

    // Set pointers
    context.setProgram(&m_program);
    context.setVertexBuffer(&m_buffer);
    context.setVertexFormat(&vformat);

    context.setProgramParam(m_uniformHasBud, budSize != 0.0f);
    context.setProgramParam(m_uniformSizeMain, size);
    context.setProgramParam(m_uniformSizeBud, budSize);

    // Draw circle
    context.draw(render::PrimitiveType::Quads, 4);

    context.setVertexFormat(nullptr);
    context.setVertexBuffer(nullptr);
    context.setProgram(nullptr);
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

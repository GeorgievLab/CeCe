
/* ************************************************************************ */

// Declaration
#include "DrawableCell.hpp"

// C++
#include <cmath>
#include <array>
#include <cassert>

// Render
#include "render/Context.hpp"
#include "render/VertexFormat.hpp"

// Shaders
#include "vs.cell.hpp"
#include "fs.cell.hpp"

/* ************************************************************************ */

namespace module {
namespace cell {

/* ************************************************************************ */

struct Vertex
{
    float x, y;
    float u, v;
};

/* ************************************************************************ */

static const std::array<Vertex, 4> g_vertices = {{
    { 0.5f,  0.5f, 1.0f, 1.0f},
    { 0.5f, -0.5f, 1.0f, 0.0f},
    {-0.5f, -0.5f, 0.0f, 0.0f},
    {-0.5f,  0.5f, 0.0f, 1.0f}
}};

/* ************************************************************************ */

DrawableCell::DrawableCell(render::Context& context)
    : m_buffer(context, g_vertices.size() * sizeof(decltype(g_vertices)::value_type), g_vertices.data())
{
    m_vertexShader.init(render::Shader::Type::VERTEX, g_vertexShader);
    m_fragmentShader.init(render::Shader::Type::FRAGMENT, g_fragmentShader);
    m_program.init(m_vertexShader, m_fragmentShader);

    // Get size variable
    m_uniformSize = m_program.getUniformId("g_size");
}

/* ************************************************************************ */

void DrawableCell::draw(render::Context& context, float scale) NOEXCEPT
{
    static render::VertexFormat vformat{
        render::VertexElement(render::VertexElementType::Position, render::DataType::Float, 2),
        render::VertexElement(render::VertexElementType::TexCoord, render::DataType::Float, 2)
    };

    // Set pointers
    context.setProgram(&m_program);
    context.setVertexBuffer(&m_buffer);
    context.setVertexFormat(&vformat);

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

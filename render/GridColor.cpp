
/* ************************************************************************ */

// Declaration
#include "render/GridColor.hpp"

// C++
#include <array>
#include <cassert>

// Simulator
#include "render/Context.hpp"
#include "render/VertexFormat.hpp"

/* ************************************************************************ */

namespace render {

/* ************************************************************************ */

// Vertex structure
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

GridColor::GridColor(Context& context)
    : m_texture(context)
    , m_buffer(context, g_vertices.size() * sizeof(decltype(g_vertices)::value_type), g_vertices.data())
{
    // Nothing to do
}

/* ************************************************************************ */

GridColor::GridColor(Context& context, Vector<PositionType> size)
    : GridColor(context)
{
    resize(std::move(size));
}

/* ************************************************************************ */

void GridColor::draw(Context& context) NOEXCEPT
{
    static render::VertexFormat vformat{
        render::VertexElement(render::VertexElementType::Position, render::DataType::Float, 2),
        render::VertexElement(render::VertexElementType::TexCoord, render::DataType::Float, 2)
    };

    if (m_colorsUpdated)
        sync();

    // Set parameters
    context.setVertexBuffer(&m_buffer);
    context.setVertexFormat(&vformat);
    context.setTexture(&m_texture);

    // Draw
    context.draw(PrimitiveType::Quads, 4);

    // Unset parameters
    context.setTexture(nullptr);
    context.setVertexFormat(nullptr);
    context.setVertexBuffer(nullptr);
}

/* ************************************************************************ */

void GridColor::resize(Vector<PositionType> size, const Color& color)
{
    GridBase::resize(std::move(size));

    const auto width = getSize().getWidth();
    const auto height = getSize().getHeight();

    // Resize color grid
    m_colors.resize(getSize(), color);
    m_texture.resize(getSize(), color);
}

/* ************************************************************************ */

void GridColor::clear(const Color& color)
{
    for (auto& c : m_colors)
        c = color;

    m_colorsUpdated = true;
}

/* ************************************************************************ */

void GridColor::sync()
{
    m_texture.update(m_colors.getSize(), m_colors.getData());
    m_colorsUpdated = false;
}

/* ************************************************************************ */

}

/* ************************************************************************ */

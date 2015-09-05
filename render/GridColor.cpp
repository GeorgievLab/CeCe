/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */

// Declaration
#include "render/GridColor.hpp"

// C++
#include <cassert>

// Simulator
#include "core/StaticArray.hpp"
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

static const StaticArray<Vertex, 4> g_vertices = {{
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

GridColor::GridColor(Context& context, Size size)
    : GridColor(context)
{
    resize(std::move(size));
}

/* ************************************************************************ */

void GridColor::draw(Context& context) noexcept
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

void GridColor::resize(Size size, const Color& color)
{
    GridBase::resize(std::move(size));

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
    m_texture.update(m_colors.getData());
    m_colorsUpdated = false;
}

/* ************************************************************************ */

}

/* ************************************************************************ */

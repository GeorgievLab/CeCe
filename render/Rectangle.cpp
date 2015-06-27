/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */

// Declaration
#include "render/Rectangle.hpp"

// Simulator
#include "render/Context.hpp"
#include "render/VertexFormat.hpp"

/* ************************************************************************ */

namespace render {

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

// Vertex structure
struct Vertex
{
    float x, y;
};

/* ************************************************************************ */

const StaticArray<Vertex, 4> g_vertices = {{
    {-0.5, -0.5},
    {-0.5,  0.5},
    { 0.5,  0.5},
    { 0.5, -0.5}
}};

/* ************************************************************************ */

}

/* ************************************************************************ */

Rectangle::Rectangle(Context& context)
    : m_buffer(context, g_vertices.size() * sizeof(decltype(g_vertices)::value_type), g_vertices.data())
{
    // Nothing to do
}

/* ************************************************************************ */

void Rectangle::draw(Context& context) NOEXCEPT
{
    static render::VertexFormat vformat{
        render::VertexElement(render::VertexElementType::Position, render::DataType::Float, 2)
    };

    // Set parameters
    context.setVertexBuffer(&m_buffer);
    context.setVertexFormat(&vformat);

    // Draw
    context.draw(PrimitiveType::Quads, 4);

    // Unset parameters
    context.setVertexFormat(nullptr);
    context.setVertexBuffer(nullptr);
}

/* ************************************************************************ */

}

/* ************************************************************************ */

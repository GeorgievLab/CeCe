/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */

// Declaration
#include "render/Circle.hpp"

// C++
#include <cassert>

// Simulator
#include "core/constants.hpp"
#include "render/Context.hpp"
#include "render/VertexFormat.hpp"

/* ************************************************************************ */

namespace render {

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

CONSTEXPR_CONST unsigned PARTS = 32;
CONSTEXPR_CONST unsigned VERTEX_COUNT = PARTS + 2;

/* ************************************************************************ */

// Vertex structure
struct Vertex
{
    float x, y;
    float r, g, b;
};

/* ************************************************************************ */

/**
 * @brief Generate vertices for circle.
 */
StaticArray<Vertex, VERTEX_COUNT> generateVertices()
{
    StaticArray<Vertex, VERTEX_COUNT> res;

    CONSTEXPR_CONST float step = 2 * constants::PI / PARTS;

    res[0] = Vertex{0.f, 0.f, 0.8f, 0.8f, 0.8f};

    for (unsigned i = 1; i < VERTEX_COUNT; ++i)
    {
        const float angle = step * i;
        res[i] = Vertex{std::cos(angle), std::sin(angle), 0.8f, 0.8f, 0.8f};
    }

    return res;
}

/* ************************************************************************ */

const StaticArray<Vertex, VERTEX_COUNT> g_vertices = generateVertices();

/* ************************************************************************ */

const render::VertexFormat g_vformat{
    render::VertexElement(render::VertexElementType::Position, render::DataType::Float, 2),
    render::VertexElement(render::VertexElementType::Color, render::DataType::Float, 3)
};

/* ************************************************************************ */

}

/* ************************************************************************ */

Circle::Circle(Context& context)
    : m_buffer(context, g_vertices.size() * sizeof(decltype(g_vertices)::value_type), g_vertices.data())
{
    // Nothing to do
}

/* ************************************************************************ */

void Circle::draw(Context& context) NOEXCEPT
{
    // Set parameters
    context.setVertexBuffer(&m_buffer);
    context.setVertexFormat(&g_vformat);

    // Draw
    context.draw(PrimitiveType::TriangleFan, VERTEX_COUNT);

    // Unset parameters
    context.setVertexFormat(nullptr);
    context.setVertexBuffer(nullptr);
}

/* ************************************************************************ */

}

/* ************************************************************************ */

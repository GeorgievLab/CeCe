/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */

// Declaration
#include "render/Grid.hpp"

// Simulator
#include "core/DynamicArray.hpp"
#include "render/Context.hpp"
#include "render/VertexFormat.hpp"

/* ************************************************************************ */

namespace render {

/* ************************************************************************ */

Grid::Grid(Context& context)
    : m_buffer(context)
{
    // Nothing to do
}

/* ************************************************************************ */

void Grid::draw(Context& context) noexcept
{
    static VertexFormat vformat{
        VertexElement(VertexElementType::Position, DataType::Float, 2)
    };

    // Set vertex buffer
    context.setVertexBuffer(&m_buffer);
    context.setVertexFormat(&vformat);

    auto width = getSize().getWidth();
    auto height = getSize().getHeight();

    // Draw grid
    context.draw(PrimitiveType::Lines, 2 * ((width + 1) + (height + 1)));

    // Unbind vertex buffer
    context.setVertexFormat(nullptr);
    context.setVertexBuffer(nullptr);
}

/* ************************************************************************ */

void Grid::resize(Size size) noexcept
{
    GridBase::resize(std::move(size));

    // Get grid size
    const auto width = getSize().getWidth();
    const auto height = getSize().getHeight();

    constexpr Vector<float> start{-0.5f};
    const Vector<float> step = getSize().inversed<float>();

    struct Vertex { float x, y; };

    DynamicArray<Vertex> vertices;
    vertices.reserve((width + 1) * (height + 1));

    // X lines
    for (Size::value_type i = 0; i <= width; ++i)
    {
        vertices.push_back(Vertex{start.getX() + i * step.getX(), start.getY()});
        vertices.push_back(Vertex{start.getX() + i * step.getX(), start.getY() + 1.f});
    }

    // Y lines
    for (Size::value_type i = 0; i <= height; ++i)
    {
        vertices.push_back(Vertex{start.getX(), start.getY() + i * step.getY()});
        vertices.push_back(Vertex{start.getX() + 1.f, start.getY() + i * step.getY()});
    }

    // Initialize buffer
    m_buffer.resize(
        vertices.size() * sizeof(decltype(vertices)::value_type),
        vertices.data()
    );
}

/* ************************************************************************ */

}

/* ************************************************************************ */

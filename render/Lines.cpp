/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* ************************************************************************ */

// Declaration
#include "render/Lines.hpp"

// C++
#include <cassert>
#include <cmath>

// Simulator
#include "core/DynamicArray.hpp"
#include "render/Context.hpp"
#include "render/VertexFormat.hpp"

/* ************************************************************************ */

struct Vertex
{
    float x, y;
    float red, green, blue;
};

/* ************************************************************************ */

namespace render {

/* ************************************************************************ */

void Lines::draw(Context& context) NOEXCEPT
{
    static render::VertexFormat vformat{
        render::VertexElement(render::VertexElementType::Position, render::DataType::Float, 2),
        render::VertexElement(render::VertexElementType::Color, render::DataType::Float, 3)
    };

    context.setVertexBuffer(&m_buffer);
    context.setVertexFormat(&vformat);

    // Draw lines
    context.draw(render::PrimitiveType::Lines, m_count);

    context.setVertexFormat(nullptr);
    context.setVertexBuffer(nullptr);
}

/* ************************************************************************ */

void Lines::add(const DynamicArray<Pair<PointType, PointType>>& lines)
{
    DynamicArray<Vertex> vertices;
    vertices.reserve(2 * lines.size());

    for (const auto& line : lines)
    {
        const auto& p1 = line.first;
        const auto& p2 = line.second;

        vertices.push_back(Vertex{p1.getX(), p1.getY(), 0, 1.f, 0});
        vertices.push_back(Vertex{p2.getX(), p2.getY(), 0, 1.f, 0});
    }

    m_buffer.resize(
        vertices.size() * sizeof(decltype(vertices)::value_type),
        vertices.data()
    );

    m_count += 2 * lines.size();
}

/* ************************************************************************ */

}

/* ************************************************************************ */

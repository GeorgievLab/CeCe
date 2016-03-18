/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/*                                                                          */
/* This file is part of CeCe.                                               */
/*                                                                          */
/* CeCe is free software: you can redistribute it and/or modify             */
/* it under the terms of the GNU General Public License as published by     */
/* the Free Software Foundation, either version 3 of the License, or        */
/* (at your option) any later version.                                      */
/*                                                                          */
/* CeCe is distributed in the hope that it will be useful,                  */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of           */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            */
/* GNU General Public License for more details.                             */
/*                                                                          */
/* You should have received a copy of the GNU General Public License        */
/* along with CeCe.  If not, see <http://www.gnu.org/licenses/>.            */
/*                                                                          */
/* ************************************************************************ */

// Declaration
#include "cece/render/Circle.hpp"

// C++
#include <cassert>

// CeCe
#include "cece/core/constants.hpp"
#include "cece/render/Color.hpp"
#include "cece/render/Context.hpp"
#include "cece/render/VertexFormat.hpp"

/* ************************************************************************ */

namespace cece {
namespace render {

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

constexpr unsigned PARTS = 32;
constexpr unsigned VERTEX_COUNT = PARTS + 2;

/* ************************************************************************ */

// Vertex structure
struct Vertex
{
    float x, y;
};

/* ************************************************************************ */

/**
 * @brief Generate vertices for circle.
 */
StaticArray<Vertex, VERTEX_COUNT> generateVertices()
{
    StaticArray<Vertex, VERTEX_COUNT> res;

    constexpr float step = 2 * constants::PI / PARTS;

    res[0] = Vertex{0.f, 0.f};

    for (unsigned i = 1; i < VERTEX_COUNT; ++i)
    {
        const float angle = step * i;
        res[i] = Vertex{std::cos(angle), std::sin(angle)};
    }

    return res;
}

/* ************************************************************************ */

const StaticArray<Vertex, VERTEX_COUNT> g_vertices = generateVertices();

/* ************************************************************************ */

const render::VertexFormat g_vformat{
    render::VertexElement(render::VertexElementType::Position, render::DataType::Float, 2)
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

void Circle::draw(Context& context, const Color& color) noexcept
{
    // Set parameters
    context.setColor(color);
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
}

/* ************************************************************************ */

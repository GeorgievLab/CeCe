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
#include "cece/render/Lines.hpp"

// C++
#include <cassert>
#include <cmath>

// CeCe
#include "cece/core/DynamicArray.hpp"
#include "cece/render/Context.hpp"
#include "cece/render/VertexFormat.hpp"

/* ************************************************************************ */

struct Vertex
{
    float x, y;
    float red, green, blue;
};

/* ************************************************************************ */

namespace cece {
namespace render {

/* ************************************************************************ */

void Lines::draw(Context& context) noexcept
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

        vertices.push_back(Vertex{p1.getX(), p1.getY(), 0.8f, 0.8f, 0.8f});
        vertices.push_back(Vertex{p2.getX(), p2.getY(), 0.8f, 0.8f, 0.8f});
    }

    m_buffer.resize(
        vertices.size() * sizeof(decltype(vertices)::value_type),
        vertices.data()
    );

    m_count += 2 * lines.size();
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

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
#include "cece/render/GridVector.hpp"

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

GridVector::GridVector(Context& context, Size size, const Vector<float>* data, float max)
    : m_buffer(context)
    , m_max(max)
{
    resize(std::move(size), data);
}

/* ************************************************************************ */

void GridVector::draw(Context& context) noexcept
{
    static render::VertexFormat vformat{
        render::VertexElement(render::VertexElementType::Position, render::DataType::Float, 2),
        render::VertexElement(render::VertexElementType::Color, render::DataType::Float, 3)
    };

    context.setVertexBuffer(&m_buffer);
    context.setVertexFormat(&vformat);

    // Draw lines
    const auto size = getSize();
    context.draw(render::PrimitiveType::Lines, 2 * size.getWidth() * size.getHeight());

    context.setVertexFormat(nullptr);
    context.setVertexBuffer(nullptr);
}

/* ************************************************************************ */

void GridVector::resize(Size size, const Vector<float>* data)
{
    GridBase::resize(std::move(size));

    update(data);
}

/* ************************************************************************ */

void GridVector::update(const Vector<float>* data) noexcept
{
    const auto size = getSize();

    const auto start = Vector<float>::createSingle(-0.5f);
    const Vector<float> step = getSize().inversed<float>();

    const auto width = size.getWidth();
    const auto height = size.getHeight();

    DynamicArray<Vertex> vertices;
    vertices.reserve(2 * width * height);

    // Draw grid vectors
    for (Size::ValueType j = 0; j < height; ++j)
    {
        for (Size::ValueType i = 0; i < width; ++i)
        {
            // Get vector normalized by max length
            const auto vec = data[i + j * width] / m_max;
            const Vector<float> pos{
                start.getX() + i * step.getX() + step.getX() / 2.f,
                start.getY() + j * step.getY() + step.getY() / 2.f
            };
            const float gray = std::min(vec.getLength(), 1.0f);

            const Vector<float> dest = pos + vec * step;

            vertices.push_back(Vertex{pos.getX(), pos.getY(), 0, 0, 0});
            vertices.push_back(Vertex{dest.getX(), dest.getY(), gray, gray, gray});
        }
    }

    m_buffer.resize(
        vertices.size() * sizeof(decltype(vertices)::value_type),
        vertices.data()
    );
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

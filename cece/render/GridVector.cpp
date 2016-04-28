/* ************************************************************************ */
/* Georgiev Lab (c) 2015-2016                                               */
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

GridVector::GridVector(Context& context, Size size, const Vector<RealType>* data, RealType max)
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

void GridVector::resize(Size size, const Vector<RealType>* data)
{
    GridBase::resize(std::move(size));

    update(data);
}

/* ************************************************************************ */

void GridVector::update(const Vector<RealType>* data) noexcept
{
    const auto size = getSize();

    const auto start = Vector<RealType>::createSingle(-0.5);
    const Vector<RealType> step = getSize().inversed<RealType>();

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
            const Vector<RealType> pos{
                start.getX() + i * step.getX() + step.getX() / RealType(2.0),
                start.getY() + j * step.getY() + step.getY() / RealType(2.0)
            };
            const RealType gray = std::min<RealType>(vec.getLength(), 1.0);

            const Vector<RealType> dest = pos + vec * step;

            // Source vertex
            vertices.push_back(Vertex{
                static_cast<float>(pos.getX()),
                static_cast<float>(pos.getY()),
                0,
                0,
                0
            });

            // Destination vertex
            vertices.push_back(Vertex{
                static_cast<float>(dest.getX()),
                static_cast<float>(dest.getY()),
                static_cast<float>(gray),
                static_cast<float>(gray),
                static_cast<float>(gray)
            });
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

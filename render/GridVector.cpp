/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* ************************************************************************ */

// Declaration
#include "render/GridVector.hpp"

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

GridVector::GridVector(Context& context, Size size, const Vector<float>* data, float scale)
    : m_buffer(context)
    , m_scale(scale)
{
    resize(std::move(size), data);
}

/* ************************************************************************ */

void GridVector::draw(Context& context) NOEXCEPT
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

void GridVector::update(const Vector<float>* data) NOEXCEPT
{
    const auto size = getSize();

    CONSTEXPR Vector<float> start{-0.5f};
    const Vector<float> step = getSize().inversed<float>();

    const auto width = size.getWidth();
    const auto height = size.getHeight();

    DynamicArray<Vertex> vertices;
    vertices.reserve(2 * width * height);

    // Get maximum value
    float max_squared = 1.f;
    for (Size::value_type j = 0; j < height; ++j)
    {
        for (Size::value_type i = 0; i < width; ++i)
        {
            const auto& vec = data[i + j * width];
            const float len_squared = vec.getLengthSquared();

            if (max_squared < len_squared)
                max_squared = len_squared;
        }
    }

    // Maximum value
    const float max = std::sqrt(max_squared) * m_scale;

    // Draw grid vectors
    for (Size::value_type j = 0; j < height; ++j)
    {
        for (Size::value_type i = 0; i < width; ++i)
        {
            // Get vector normalized by max length
            const auto vec = data[i + j * width] / max;
            const Vector<float> pos{
                start.getX() + i * step.getX() + step.getX() / 2.f,
                start.getY() + j * step.getY() + step.getY() / 2.f
            };
            const float red = 5 * vec.getLength();
            const float green = 5 * vec.getLength();
            const float blue = 5 * vec.getLength();

            const Vector<float> dest = pos + vec * step;

            vertices.push_back(Vertex{pos.getX(), pos.getY(), 0, 0, 0});
            vertices.push_back(Vertex{dest.getX(), dest.getY(), red, green, blue});
        }
    }

    m_buffer.resize(
        vertices.size() * sizeof(decltype(vertices)::value_type),
        vertices.data()
    );
}

/* ************************************************************************ */

}

/* ************************************************************************ */

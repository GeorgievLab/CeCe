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
#include "cece/render/GridColor.hpp"

// C++
#include <cassert>

// CeCe
#include "cece/core/StaticArray.hpp"
#include "cece/render/Context.hpp"
#include "cece/render/VertexFormat.hpp"

/* ************************************************************************ */

namespace cece {
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
}

/* ************************************************************************ */

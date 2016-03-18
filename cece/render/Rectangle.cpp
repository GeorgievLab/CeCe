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
#include "cece/render/Rectangle.hpp"

// CeCe
#include "cece/render/Color.hpp"
#include "cece/render/Context.hpp"
#include "cece/render/VertexFormat.hpp"

/* ************************************************************************ */

namespace cece {
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

const render::VertexFormat g_vformat{
    render::VertexElement(render::VertexElementType::Position, render::DataType::Float, 2)
};

/* ************************************************************************ */

}

/* ************************************************************************ */

Rectangle::Rectangle(Context& context)
    : m_buffer(context, g_vertices.size() * sizeof(decltype(g_vertices)::value_type), g_vertices.data())
{
    // Nothing to do
}

/* ************************************************************************ */

void Rectangle::draw(Context& context, const Color& color) noexcept
{
    // Set parameters
    context.setColor(color);
    context.setVertexBuffer(&m_buffer);
    context.setVertexFormat(&g_vformat);

    // Draw
    context.draw(PrimitiveType::Quads, 4);

    // Unset parameters
    context.setVertexFormat(nullptr);
    context.setVertexBuffer(nullptr);
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

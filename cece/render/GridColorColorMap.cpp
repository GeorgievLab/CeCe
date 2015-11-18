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
#include "cece/render/GridColorColorMap.hpp"

// CeCe
#include "cece/render/Context.hpp"

// Shaders
#include "vs.colormap.hpp"
#include "fs.colormap.hpp"

/* ************************************************************************ */

namespace cece {
namespace render {

/* ************************************************************************ */

GridColorColorMap::GridColorColorMap(Context& context)
    : GridColor(context)
{
    m_vertexShader.init(render::Shader::Type::VERTEX, g_vertexShader);
    m_fragmentShader.init(render::Shader::Type::FRAGMENT, g_fragmentShader);
    m_program.init(m_vertexShader, m_fragmentShader);
}

/* ************************************************************************ */

void GridColorColorMap::draw(Context& context) noexcept
{
    context.setProgram(&m_program);
    GridColor::draw(context);
    context.setProgram(nullptr);
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

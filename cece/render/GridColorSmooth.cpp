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
#include "cece/render/GridColorSmooth.hpp"

// CeCe
#include "cece/render/Context.hpp"

// Shaders
#include "vs.smooth.hpp"
#include "fs.smooth.hpp"

/* ************************************************************************ */

namespace cece {
namespace render {

/* ************************************************************************ */

GridColorSmooth::GridColorSmooth(Context& context)
    : GridColor(context)
{
    m_vertexShader.init(render::Shader::Type::VERTEX, g_vertexShader);
    m_fragmentShader.init(render::Shader::Type::FRAGMENT, g_fragmentShader);
    m_program.init(m_vertexShader, m_fragmentShader);

    m_sizePtr = m_program.getUniformId("size");
}

/* ************************************************************************ */

void GridColorSmooth::draw(Context& context) noexcept
{
    context.setProgram(&m_program);
    context.setProgramParam(m_sizePtr, getSize().getWidth(), getSize().getHeight());
    GridColor::draw(context);
    context.setProgram(nullptr);
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

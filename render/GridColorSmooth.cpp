/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */

// Declaration
#include "render/GridColorSmooth.hpp"

// Simulator
#include "render/Context.hpp"

// Shaders
#include "vs.smooth.hpp"
#include "fs.smooth.hpp"

/* ************************************************************************ */

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

/* ************************************************************************ */

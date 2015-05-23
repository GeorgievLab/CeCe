
/* ************************************************************************ */

// Declaration
#include "DrawableYeast.hpp"

// C++
#include <cmath>
#include <array>
#include <cassert>

// OpenGL
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

// Simulator
#include "render/errors.hpp"

// Shaders
#include "vs.yeast.hpp"
#include "fs.yeast.hpp"

/* ************************************************************************ */

namespace module {
namespace cell {

/* ************************************************************************ */

struct Vertex
{
    GLfloat x, y;
    GLfloat u, v;
};

/* ************************************************************************ */

static const std::array<Vertex, 4> g_vertices = {{
    { 0.5f,  1.5f, 1.0f, 1.0f},
    { 0.5f, -0.5f, 1.0f, 0.0f},
    {-0.5f, -0.5f, 0.0f, 0.0f},
    {-0.5f,  1.5f, 0.0f, 1.0f}
}};

/* ************************************************************************ */

DrawableYeast::DrawableYeast(render::Context& context)
    : m_buffer(context, g_vertices.size() * sizeof(decltype(g_vertices)::value_type), g_vertices.data())
{
    m_vertexShader.init(render::Shader::Type::VERTEX, g_vertexShader);
    m_fragmentShader.init(render::Shader::Type::FRAGMENT, g_fragmentShader);
    m_program.init(m_vertexShader, m_fragmentShader);

    // Get variables
    m_uniformHasBud = m_program.getUniformId("g_hasBud");
    m_uniformSizeMain = m_program.getUniformId("g_sizeMain");
    m_uniformSizeBud = m_program.getUniformId("g_sizeBud");
}

/* ************************************************************************ */

void DrawableYeast::draw(render::Context& context, float size, float budSize) noexcept
{
    gl(glUseProgram(m_program.getId()));

    //gl(glMatrixMode(GL_TEXTURE));
    //glPushMatrix();
    //gl(glScalef(1.f, 2.f, 1.f));

    // Set interpolate flag
    gl(glUniform1i(m_uniformHasBud, budSize != 0.0f));
    gl(glUniform1f(m_uniformSizeMain, size));
    gl(glUniform1f(m_uniformSizeBud, budSize));

    // Bind buffer
    gl(glBindBuffer(GL_ARRAY_BUFFER, m_buffer.getId()));
    gl(glEnableClientState(GL_VERTEX_ARRAY));
    gl(glEnableClientState(GL_TEXTURE_COORD_ARRAY));
    gl(glVertexPointer(2, GL_FLOAT, sizeof(Vertex), 0));
    gl(glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), reinterpret_cast<const void*>(2 * sizeof(GLfloat))));

    // Draw circle
    gl(glDrawArrays(GL_QUADS, 0, 4));

    // Disable states
    gl(glDisableClientState(GL_TEXTURE_COORD_ARRAY));
    gl(glDisableClientState(GL_VERTEX_ARRAY));
    gl(glBindBuffer(GL_ARRAY_BUFFER, 0));

    gl(glUseProgram(0));

    //glPopMatrix();
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

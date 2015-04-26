
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

/* ************************************************************************ */

namespace module {
namespace cell {

/* ************************************************************************ */

static const char g_vertexShaderSrc[] =
    "void main() {\n"
    "    gl_Position = ftransform();\n"
    "    gl_TexCoord[0] = gl_MultiTexCoord0;\n"
    "}\n"
;

/* ************************************************************************ */

static const char g_fragmentShaderSrc[] =
    "#version 120\n"
    "\n"
    "uniform bool g_hasBud = true;\n"
    "uniform vec2 g_centerMain = vec2(0.5, 0.25);\n"
    "uniform float g_sizeMain = 0.5;\n"
    "uniform float g_sizeBud = 0.1;\n"
    "\n"
    "const vec4 g_color = vec4(0.5, 0.5, 0.5, 0.5);\n"
    "const vec4 g_borderColor = vec4(0, 0, 0, 1);\n"
    "const vec4 g_backgroundColor = vec4(0, 0, 0, 0);\n"
    "\n"
    "void main() {\n"
    "    vec2 centerBud = g_centerMain + vec2(0, (g_sizeBud + g_sizeMain) * 0.5);\n"
    "\n"
    "    vec2 diffMain = gl_TexCoord[0].xy - g_centerMain;\n"
    "    vec2 diffBud = gl_TexCoord[0].xy - centerBud;\n"
    "\n"
    "    float a = 0.7;\n"
    "    float b = 0.55;\n"
    "\n"
    "    vec2 ellipseMain = pow(diffMain, vec2(2)) / pow(vec2(a, a / 2), vec2(2));\n"
    "    float distanceMain = ellipseMain.x + ellipseMain.y;\n"
    "\n"
    "    vec2 ellipseBud = pow(diffBud, vec2(2)) / pow(vec2(b, b / 2), vec2(2));\n"
    "    float distanceBud = ellipseBud.x + ellipseBud.y;\n"
    "\n"
    "    if (distanceMain <= g_sizeMain) {\n"
    "        if (distanceMain <= g_sizeMain - 0.01)\n"
    "            gl_FragColor = g_color;\n"
    "        else\n"
    "            gl_FragColor = g_borderColor;\n"
    "    } else if (g_hasBud && distanceBud <= g_sizeBud) {\n"
    "        if (distanceBud <= g_sizeBud - 0.01)\n"
    "            gl_FragColor = g_color;\n"
    "        else\n"
    "            gl_FragColor = g_borderColor;\n"
    "    } else {\n"
    "        gl_FragColor = g_backgroundColor;\n"
    "    }\n"
    "}\n"
;

/* ************************************************************************ */

struct Vertex
{
    GLfloat x, y;
    GLfloat u, v;
};

/* ************************************************************************ */

void DrawableYeast::init(render::Context& context)
{
    m_buffer.init();

    m_vertexShader.init(render::Shader::Type::VERTEX, g_vertexShaderSrc, sizeof(g_vertexShaderSrc));
    m_fragmentShader.init(render::Shader::Type::FRAGMENT, g_fragmentShaderSrc, sizeof(g_fragmentShaderSrc));
    m_program.init(m_vertexShader, m_fragmentShader);

    // Get variables
    m_uniformHasBud = m_program.getUniformId("g_hasBud");
    m_uniformSizeMain = m_program.getUniformId("g_sizeMain");
    m_uniformSizeBud = m_program.getUniformId("g_sizeBud");

    const std::array<Vertex, 4> vertices = {{
        { 0.5f,  1.5f, 1.0f, 1.0f},
        { 0.5f, -0.5f, 1.0f, 0.0f},
        {-0.5f, -0.5f, 0.0f, 0.0f},
        {-0.5f,  1.5f, 0.0f, 1.0f}
    }};

    assert(m_buffer.getId() != 0);

    gl(glBindBuffer(GL_ARRAY_BUFFER, m_buffer.getId()));
    gl(glBufferData(GL_ARRAY_BUFFER,
        vertices.size() * sizeof(decltype(vertices)::value_type),
        vertices.data(),
        GL_STATIC_DRAW
    ));

}

/* ************************************************************************ */

void DrawableYeast::draw(render::Context& context, float size, float budSize) noexcept
{
    gl(glUseProgram(m_program.getId()));

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
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

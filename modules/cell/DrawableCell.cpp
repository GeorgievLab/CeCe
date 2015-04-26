
/* ************************************************************************ */

// Declaration
#include "DrawableCell.hpp"

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
#if 1
    "#version 120\n"
    "\n"
    "uniform vec2 g_center = vec2(0.5, 0.5);\n"
    "uniform float g_size = 0.49;\n"
    "\n"
    "const vec4 g_color = vec4(0.5, 0.5, 0.5, 0.5);\n"
    "const vec4 g_borderColor = vec4(0, 0, 0, 1);\n"
    "const vec4 g_backgroundColor = vec4(0, 0, 0, 0);\n"
    "\n"
    "void main() {\n"
    "    vec4 color = g_backgroundColor;\n"
    "\n"
    "    vec2 diff = gl_TexCoord[0].xy - g_center;\n"
    "    float dist = distance(gl_TexCoord[0].xy, g_center);\n"
    "\n"
    "    if (dist <= g_size) {\n"
    "        if (dist <= g_size - 0.01)\n"
    "            gl_FragColor = g_color;\n"
    "        else\n"
    "            gl_FragColor = g_borderColor;\n"
    "    } else {\n"
    "        gl_FragColor = g_backgroundColor;\n"
    "    }\n"
    "}\n"
#else
    "#version 120\n"
    "\n"
    "uniform vec2 membraneCenter = vec2(0.5, 0.5);\n"
    "uniform vec2 nucleusCenter = vec2(0.55, 0.43);\n"
    "uniform float membraneSize = 0.48;\n"
    "uniform float nucleusSize = 0.2;\n"
    "\n"
    "const vec4 membraneColor = vec4(0.5, 0.5, 0.5, 0.5);\n"
    "const vec4 nucleusColor = vec4(0.5, 0.5, 0.5, 0.5);\n"
    "const vec4 backgroundColor = vec4(0, 0, 0, 0);\n"
    "\n"
    "float get_offset(float angle) {\n"
    "    return\n"
    "        0.3 * sin(angle * 2) +\n"
    "        0.2 * cos(angle * 6) +\n"
    "        0.5 * sin(angle * 3)\n"
    "    ;\n"
    "}\n"
    "\n"
    "float get_offset(vec2 pos) {\n"
    "    return sin(pos.x);\n"
    "}\n"
    "\n"
    "vec4 draw_circle(vec4 curColor, vec4 color, vec2 center, float size, float offsetCoeff, float angleCoeff, float base) {\n"
    "    vec2 diff = gl_TexCoord[0].xy - center;\n"
    "    float dist = distance(gl_TexCoord[0].xy, center);\n"
    "    float angle = atan(diff.y, diff.x);\n"
    "    float offset = offsetCoeff * get_offset(angle * angleCoeff);\n"
    "    float sizeMax = size + offset;\n"
    "\n"
    "    if (dist <= sizeMax) {\n"
    "        float alpha = smoothstep(sizeMax - 0.08, sizeMax, dist);\n"
    "        curColor = mix(curColor, color, base + alpha * alpha);\n"
    "    } else {\n"
    "        float alpha = 1 - smoothstep(sizeMax, sizeMax + 0.01, dist);\n"
    "        curColor = mix(curColor, color * (1.0 + base), alpha);\n"
    "    }\n"
    "\n"
    "    return curColor;\n"
    "}\n"
    "\n"
    "void main() {\n"
    "    vec4 color = backgroundColor;\n"
    "    color = draw_circle(color, membraneColor, membraneCenter, membraneSize, 0.008, 2, 0.5);\n"
    "    color = draw_circle(color, nucleusColor, nucleusCenter, nucleusSize, 0.01, 1, 0.2);\n"
    "\n"
    "    gl_FragColor = color;\n"
    "}\n"
#endif
;

/* ************************************************************************ */

struct Vertex
{
    GLfloat x, y;
    GLfloat u, v;
};

/* ************************************************************************ */

void DrawableCell::init(render::Context& context)
{
    m_buffer.init();

    m_vertexShader.init(render::Shader::Type::VERTEX, g_vertexShaderSrc, sizeof(g_vertexShaderSrc));
    m_fragmentShader.init(render::Shader::Type::FRAGMENT, g_fragmentShaderSrc, sizeof(g_fragmentShaderSrc));
    m_program.init(m_vertexShader, m_fragmentShader);

    // Get size variable
    m_uniformSize = m_program.getUniformId("g_size");

    const std::array<Vertex, 4> vertices = {{
        { 0.5f,  0.5f, 1.0f, 1.0f},
        { 0.5f, -0.5f, 1.0f, 0.0f},
        {-0.5f, -0.5f, 0.0f, 0.0f},
        {-0.5f,  0.5f, 0.0f, 1.0f}
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

void DrawableCell::draw(render::Context& context, float scale) noexcept
{
    gl(glUseProgram(m_program.getId()));
    gl(glActiveTexture(GL_TEXTURE0));

    // Set interpolate flag
    //gl(glUniform1f(m_uniformSize, scale));

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

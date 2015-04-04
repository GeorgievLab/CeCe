
/* ************************************************************************ */

// Declaration
#include "render/GridValue.hpp"

// C++
#include <array>
#include <vector>
#include <cassert>
#include <cmath>
#include <stdexcept>

// Simulator
#include "render/errors.hpp"

/* ************************************************************************ */

struct Vertex
{
    GLfloat x, y;
    GLfloat u, v;
};

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
    "uniform sampler2D data;\n"
    "uniform vec4 color = vec4(1, 1, 1, 1);\n"
    "const int COUNT = 9;\n"
    "const vec4 COLORS[COUNT] = vec4[COUNT](\n"
    "    vec4(0.208, 0.165, 0.529, 1.0), // '#352a87'\n"
    "    vec4(0.059, 0.361, 0.867, 1.0), // '#0f5cdd'\n"
    "    vec4(0.078, 0.506, 0.839, 1.0), // '#1481d6'\n"
    "    vec4(0.024, 0.643, 0.792, 1.0), // '#06a4ca'\n"
    "    vec4(0.180, 0.718, 0.643, 1.0), // '#2eb7a4'\n"
    "    vec4(0.529, 0.749, 0.467, 1.0), // '#87bf77'\n"
    "    vec4(0.820, 0.733, 0.349, 1.0), // '#d1bb59'\n"
    "    vec4(0.996, 0.784, 0.196, 1.0), // '#fec832'\n"
    "    vec4(0.976, 0.984, 0.055, 1.0)  // '#f9fb0e'\n"
    ");\n"
    "const float STEP = (1.0 / (COUNT - 1));\n"
    "void main() {\n"
    "    float value = texture2D(data, gl_TexCoord[0].xy).r;\n"
	//"    gl_FragColor = vec4(color.rgb * (1 - value), value);\n"
    "    int ix = int(value * (COUNT - 1));\n"
    "    vec4 thermal = mix(COLORS[ix], COLORS[ix + 1], (value - (ix * STEP)) / STEP);\n"
    "    gl_FragColor = thermal;\n"
    "}\n"
;

/* ************************************************************************ */

namespace render {

/* ************************************************************************ */

GridValue::~GridValue()
{
    // Delete program
    if (m_texture)
        gl(glDeleteTextures(1, &m_texture));
}

/* ************************************************************************ */

void GridValue::init(Vector<unsigned int> size, const float* data)
{
    Drawable::init();

    // Generate texture
    gl(glGenTextures(1, &m_texture));

    gl(glBindTexture(GL_TEXTURE_2D, m_texture));
    gl(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    gl(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

    m_vertexShader.init(Shader::Type::VERTEX, g_vertexShaderSrc, sizeof(g_vertexShaderSrc));
    m_fragmentShader.init(Shader::Type::FRAGMENT, g_fragmentShaderSrc, sizeof(g_fragmentShaderSrc));
    m_program.init(m_vertexShader, m_fragmentShader);

    // Fetch data pointers
    gl(m_dataPtr = glGetUniformLocation(m_program.getId(), "data"));
    gl(m_colorPtr = glGetUniformLocation(m_program.getId(), "color"));

    const std::array<Vertex, 4> vertices = {{
        { 0.5f,  0.5f, 1.0f, 1.0f},
        { 0.5f, -0.5f, 1.0f, 0.0f},
        {-0.5f, -0.5f, 0.0f, 0.0f},
        {-0.5f,  0.5f, 0.0f, 1.0f}
    }};

    assert(getBuffer() != 0);

    gl(glBindBuffer(GL_ARRAY_BUFFER, getBuffer()));
    gl(glBufferData(GL_ARRAY_BUFFER,
        vertices.size() * sizeof(decltype(vertices)::value_type),
        vertices.data(),
        GL_STATIC_DRAW
    ));

    resize(size, data);
}

/* ************************************************************************ */

void GridValue::render(const Vector<float>& scale) noexcept
{
    gl(glPushMatrix());
    gl(glScalef(scale.getX(), scale.getY(), 1));

    // Use texture
    gl(glEnable(GL_TEXTURE_2D));
    gl(glBindTexture(GL_TEXTURE_2D, m_texture));
    //glColor3f(1, 1, 1);

    gl(glUseProgram(m_program.getId()));
    gl(glUniform1i(m_dataPtr, 0));
    gl(glActiveTexture(GL_TEXTURE0));
    glUniform4f(m_colorPtr, 0, 0, 0, 1);

    // Bind buffer
    gl(glBindBuffer(GL_ARRAY_BUFFER, getBuffer()));
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

    gl(glBindTexture(GL_TEXTURE_2D, 0));
    gl(glDisable(GL_TEXTURE_2D));

    gl(glUseProgram(0));

    gl(glPopMatrix());
}

/* ************************************************************************ */

void GridValue::resize(Vector<unsigned int> size, const float* data)
{
    m_size = std::move(size);

    gl(glBindTexture(GL_TEXTURE_2D, m_texture));
    gl(glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, m_size.getWidth(), m_size.getHeight(), 0, GL_RED, GL_FLOAT, data));
    //gl(glGenerateMipmap(GL_TEXTURE_2D));
}

/* ************************************************************************ */

void GridValue::update(const float* data) noexcept
{
    gl(glBindTexture(GL_TEXTURE_2D, m_texture));
    gl(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_size.getWidth(), m_size.getHeight(), GL_RED, GL_FLOAT, data));
}

/* ************************************************************************ */

}

/* ************************************************************************ */

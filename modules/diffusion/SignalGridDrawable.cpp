
/* ************************************************************************ */

// Declaration
#include "SignalGridDrawable.hpp"

// C++
#include <array>
#include <vector>
#include <cassert>
#include <cmath>

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
    "uniform ivec2 size = ivec2(16, 16);\n"
    "uniform bool interpolate = false;\n"
    "vec4 get_pixel(vec2 pos) {\n"
    "    return texture2D(data, pos);\n"
    "}\n"
    "float triangular(float f) {\n"
    "    f = f * 0.5;\n"
    "    if (f < 0.0)\n"
    "        return (f + 1.0);\n"
    "    else\n"
    "        return (1.0 - f);\n"
    "    return 0.0;\n"
    "}\n"
    "\n"
    "vec4 interpolate_value(vec2 pos) {\n"
    "    vec2 texelSize = 1.0 / size;\n"
    "    vec4 sum = vec4(0.0);\n"
    "    vec4 denom = vec4(0.0);\n"
    "    vec2 ab = fract(pos * size);\n"
    "    \n"
    "    for (int m = -1; m <=2; m++) {\n"
    "        for (int n = -1; n <= 2; n++) {\n"
    "            vec2 mn = vec2(m, n);\n"
    "            vec4 pix = get_pixel(pos + vec2(texelSize * mn));\n"
    "            vec2 f = mn - ab;\n"
    "            float c = triangular(f.x) * triangular(f.y);\n"
    "            sum += pix * c;\n"
    "            denom += c;\n"
    "        }\n"
    "    }\n"
    "    return (sum / denom);\n"
    "}\n"
    "\n"
    "void main() {\n"
    "    if (interpolate)\n"
    "        gl_FragColor = interpolate_value(gl_TexCoord[0].xy);\n"
    "    else\n"
    "        gl_FragColor = get_pixel(gl_TexCoord[0].xy);\n"
    "}\n"
;

/* ************************************************************************ */

namespace module {
namespace diffusion {

/* ************************************************************************ */

SignalGridDrawable::~SignalGridDrawable()
{
    // Delete program
    if (m_texture)
        gl(glDeleteTextures(1, &m_texture));
}

/* ************************************************************************ */

void SignalGridDrawable::init(Vector<unsigned int> size, const Signal* data)
{
    render::Grid::init();

    m_buffer.init();

    // Generate texture
    gl(glGenTextures(1, &m_texture));

    gl(glBindTexture(GL_TEXTURE_2D, m_texture));
    gl(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    gl(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    gl(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    gl(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    m_vertexShader.init(render::Shader::Type::VERTEX, g_vertexShaderSrc, sizeof(g_vertexShaderSrc));
    m_fragmentShader.init(render::Shader::Type::FRAGMENT, g_fragmentShaderSrc, sizeof(g_fragmentShaderSrc));
    m_program.init(m_vertexShader, m_fragmentShader);

    // Fetch data pointers
    gl(m_sizePtr = glGetUniformLocation(m_program.getId(), "size"));
    gl(m_interpolatePtr = glGetUniformLocation(m_program.getId(), "interpolate"));

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

    resize(size, data);
}

/* ************************************************************************ */

void SignalGridDrawable::draw(const Vector<float>& scale) noexcept
{
    gl(glPushMatrix());
    gl(glScalef(scale.getX(), scale.getY(), 1));

    // Use texture
    gl(glEnable(GL_TEXTURE_2D));
    gl(glBindTexture(GL_TEXTURE_2D, m_texture));

    gl(glUseProgram(m_program.getId()));
    gl(glActiveTexture(GL_TEXTURE0));

    // Set size
    gl(glUniform2i(m_sizePtr, getWidth(), getHeight()));

    // Set interpolate flag
    gl(glUniform1i(m_interpolatePtr, m_interpolate));

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

    gl(glBindTexture(GL_TEXTURE_2D, 0));
    gl(glDisable(GL_TEXTURE_2D));

    gl(glUseProgram(0));

    gl(glPopMatrix());

    // Render grid
    render::Grid::draw(scale, {1, 0, 0, 1});
}

/* ************************************************************************ */

void SignalGridDrawable::resize(Vector<unsigned int> size, const Signal* data)
{
    render::Grid::resize(std::move(size));

    // Resize texture buffer
    m_textureData.resize(getWidth() * getHeight());

    gl(glBindTexture(GL_TEXTURE_2D, m_texture));
    gl(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, getWidth(), getHeight(), 0, GL_RGBA, GL_FLOAT, updateTextureData(data)));
    //gl(glGenerateMipmap(GL_TEXTURE_2D));
}

/* ************************************************************************ */

void SignalGridDrawable::update(const Signal* data) noexcept
{
    gl(glBindTexture(GL_TEXTURE_2D, m_texture));
    gl(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, getWidth(), getHeight(), GL_RGBA, GL_FLOAT, updateTextureData(data)));
}

/* ************************************************************************ */

render::Color* SignalGridDrawable::updateTextureData(const Signal* data) noexcept
{
    // Update all pixels
    for (auto it = m_textureData.begin(), ite = m_textureData.end(); it != ite; ++it, ++data)
    {
        // Pixel alias
        auto& pixel = *it;

        // Set initial value as background
        pixel = m_background;

        // Mixup signal colors
        for (unsigned int i = 0; i < Signal::COUNT; ++i)
        {
            pixel += m_colors[i % m_colors.size()] * (*data)[i];
        }
    }

    return m_textureData.data();
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

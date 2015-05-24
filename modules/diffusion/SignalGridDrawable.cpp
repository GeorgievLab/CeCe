
/* ************************************************************************ */

// Declaration
#include "SignalGridDrawable.hpp"

// C++
#include <array>
#include <vector>
#include <cassert>
#include <cmath>

// OpenGL
#ifdef __linux__
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#elif defined(_WIN32)
#include <GL/gl.h>
#include "render/glext.h"
#pragma comment(lib, "opengl32.lib")
#endif

// Simulator
#include "render/errors.hpp"
#include "render/Context.hpp"

/* ************************************************************************ */

#ifdef _WIN32
static PFNGLACTIVETEXTUREPROC glActiveTexture = nullptr;
#endif

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

static const std::array<Vertex, 4> g_vertices = {{
    { 0.5f,  0.5f, 1.0f, 1.0f},
    { 0.5f, -0.5f, 1.0f, 0.0f},
    {-0.5f, -0.5f, 0.0f, 0.0f},
    {-0.5f,  0.5f, 0.0f, 1.0f}
}};

/* ************************************************************************ */

namespace module {
namespace diffusion {

/* ************************************************************************ */

SignalGridDrawable::SignalGridDrawable(render::Context& context, core::Vector<unsigned int> size, const Signal* data)
    : m_buffer(context, g_vertices.size() * sizeof(decltype(g_vertices)::value_type), g_vertices.data())
{
	m_colors.push_back(render::colors::CYAN);
	m_colors.push_back(render::colors::MAGENTA);
	m_colors.push_back(render::colors::YELLOW);
	m_colors.push_back(render::colors::BLUE);
	m_colors.push_back(render::colors::RED);
	m_colors.push_back(render::colors::GREEN);
	m_colors.push_back(render::Color{ 1, 0.894f, 0.769f });

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
    m_sizePtr = m_program.getUniformId("size");
	m_interpolatePtr = m_program.getUniformId("interpolate");

    resize(size, data);
}

/* ************************************************************************ */

SignalGridDrawable::~SignalGridDrawable()
{
    // Delete program
    if (m_texture)
        gl(glDeleteTextures(1, &m_texture));
}

/* ************************************************************************ */

void SignalGridDrawable::draw(render::Context& context) NOEXCEPT
{
    // Use texture
    gl(glEnable(GL_TEXTURE_2D));
    gl(glBindTexture(GL_TEXTURE_2D, m_texture));

#ifdef _WIN32
	if (!glActiveTexture)
		glActiveTexture = (PFNGLACTIVETEXTUREPROC)wglGetProcAddress("glActiveTexture");
#endif

	context.setProgram(&m_program);
    gl(glActiveTexture(GL_TEXTURE0));

    // Set size
	context.setProgramParam(m_sizePtr, getSize().getWidth(), getSize().getHeight());

    // Set interpolate flag
	context.setProgramParam(m_interpolatePtr, m_interpolate);

    // Bind buffer
	context.setVertexBuffer(&m_buffer);

    gl(glEnableClientState(GL_VERTEX_ARRAY));
    gl(glEnableClientState(GL_TEXTURE_COORD_ARRAY));
    gl(glVertexPointer(2, GL_FLOAT, sizeof(Vertex), 0));
    gl(glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), reinterpret_cast<const void*>(2 * sizeof(GLfloat))));

    // Draw circle
    gl(glDrawArrays(GL_QUADS, 0, 4));

    // Disable states
    gl(glDisableClientState(GL_TEXTURE_COORD_ARRAY));
    gl(glDisableClientState(GL_VERTEX_ARRAY));
    
	context.setVertexBuffer(nullptr);

    gl(glBindTexture(GL_TEXTURE_2D, 0));
    gl(glDisable(GL_TEXTURE_2D));

	context.setProgram(nullptr);
}

/* ************************************************************************ */

void SignalGridDrawable::resize(core::Vector<unsigned int> size, const Signal* data)
{
    m_size = std::move(size);

    // Resize texture buffer
    m_textureData.resize(getSize().getWidth() * getSize().getHeight());

    gl(glBindTexture(GL_TEXTURE_2D, m_texture));
    gl(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, getSize().getWidth(), getSize().getHeight(), 0, GL_RGBA, GL_FLOAT, updateTextureData(data)));
    //gl(glGenerateMipmap(GL_TEXTURE_2D));
}

/* ************************************************************************ */

void SignalGridDrawable::update(const Signal* data) NOEXCEPT
{
    gl(glBindTexture(GL_TEXTURE_2D, m_texture));
    gl(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, getSize().getWidth(), getSize().getHeight(), GL_RGBA, GL_FLOAT, updateTextureData(data)));
}

/* ************************************************************************ */

render::Color* SignalGridDrawable::updateTextureData(const Signal* data) NOEXCEPT
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
            pixel *= (1 - (*data)[i]);
            pixel += m_colors[i % m_colors.size()] * (*data)[i];
        }
    }

    return m_textureData.data();
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

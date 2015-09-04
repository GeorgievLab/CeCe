/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */

// Declaration
#include "render/Font.hpp"

// OpenGL
#ifdef __linux__
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#elif defined(_WIN32)
#include <GL/gl.h>
#include "render/glext.h"
#ifdef _MSC_VER
#pragma comment(lib, "opengl32.lib")
#endif
#elif __APPLE__ && __MACH__
#include <OpenGL/gl.h>
#endif

// FreeType
#include <ft2build.h>
#include FT_FREETYPE_H

// Simulator
#include "core/UniquePtr.hpp"
#include "core/Units.hpp"
#include "render/Context.hpp"
#include "render/VertexFormat.hpp"

/* ************************************************************************ */

namespace render {

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

/**
 * @brief FreeType library deleter.
 *
 * @param ptr
 */
void libraryDeleter(FT_LibraryRec_* ptr)
{
    FT_Done_FreeType(ptr);
}

/* ************************************************************************ */

UniquePtr<FT_LibraryRec_, decltype(&libraryDeleter)> g_library(nullptr, &libraryDeleter);

/* ************************************************************************ */

// Vertex structure
struct Vertex
{
    float x, y;
};

/* ************************************************************************ */

const StaticArray<Vertex, 4> g_vertices = {{
    { 0.0f, 0.0f},
    { 0.0f, 1.0f},
    { 1.0f, 0.0f},
    { 1.0f, 1.0f}
}};

/* ************************************************************************ */

/// Vertex shader
const char g_vertexShader[] =
    "#version 120\n"
    "\n"
    "attribute vec4 coord;\n"
    "varying vec2 texcoord;\n"
    "\n"
    "void main(void) {\n"
    "    gl_Position = vec4(coord.xy, 0, 1);\n"
    "    texcoord = coord.zw;\n"
    "}\n"
;

/* ************************************************************************ */

/// Fragment shader
const char g_fragmentShader[] =
    "#version 120\n"
    "\n"
    "varying vec2 texcoord;\n"
    "uniform sampler2D tex;\n"
    "uniform vec4 color;\n"
    "\n"
    "void main(void) {\n"
    "    gl_FragColor = vec4(1, 1, 1, texture2D(tex, texcoord).r) * color;\n"
    "}\n"
;

/* ************************************************************************ */

}

/* ************************************************************************ */

struct Font::Impl
{
    FT_Face face;
};

/* ************************************************************************ */

Font::Font(Context& context, const String& name)
    : m_impl(new Impl{})
    , m_buffer(context, g_vertices.size() * sizeof(decltype(g_vertices)::value_type), g_vertices.data())
    , m_texture(context)
{
    if (!g_library)
    {
        FT_LibraryRec_* lib;

        if (FT_Init_FreeType(&lib))
            throw RuntimeException("Could not init freetype library");

        g_library.reset(lib);
    }

    if (FT_New_Face(g_library.get(), name.c_str(), 0, &m_impl->face))
        throw RuntimeException("Could not open font: " + name);

    m_vertexShader.init(render::Shader::Type::VERTEX, g_vertexShader, sizeof g_vertexShader);
    m_fragmentShader.init(render::Shader::Type::FRAGMENT, g_fragmentShader, sizeof g_fragmentShader);
    m_program.init(m_vertexShader, m_fragmentShader);

    m_colorPtr = m_program.getUniformId("g_color");
}

/* ************************************************************************ */

Font::~Font()
{
    FT_Done_Face(m_impl->face);
}

/* ************************************************************************ */

void Font::setSize(unsigned int size) noexcept
{
    FT_Set_Pixel_Sizes(m_impl->face, 0, size);
}

/* ************************************************************************ */

void Font::draw(Context& context, const String& text, const Color& color, const Vector<float> pos) noexcept
{
    static render::VertexFormat vformat{
        render::VertexElement(render::VertexElementType::Position, render::DataType::Float, 2)
    };

    float x = pos.getX();
    float y = pos.getY();
    float sx = 0.01;
    float sy = 0.01;

    context.matrixProjection();
    context.matrixPush();
    //context.matrixIdentity();

    context.matrixView();
    context.matrixPush();

    // Translate to initial position
    context.matrixTranslate(pos * units::Length(1));

    // Set parameters
    context.setVertexBuffer(&m_buffer);
    context.setVertexFormat(&vformat);
    context.setTexture(&m_texture);

    context.setProgram(&m_program);
    context.setProgramParam(m_colorPtr, color);

    FT_GlyphSlot glyph = m_impl->face->glyph;

    for (auto c : text)
    {
        if (FT_Load_Char(m_impl->face, c, FT_LOAD_RENDER))
            continue;

        // TODO: replace by render::Texture::<something>
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            glyph->bitmap.width,
            glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            glyph->bitmap.buffer
        );

        float x2 = x + glyph->bitmap_left * sx;
        float y2 = -y - glyph->bitmap_top * sy;
        float w = glyph->bitmap.width * sx;
        float h = glyph->bitmap.rows * sy;

        // Draw
        context.draw(PrimitiveType::Quads, 4);

        // Translate to next position
        context.matrixTranslate({
            units::Length((glyph->advance.x >> 6) * sx),
            units::Length((glyph->advance.y >> 6) * sy)
        });
    }

    context.setProgram(nullptr);

    // Unset parameters
    context.setTexture(nullptr);
    context.setVertexFormat(nullptr);
    context.setVertexBuffer(nullptr);

    context.matrixPop();

    context.matrixProjection();
    context.matrixPop();

    context.matrixView();
}

/* ************************************************************************ */

}

/* ************************************************************************ */

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
#include "cece/render/Font.hpp"

// OpenGL
#ifdef __linux__
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#elif defined(_WIN32)
#include <GL/gl.h>
#include "cece/render/glext.h"
#ifdef _MSC_VER
#pragma comment(lib, "opengl32.lib")
#endif
#elif __APPLE__ && __MACH__
#include <OpenGL/gl.h>
#endif

// FreeType
#include <ft2build.h>
#include FT_FREETYPE_H

// CeCe
#include "cece/core/UniquePtr.hpp"
#include "cece/core/Units.hpp"
#include "cece/render/Context.hpp"
#include "cece/render/VertexFormat.hpp"

/* ************************************************************************ */

namespace cece {
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
    float u, v;
};

/* ************************************************************************ */

const StaticArray<Vertex, 4> g_vertices = {{
    { 0.0f, 0.0f, 0.0f, 0.0f},
    { 1.0f, 0.0f, 1.0f, 0.0f},
    { 1.0f, 1.0f, 1.0f, 1.0f},
    { 0.0f, 1.0f, 0.0f, 1.0f}
}};

/* ************************************************************************ */

/// Vertex shader
const char g_vertexShader[] =
    "#version 120\n"
    "\n"
    "void main(void) {\n"
    "    gl_Position = ftransform();\n"
    "    gl_TexCoord[0] = gl_MultiTexCoord0;\n"
    "}\n"
;

/* ************************************************************************ */

/// Fragment shader
const char g_fragmentShader[] =
    "#version 120\n"
    "\n"
    "uniform sampler2D g_tex;\n"
    "uniform vec4 g_color;\n"
    "\n"
    "void main(void) {\n"
    "    gl_FragColor = vec4(1, 1, 1, texture2D(g_tex, gl_TexCoord[0].xy).r) * g_color;\n"
    "}\n"
;

/* ************************************************************************ */

}

/* ************************************************************************ */

struct Font::Impl
{
    FT_Face face;
    unsigned int size = 0;
};

/* ************************************************************************ */

Font::Font(Context& context, const String& name)
    : m_impl(new Impl{})
    , m_buffer(context, g_vertices.size() * sizeof(decltype(g_vertices)::value_type), g_vertices.data())
    , m_texture(context, false)
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

    m_texPtr = m_program.getUniformId("g_tex");
    m_colorPtr = m_program.getUniformId("g_color");
}

/* ************************************************************************ */

Font::Font(Context& context, const unsigned char* data, unsigned int size)
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

    if (FT_New_Memory_Face(g_library.get(), data, size, 0, &m_impl->face))
        throw RuntimeException("Could not open font from memory");

    m_vertexShader.init(render::Shader::Type::VERTEX, g_vertexShader, sizeof g_vertexShader);
    m_fragmentShader.init(render::Shader::Type::FRAGMENT, g_fragmentShader, sizeof g_fragmentShader);
    m_program.init(m_vertexShader, m_fragmentShader);

    m_texPtr = m_program.getUniformId("g_tex");
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
    m_impl->size = size;
}

/* ************************************************************************ */

void Font::draw(Context& context, const String& text, const Color& color, const Vector<float> pos) noexcept
{
    static render::VertexFormat vformat{
        render::VertexElement(render::VertexElementType::Position, render::DataType::Float, 2),
        render::VertexElement(render::VertexElementType::TexCoord, render::DataType::Float, 2)
    };

    // Viewport size
    const auto size = context.getSize();

    const float sx = 1.0 / size.getWidth();
    const float sy = 1.0 / size.getHeight();

    //context.setWireframe(true);

    context.matrixProjection();
    context.matrixPush();
    context.matrixIdentity();

    context.matrixView();
    context.matrixPush();
    context.matrixIdentity();
    context.matrixTranslate({units::Length(-1.f), units::Length(1.f - sy * m_impl->size)});

    // Translate to initial position
    context.matrixTranslate(pos * units::Length(1));

    // Set parameters
    context.setVertexBuffer(&m_buffer);
    context.setVertexFormat(&vformat);
    context.setTexture(&m_texture);

    context.setProgram(&m_program);
    context.setProgramParam(m_texPtr, 0);
    context.setProgramParam(m_colorPtr, color);

    context.colorPush();
    context.enableAlpha();

    FT_GlyphSlot glyph = m_impl->face->glyph;

    for (auto c : text)
    {
        if (FT_Load_Char(m_impl->face, c, FT_LOAD_RENDER))
            continue;

        // Recreate texture
        m_texture.createGray({
            static_cast<unsigned>(glyph->bitmap.width),
            static_cast<unsigned>(glyph->bitmap.rows)
        }, glyph->bitmap.buffer);

        // Transformation for glyph
        context.matrixPush();

        context.matrixTranslate({
            units::Length(glyph->bitmap_left * sx),
            units::Length(glyph->bitmap_top * sy)
        });

        context.matrixScale({
            glyph->bitmap.width * sx,
            -(glyph->bitmap.rows * sy)
        });

        // Draw
        context.draw(PrimitiveType::Quads, 4);
        context.matrixPop();

        // Translate to next position
        context.matrixTranslate({
            units::Length((glyph->advance.x >> 6) * sx),
            units::Length((glyph->advance.y >> 6) * sy)
        });
    }

    context.disableAlpha();
    context.colorPop();

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
}

/* ************************************************************************ */

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
#include "cece/render/Context.hpp"

// C++
#include <cmath>
#include <cassert>
#include <algorithm>

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

// CeCe
#include "cece/render/Color.hpp"
#include "cece/render/Buffer.hpp"
#include "cece/render/errors.hpp"
#include "cece/render/Object.hpp"
#include "cece/render/VertexFormat.hpp"
#include "cece/render/Texture.hpp"

/* ************************************************************************ */

#ifdef _WIN32
static PFNGLBINDBUFFERPROC glBindBuffer = nullptr;
static PFNGLUSEPROGRAMPROC glUseProgram = nullptr;
static PFNGLUNIFORM1IPROC glUniform1i = nullptr;
static PFNGLUNIFORM2IPROC glUniform2i = nullptr;
static PFNGLUNIFORM1FPROC glUniform1f = nullptr;
static PFNGLUNIFORM4FPROC glUniform4f = nullptr;
static PFNGLBLENDFUNCSEPARATEPROC glBlendFuncSeparate = nullptr;
#endif

/* ************************************************************************ */

namespace cece {
namespace render {

/* ************************************************************************ */

/**
 * @brief Convert primitive type to OpenGL.
 *
 * @param type
 *
 * @return
 */
static GLenum convert(PrimitiveType type) noexcept
{
    switch (type)
    {
    case PrimitiveType::Lines:          return GL_LINES;
    case PrimitiveType::Triangles:      return GL_TRIANGLES;
    case PrimitiveType::Quads:          return GL_QUADS;
    case PrimitiveType::TriangleFan:    return GL_TRIANGLE_FAN;
    }

    return 0;
}

/* ************************************************************************ */

/**
 * @brief Convert data type.
 *
 * @param type
 *
 * @return
 */
GLenum convert(DataType type)
{
    // GL_SHORT, GL_INT, GL_FLOAT, or GL_DOUBLE

    switch (type)
    {
    default: break;
    case DataType::Byte:    return GL_BYTE;
    case DataType::Ubyte:   return GL_UNSIGNED_BYTE;
    case DataType::Short:   return GL_SHORT;
    case DataType::Ushort:  return GL_UNSIGNED_SHORT;
    case DataType::Int:     return GL_INT;
    case DataType::Uint:    return GL_UNSIGNED_INT;
    case DataType::Float:   return GL_FLOAT;
    case DataType::Double:  return GL_DOUBLE;
    }

    return GL_NONE;
}

/* ************************************************************************ */

Context::Context()
{
    // Nothing to do
}

/* ************************************************************************ */

Context::~Context()
{
    // Nothing to do
}

/* ************************************************************************ */

Size Context::getSize() const noexcept
{
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    const auto width = viewport[2];
    const auto height = viewport[3];

    return Size(width, height);
}

/* ************************************************************************ */

void Context::getData(void* data, bool alpha, bool bgra) const noexcept
{
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    const auto x = viewport[0];
    const auto y = viewport[1];
    const auto width = viewport[2];
    const auto height = viewport[3];

    gl(glPixelStorei(GL_PACK_ALIGNMENT, alpha ? 4 : 1));
    gl(glPixelStorei(GL_PACK_ROW_LENGTH, width));

    const GLenum type = alpha
        ? (bgra ? GL_BGRA : GL_RGBA)
        : (bgra ? GL_BGR : GL_RGB)
    ;

    gl(glReadBuffer(GL_FRONT));
    gl(glReadPixels(x, y, width, height, type, GL_UNSIGNED_BYTE, data));
}

/* ************************************************************************ */

ImageData Context::getData(bool alpha, bool bgra) const noexcept
{
    ImageData result;

    result.size = getSize();
    result.data.resize((alpha ? 4 : 3) * result.size.getWidth() * result.size.getHeight());

    getData(result.data.data(), alpha, bgra);

    return result;
}

/* ************************************************************************ */

bool Context::isWireframe() const noexcept
{
    GLint val;
    glGetIntegerv(GL_POLYGON_MODE, &val);
    return val == GL_LINE;
}

/* ************************************************************************ */

void Context::setWireframe(bool flag) noexcept
{
    if (flag)
    {
        gl(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
    }
    else
    {
        gl(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
    }
}

/* ************************************************************************ */

void Context::init(const render::Color& color) noexcept
{
    assert(!isInitialized());

    // Clear color.
    glClearColor(color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);

    m_isInitialized = true;
}

/* ************************************************************************ */

void Context::setView(int width, int height) noexcept
{
    if (!isInitialized())
        return;

     // Setup viewport (whole window)
    glViewport(0, 0, width, height);

    // Setup projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    const GLdouble wh = width / 2.0;
    const GLdouble hh = height / 2.0;

    glOrtho(-wh, wh, -hh, hh, -1000, 1000);
    //glRotatef(90, 1, 0, 0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Apply zoom matrix
    float scale = 1.0 / m_camera.getZoom();
    glScalef(scale, scale, scale);

    // Move camera
    glTranslatef(m_camera.getPosition().getX().value(), m_camera.getPosition().getY().value(), 0);

    // Rotate camera
    //glRotatef(m_camera.getRotation(), 0, 1, 0);
}

/* ************************************************************************ */

void Context::setStencilBuffer(float width, float height) noexcept
{
    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_STENCIL_TEST);

    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDepthMask(GL_FALSE);
    glStencilFunc(GL_NEVER, 1, 0xFF);
    glStencilOp(GL_REPLACE, GL_KEEP, GL_KEEP);

    // draw stencil pattern
    glStencilMask(0xFF);
    glClear(GL_STENCIL_BUFFER_BIT);

    float wh = width * 0.5f;
    float hh = height * 0.5f;

    glBegin(GL_QUADS);
    glVertex2f(-wh, -hh);
    glVertex2f( wh, -hh);
    glVertex2f( wh,  hh);
    glVertex2f(-wh,  hh);
    glEnd();

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDepthMask(GL_TRUE);
    glStencilMask(0x00);
    glStencilFunc(GL_EQUAL, 0, 0xFF);
    glStencilFunc(GL_EQUAL, 1, 0xFF);
}

/* ************************************************************************ */

void Context::disableStencilBuffer() noexcept
{
    gl(glDisable(GL_STENCIL_TEST));
}

/* ************************************************************************ */

void Context::frameBegin(int width, int height) noexcept
{
    assert(isInitialized());

    // Clear
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    setView(width, height);
}

/* ************************************************************************ */

void Context::frameEnd() noexcept
{
    assert(isInitialized());
    glFlush();
}

/* ************************************************************************ */

void Context::enableAlpha() noexcept
{
#if _WIN32
    if (!glBlendFuncSeparate)
        glBlendFuncSeparate = (PFNGLBLENDFUNCSEPARATEPROC)wglGetProcAddress("glBlendFuncSeparate");
#endif

    gl(glEnable(GL_BLEND));
    //gl(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    gl(glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE));
}

/* ************************************************************************ */

void Context::disableAlpha() noexcept
{
    gl(glDisable(GL_BLEND));
}

/* ************************************************************************ */

void Context::colorPush() noexcept
{
    gl(glPushAttrib(GL_COLOR_BUFFER_BIT));
}

/* ************************************************************************ */

void Context::colorPop() noexcept
{
    gl(glPopAttrib());
}

/* ************************************************************************ */

void Context::matrixProjection() noexcept
{
    gl(glMatrixMode(GL_PROJECTION));
}

/* ************************************************************************ */

void Context::matrixView() noexcept
{
    gl(glMatrixMode(GL_MODELVIEW));
}

/* ************************************************************************ */

void Context::matrixTexture() noexcept
{
    gl(glMatrixMode(GL_TEXTURE));
}

/* ************************************************************************ */

void Context::matrixPush() noexcept
{
    gl(glPushMatrix());
}

/* ************************************************************************ */

void Context::matrixPop() noexcept
{
    gl(glPopMatrix());
}

/* ************************************************************************ */

void Context::matrixIdentity() noexcept
{
    gl(glLoadIdentity());
}

/* ************************************************************************ */

void Context::matrixTranslate(const PositionVector& pos) noexcept
{
    gl(glTranslatef(pos.getX().value(), pos.getY().value(), 0));
}

/* ************************************************************************ */

void Context::matrixScale(const ScaleVector& scale) noexcept
{
    gl(glScalef(scale.getX(), scale.getY(), 1));
}

/* ************************************************************************ */

void Context::matrixRotate(units::Angle angle) noexcept
{
    gl(glRotatef(units::rad2deg(angle.value()), 0.f, 0.f, 1.f));
}

/* ************************************************************************ */

void Context::setColor(const Color& color) noexcept
{
    gl(glColor4f(color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha()));
}

/* ************************************************************************ */

void Context::setVertexBuffer(Buffer* buffer) noexcept
{
#ifdef _WIN32
    if (!glBindBuffer)
        glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
#endif

    // Bind buffer
    gl(glBindBuffer(GL_ARRAY_BUFFER, buffer ? buffer->getId() : 0));
}

/* ************************************************************************ */

void Context::setVertexFormat(const VertexFormat* format) noexcept
{
    if (format)
    {
        size_t offset = 0;
        const unsigned char* ptr = nullptr;
        const auto size = format->getSize();

        for (const auto& e : *format)
        {
            switch (e.getType())
            {
            default:
                break;

            case render::VertexElementType::Position:
                gl(glEnableClientState(GL_VERTEX_ARRAY));
                gl(glVertexPointer(e.getCount(), convert(e.getDataType()), static_cast<GLsizei>(size), ptr + offset));
                //glVertexAttribPointer(i, e.getCount(), Convert(e.getDataType()), GL_FALSE, size, reinterpret_cast<void*>(offset));
                break;

            case render::VertexElementType::Color:
                gl(glEnableClientState(GL_COLOR_ARRAY));
                gl(glColorPointer(e.getCount(), convert(e.getDataType()), static_cast<GLsizei>(size), ptr + offset));
                //glVertexAttribPointer(i, e.getCount(), Convert(e.getDataType()), GL_TRUE, size, reinterpret_cast<void*>(offset));
                break;

            case render::VertexElementType::TexCoord:
                gl(glEnableClientState(GL_TEXTURE_COORD_ARRAY));
                gl(glTexCoordPointer(e.getCount(), convert(e.getDataType()), static_cast<GLsizei>(size), ptr + offset));
                break;

            }

            // Add size
            offset += e.getSize();
        }
    }
    else
    {
        gl(glDisableClientState(GL_COLOR_ARRAY));
        gl(glDisableClientState(GL_TEXTURE_COORD_ARRAY));
        gl(glDisableClientState(GL_VERTEX_ARRAY));
    }
}

/* ************************************************************************ */

void Context::setTexture(Texture* texture) noexcept
{
    if (texture)
    {
        // Use texture
        gl(glEnable(GL_TEXTURE_2D));
        gl(glBindTexture(GL_TEXTURE_2D, texture->getId()));
    }
    else
    {
        gl(glBindTexture(GL_TEXTURE_2D, 0));
        gl(glDisable(GL_TEXTURE_2D));
    }
}

/* ************************************************************************ */

void Context::setProgram(Program* program) noexcept
{
#ifdef _WIN32
    if (!glUseProgram)
        glUseProgram = (PFNGLUSEPROGRAMPROC) wglGetProcAddress("glUseProgram");
#endif

    gl(glUseProgram(program ? program->getId() : 0));
}

/* ************************************************************************ */

void Context::setProgramParam(Program::UniformId id, bool value) noexcept
{
#ifdef _WIN32
    if (!glUniform1i)
        glUniform1i = (PFNGLUNIFORM1IPROC) wglGetProcAddress("glUniform1i");
#endif

    gl(glUniform1i(id, int(value)));
}

/* ************************************************************************ */

void Context::setProgramParam(Program::UniformId id, int value) noexcept
{
#ifdef _WIN32
    if (!glUniform1i)
        glUniform1i = (PFNGLUNIFORM1IPROC) wglGetProcAddress("glUniform1i");
#endif

    gl(glUniform1i(id, value));
}

/* ************************************************************************ */

void Context::setProgramParam(Program::UniformId id, unsigned int value) noexcept
{
#ifdef _WIN32
    if (!glUniform1i)
        glUniform1i = (PFNGLUNIFORM1IPROC) wglGetProcAddress("glUniform1i");
#endif

    gl(glUniform1i(id, value));
}

/* ************************************************************************ */

void Context::setProgramParam(Program::UniformId id, int value1, int value2) noexcept
{
#ifdef _WIN32
    if (!glUniform2i)
        glUniform2i = (PFNGLUNIFORM2IPROC)wglGetProcAddress("glUniform2i");
#endif

    gl(glUniform2i(id, value1, value2));
}

/* ************************************************************************ */

void Context::setProgramParam(Program::UniformId id, float value) noexcept
{
#ifdef _WIN32
    if (!glUniform1f)
        glUniform1f = (PFNGLUNIFORM1FPROC)wglGetProcAddress("glUniform1f");
#endif

    gl(glUniform1f(id, value));
}

/* ************************************************************************ */

void Context::setProgramParam(Program::UniformId id, double value) noexcept
{
#ifdef _WIN32
    if (!glUniform1f)
        glUniform1f = (PFNGLUNIFORM1FPROC)wglGetProcAddress("glUniform1f");
#endif

    gl(glUniform1f(id, value));
}

/* ************************************************************************ */

void Context::setProgramParam(Program::UniformId id, long double value) noexcept
{
#ifdef _WIN32
    if (!glUniform1f)
        glUniform1f = (PFNGLUNIFORM1FPROC)wglGetProcAddress("glUniform1f");
#endif

    gl(glUniform1f(id, value));
}

/* ************************************************************************ */

void Context::setProgramParam(Program::UniformId id, const render::Color& color) noexcept
{
#ifdef _WIN32
    if (!glUniform4f)
        glUniform4f = (PFNGLUNIFORM4FPROC)wglGetProcAddress("glUniform4f");
#endif

    gl(glUniform4f(id, color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha()));
}

/* ************************************************************************ */

void Context::draw(PrimitiveType type, unsigned int count, unsigned int offset)
{
    gl(glDrawArrays(convert(type), offset, count));
}

/* ************************************************************************ */

void Context::drawLine(Vector<float> pos, Vector<float> size, const Color& color)
{
    if (size == Zero)
        return;

    const auto pos2 = pos + size;

    glPushAttrib(GL_CURRENT_BIT);

    glColor4f(color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());

    glBegin(GL_LINES);
    glVertex2f(pos.getX(), pos.getY());
    glVertex2f(pos2.getX(), pos2.getY());
    glEnd();

    glPopAttrib();
}

/* ************************************************************************ */

Object* Context::addObject(Object* obj)
{
    m_objects.emplace_back(obj);
    return m_objects.back().get();
}

/* ************************************************************************ */

void Context::deleteObject(const Object* obj)
{
    using std::begin;
    using std::end;

    auto it = std::find_if(begin(m_objects), end(m_objects), [obj](const std::unique_ptr<Object>& ptr) {
        return ptr.get() == obj;
    });

    // Not found
    if (it != end(m_objects))
    {
        // Move object
        m_releasedObjects.push_back(std::move(*it));
        m_objects.erase(it);
    }
}

/* ************************************************************************ */

void Context::deleteReleasedObjects()
{
    m_releasedObjects.clear();
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */


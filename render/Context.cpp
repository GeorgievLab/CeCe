
/* ************************************************************************ */

#if !ENABLE_RENDER
#error ENABLE_RENDER must be 1
#endif

/* ************************************************************************ */

// Declaration
#include "render/Context.hpp"

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
#include "render/glext.h"
#pragma comment(lib, "opengl32.lib")
#endif

// Simulator
#include "render/Color.hpp"
#include "render/Buffer.hpp"
#include "render/errors.hpp"
#include "render/Object.hpp"
#include "render/VertexFormat.hpp"
#include "render/Texture.hpp"

/* ************************************************************************ */

#ifdef _WIN32
static PFNGLBINDBUFFERPROC glBindBuffer = nullptr;
static PFNGLUSEPROGRAMPROC glUseProgram = nullptr;
static PFNGLUNIFORM1IPROC glUniform1i = nullptr;
static PFNGLUNIFORM2IPROC glUniform2i = nullptr;
static PFNGLUNIFORM1FPROC glUniform1f = nullptr;
#endif

/* ************************************************************************ */

namespace render {

/* ************************************************************************ */

/**
 * @brief Convert primitive type to OpenGL.
 *
 * @param type
 *
 * @return
 */
static GLenum convert(PrimitiveType type) NOEXCEPT
{
    switch (type)
    {
    case PrimitiveType::Lines:      return GL_LINES;
    case PrimitiveType::Triangles:  return GL_TRIANGLES;
    case PrimitiveType::Quads:      return GL_QUADS;
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

std::pair<std::vector<std::uint8_t>, core::Vector<unsigned>> Context::getData() const NOEXCEPT
{
    std::pair<std::vector<std::uint8_t>, core::Vector<unsigned>> result;

    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    auto x = viewport[0];
    auto y = viewport[1];
    auto width = viewport[2];
    auto height = viewport[3];

    result.first.resize(3 * width * height);
    result.second = core::Vector<unsigned>(width, height);

    glReadBuffer(GL_FRONT);
    glReadPixels(x, y, width, height, GL_RGB, GL_UNSIGNED_BYTE, result.first.data());

    return result;
}

/* ************************************************************************ */

bool Context::isWireframe() const NOEXCEPT
{
    GLint val;
    glGetIntegerv(GL_POLYGON_MODE, &val);
    return val == GL_LINE;
}

/* ************************************************************************ */

void Context::setWireframe(bool flag) NOEXCEPT
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

void Context::init() NOEXCEPT
{
    assert(!isInitialized());

    // Clear color.
    glClearColor(1.f, 1.f, 1.f, 1.f);
    //glClearColor(0.f, 0.f, 0.f, 1.f);

    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);

    m_is_init = true;
}

/* ************************************************************************ */

void Context::setView(int width, int height) NOEXCEPT
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

    // Move camera
    glTranslatef(m_camera.getPosition().getX(), m_camera.getPosition().getY(), 0);

    // Apply zoom matrix
    float scale = 1 / m_camera.getZoom();
    glScalef(scale, scale, scale);

    // Rotate camera
    //glRotatef(m_camera.getRotation(), 0, 1, 0);
}

/* ************************************************************************ */

void Context::setStencilBuffer(float width, float height) NOEXCEPT
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

    //glDisable(GL_STENCIL_TEST);
}

/* ************************************************************************ */

void Context::frameBegin(int width, int height) NOEXCEPT
{
    assert(isInitialized());

    // Clear
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_BLEND);

    setView(width, height);
}

/* ************************************************************************ */

void Context::frameEnd() NOEXCEPT
{
    assert(isInitialized());

    glDisable(GL_BLEND);

    glFlush();
}

/* ************************************************************************ */

void Context::matrixPush() NOEXCEPT
{
    gl(glPushMatrix());
}

/* ************************************************************************ */

void Context::matrixPop() NOEXCEPT
{
    gl(glPopMatrix());
}

/* ************************************************************************ */

void Context::matrixIdentity() NOEXCEPT
{
    gl(glLoadIdentity());
}

/* ************************************************************************ */

void Context::matrixTranslate(const core::PositionVector& pos) NOEXCEPT
{
    gl(glTranslatef(pos.getX(), pos.getY(), 0));
}

/* ************************************************************************ */

void Context::matrixScale(const core::Vector<float>& scale) NOEXCEPT
{
    gl(glScalef(scale.getX(), scale.getY(), 1));
}

/* ************************************************************************ */

void Context::matrixRotate(core::units::Angle angle) NOEXCEPT
{
    gl(glRotatef(core::units::rad2deg(angle), 0.f, 0.f, 1.f));
}

/* ************************************************************************ */

void Context::setColor(const Color& color) NOEXCEPT
{
    gl(glColor4f(color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha()));
}

/* ************************************************************************ */

void Context::setVertexBuffer(Buffer* buffer) NOEXCEPT
{
#ifdef _WIN32
    if (!glBindBuffer)
        glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
#endif

    // Bind buffer
    gl(glBindBuffer(GL_ARRAY_BUFFER, buffer ? buffer->getId() : 0));
}

/* ************************************************************************ */

void Context::setVertexFormat(VertexFormat* format) NOEXCEPT
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

void Context::setTexture(Texture* texture) NOEXCEPT
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

void Context::setProgram(Program* program) NOEXCEPT
{
#ifdef _WIN32
    if (!glUseProgram)
        glUseProgram = (PFNGLUSEPROGRAMPROC) wglGetProcAddress("glUseProgram");
#endif

    gl(glUseProgram(program ? program->getId() : 0));
}

/* ************************************************************************ */

void Context::setProgramParam(Program::UniformId id, bool value) NOEXCEPT
{
#ifdef _WIN32
    if (!glUniform1i)
        glUniform1i = (PFNGLUNIFORM1IPROC) wglGetProcAddress("glUniform1i");
#endif

    gl(glUniform1i(id, int(value)));
}

/* ************************************************************************ */

void Context::setProgramParam(Program::UniformId id, int value) NOEXCEPT
{
#ifdef _WIN32
    if (!glUniform1i)
        glUniform1i = (PFNGLUNIFORM1IPROC) wglGetProcAddress("glUniform1i");
#endif

    gl(glUniform1i(id, value));
}

/* ************************************************************************ */

void Context::setProgramParam(Program::UniformId id, int value1, int value2) NOEXCEPT
{
#ifdef _WIN32
    if (!glUniform2i)
        glUniform2i = (PFNGLUNIFORM2IPROC)wglGetProcAddress("glUniform2i");
#endif

    gl(glUniform2i(id, value1, value2));
}

/* ************************************************************************ */

void Context::setProgramParam(Program::UniformId id, float value) NOEXCEPT
{
#ifdef _WIN32
    if (!glUniform1f)
        glUniform1f = (PFNGLUNIFORM1FPROC)wglGetProcAddress("glUniform1f");
#endif

    gl(glUniform1f(id, value));
}

/* ************************************************************************ */

void Context::draw(PrimitiveType type, unsigned int count, unsigned int offset)
{
    gl(glDrawArrays(convert(type), offset, count));
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

/* ************************************************************************ */


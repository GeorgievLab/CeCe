
/* ************************************************************************ */

#ifndef ENABLE_RENDER
#error ENABLE_RENDER must be 1 to compile render context.
#endif

/* ************************************************************************ */

// Declaration
#include "render/Context.hpp"

// C++
#include <cmath>
#include <cassert>

// OpenGL
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>

// Simulator
#include "render/Circle.hpp"
#include "render/Grid.hpp"

/* ************************************************************************ */

namespace render {

/* ************************************************************************ */

std::vector<std::uint8_t> Context::getData() const noexcept
{
    std::vector<std::uint8_t> data;

    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    glReadBuffer(GL_FRONT);
    glReadPixels(viewport[0], viewport[1], viewport[2], viewport[3],
                 GL_RGBA, GL_UNSIGNED_BYTE, data.data());

    return data;
}

/* ************************************************************************ */

void Context::init() noexcept
{
    assert(!isInit());

    // Clear color.
    glClearColor(1.f, 1.f, 1.f, 1.f);
    //glClearColor(0.f, 0.f, 0.f, 1.f);

    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_is_init = true;
}

/* ************************************************************************ */

void Context::setView(int width, int height) noexcept
{
    if (!m_is_init)
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
    glTranslatef(m_camera.getPosition().x, m_camera.getPosition().y, 0);

    // Apply zoom matrix
    float scale = 1 / m_camera.getZoom();
    glScalef(scale, scale, scale);

    // Rotate camera
    //glRotatef(m_camera.getRotation(), 0, 1, 0);
}

/* ************************************************************************ */

void Context::frameBegin(int width, int height) noexcept
{
    assert(isInit());

    // Clear
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    setView(width, height);
}

/* ************************************************************************ */

void Context::frameEnd() noexcept
{
    assert(isInit());

    glFlush();
}

/* ************************************************************************ */

void Context::drawLine(const Position& pos, const Vector<float>& dir, const Color& color) noexcept
{
    assert(isInit());

    // Setup transformation matrix
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslatef(pos.x, pos.y, 0);

    // Draw color
    glColor4f(color.red, color.green, color.blue, color.alpha);

    glBegin(GL_LINES);

    glVertex2f(0, 0);
    glVertex2f(dir.x, dir.y);

    glEnd();

    glPopMatrix();
}

/* ************************************************************************ */

void Context::drawCircle(const Position& pos, float radius, const Color& color) noexcept
{
    assert(isInit());

    static Circle circle;
    circle.render(pos, radius, color);
}

/* ************************************************************************ */
/*
void Context::drawSphere(const Position& pos, float radius, const Color& color) noexcept
{
    constexpr float DEG2RAD = 3.14159f / 180.f;

    // Setup transformation matrix
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslatef(pos.x, pos.y, pos.z);

    // Sphere color
    glColor4f(color.red, color.green, color.blue, color.alpha);

    constexpr int LATS = 20;
    constexpr int LONGS = 20;

    for (int i = 0; i <= LATS; i++)
    {
        double lat0 = M_PI * (-0.5 + (double) (i - 1) / LATS);
        double z0   = sin(lat0);
        double zr0  = cos(lat0);

        double lat1 = M_PI * (-0.5 + (double) i / LATS);
        double z1 = sin(lat1);
        double zr1 = cos(lat1);

        glBegin(GL_QUAD_STRIP);

        for (int j = 0; j <= LONGS; j++)
        {
            double lng = 2 * M_PI * (double) (j - 1) / LONGS;
            double x = cos(lng);
            double y = sin(lng);

            glNormal3f(x * zr0, y * zr0, z0);
            glVertex3f(radius * x * zr0, radius * y * zr0, radius * z0);
            glNormal3f(x * zr1, y * zr1, z1);
            glVertex3f(radius * x * zr1, radius * y * zr1, radius * z1);
        }

        glEnd();
    }

    glPopMatrix();
}
*/
/* ************************************************************************ */

void Context::drawGrid(const Vector<float>& size, const Vector<unsigned>& count,
                       const Color& color) noexcept
{
    assert(isInit());

    static Grid grid;
    grid.resize(count.x, count.y);
    grid.render(size, color);
}

/* ************************************************************************ */

void Context::drawText(const std::string& text, float x, float y)
{
    assert(isInit());

    glRasterPos2f(x, y);
    //glutBitmapString(GLUT_BITMAP_HELVETICA_12, reinterpret_cast<const unsigned char*>(text.c_str()));
}

/* ************************************************************************ */

}

/* ************************************************************************ */


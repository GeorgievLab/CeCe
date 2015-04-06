
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

std::pair<std::vector<std::uint8_t>, Vector<unsigned>> Context::getData() const noexcept
{
    std::pair<std::vector<std::uint8_t>, Vector<unsigned>> result;

    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    auto x = viewport[0];
    auto y = viewport[1];
    auto width = viewport[2];
    auto height = viewport[3];

    result.first.resize(3 * width * height);
    result.second = Vector<unsigned>(width, height);

    glReadBuffer(GL_FRONT);
    glReadPixels(x, y, width, height, GL_RGB, GL_UNSIGNED_BYTE, result.first.data());

    return result;
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

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);

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
    glTranslatef(m_camera.getPosition().getX(), m_camera.getPosition().getY(), 0);

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
    glTranslatef(pos.getX(), pos.getY(), 0);

    // Draw color
    glColor4f(color.red, color.green, color.blue, color.alpha);

    glBegin(GL_LINES);

    glVertex2f(0, 0);
    glVertex2f(dir.getX(), dir.getY());

    glEnd();

    glPopMatrix();
}

/* ************************************************************************ */

}

/* ************************************************************************ */


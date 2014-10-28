
/* ************************************************************************ */

// Declaration
#include "render/Context.h"

// OpenGL
#include <GL/gl.h>
#include <GL/glu.h>

// Simulator
#include "simulator/World.h"
#include "simulator/Cell.h"
#include "simulator/Yeast.h"

/* ************************************************************************ */

namespace render {

/* ************************************************************************ */

void Context::init() noexcept
{
    // Clear color.
    glClearColor(1.f, 1.f, 1.f, 1.f);

    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

/* ************************************************************************ */

void Context::setTopView(int width, int height, float zoom) noexcept
{
    const GLdouble wh = width / 2.0;
    const GLdouble hh = height / 2.0;

    // Setup viewport (whole window)
    glViewport(0, 0, width, height);

    // Setup projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-wh, wh, -hh, hh, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Apply zoom matrix
    glScalef(zoom, zoom, zoom);
}

/* ************************************************************************ */

void Context::setIsometricView(int width, int height, float zoom) noexcept
{
    // Setup viewport (whole window)
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    const GLdouble dist = sqrt(1 / 3.0);

    gluLookAt(dist, dist, dist,
              0.0,  0.0,  0.0,
              0.0,  0.0,  1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // TODO zoom
    glScalef(0.1 * zoom, 0.1 * zoom, 0.1 * zoom);
}

/* ************************************************************************ */

void Context::frameBegin() noexcept
{
    // Clear
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/* ************************************************************************ */

void Context::frameEnd() noexcept
{
    glFlush();
}

/* ************************************************************************ */

void Context::drawWorld(const simulator::World& world) noexcept
{
    // Axis
    {
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glScalef(5, 5, 5);

        glBegin(GL_LINES);

        glColor3d(1.0, 0.0, 0.0);
        glVertex3d(0.0, 0.0, 0.0);
        glVertex3d(1.0, 0.0, 0.0);

        glColor3d(0.0, 1.0, 0.0);
        glVertex3d(0.0, 0.0, 0.0);
        glVertex3d(0.0, 1.0, 0.0);

        glColor3d(0.0, 0.0, 1.0);
        glVertex3d(0.0, 0.0, 0.0);
        glVertex3d(0.0, 0.0, 1.0);

        glEnd();

        glPopMatrix();
    }

    // Plane
    {
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();

        auto width = world.getWidth().value();
        auto height = world.getHeight().value();
        auto wh = width / 2.0;
        auto hh = height / 2.0;

        glBegin(GL_QUADS);
        glColor3d(0.9, 0.9, 0.9);
        glVertex3d(-wh, -hh, 0);
        glVertex3d(-wh,  hh, 0);
        glVertex3d( wh,  hh, 0);
        glVertex3d( wh, -hh, 0);
        glEnd();

        glPopMatrix();
    }

    // Draw cells
    for (const auto& cell : world.getCells())
    {
        drawCell(*cell);
    }
}

/* ************************************************************************ */

void Context::drawCell(const simulator::Cell& cell) noexcept
{
    if (auto yeast = dynamic_cast<const simulator::Yeast*>(&cell))
    {
        drawYeast(*yeast);
    }
    else
    {
        // Unsupported type
    }
}

/* ************************************************************************ */

void Context::drawYeast(const simulator::Yeast& yeast) noexcept
{
    constexpr float DEG2RAD = 3.14159f / 180.f;

    const auto pos = yeast.getPosition();
    const auto radius = yeast.getRadius();

    // Setup transformation matrix
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslatef(pos.x.value(), pos.y.value(), 0);

    glColor4f(0, 0, 0, 0.5f);

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0, 0);

    for (int i = 0; i <= 360; i++)
    {
        float degInRad = i * DEG2RAD;
        glVertex2f(cos(degInRad) * radius.value(), sin(degInRad) * radius.value());
    }

    glEnd();

    glColor3f(0.2f, 0.2f, 0.2f);

    glBegin(GL_LINES);

    for (int i = 0; i <= 360; i++)
    {
        float degInRad = i * DEG2RAD;
        glVertex2f(cos(degInRad) * radius.value(), sin(degInRad) * radius.value());
    }

    glEnd();

    glPopMatrix();
}

/* ************************************************************************ */

}

/* ************************************************************************ */

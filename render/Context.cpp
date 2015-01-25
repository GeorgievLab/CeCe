
/* ************************************************************************ */

// Declaration
#include "render/Context.h"

// OpenGL
#include <GL/gl.h>
#include <GL/glu.h>

// Simulator
#include "simulator/World.h"
#include "simulator/Cell.h"

/* ************************************************************************ */

namespace render {

/* ************************************************************************ */

void Context::init() noexcept
{
    GLfloat sun_direction[] = { 0.0, 0.0, -1.0, 1.0 };
    GLfloat sun_intensity[] = { 0.7, 0.7, 0.7, 1.0 };
    GLfloat ambient_intensity[] = { 0.3, 0.3, 0.3, 1.0 };

    // Clear color.
    glClearColor(1.f, 1.f, 1.f, 1.f);

    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_LIGHTING);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_intensity);

    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, sun_direction);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, sun_intensity);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
}

/* ************************************************************************ */

void Context::setView(int width, int height) noexcept
{
     // Setup viewport (whole window)
    glViewport(0, 0, width, height);

    // Setup projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    const GLdouble wh = width / 2.0;
    const GLdouble hh = height / 2.0;

    glOrtho(-wh, wh, -hh, hh, -1000, 1000);

    if (m_camera.getMode() == Camera::Mode::Top)
    {
        glRotatef(90, 1, 0, 0);
    }
    else if (m_camera.getMode() == Camera::Mode::Isometric)
    {
        const GLdouble dist = 100 * sqrt(1 / 3.0);

        gluLookAt(dist, dist, dist,
                  0.0,  0.0,  0.0,
                  0.0,  1.0,  0.0);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Apply zoom matrix
    float scale = 1 / m_camera.getZoom();
    glScalef(scale, scale, scale);
}

/* ************************************************************************ */

void Context::frameBegin(int width, int height) noexcept
{
    // Clear
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    setView(width, height);
}

/* ************************************************************************ */

void Context::frameEnd() noexcept
{
    glFlush();
}

/* ************************************************************************ */

void Context::drawWorld(const simulator::World& world) noexcept
{
    // Gravity
    {
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();

        auto gravity = world.getGravity();

        glBegin(GL_LINES);

        // X
        glColor3d(0.0, 0.0, 0.0);
        glVertex3d(0.0, 0.0, 0.0);
        glVertex3d(gravity[0], gravity[1], gravity[2]);

        glEnd();

        glPopMatrix();
    }

    // Axis
    {
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glScalef(5, 5, 5);

        glBegin(GL_LINES);

        // X
        glColor3d(1.0, 0.0, 0.0);
        glVertex3d(0.0, 0.0, 0.0);
        glVertex3d(1.0, 0.0, 0.0);

        // Y
        glColor3d(0.0, 1.0, 0.0);
        glVertex3d(0.0, 0.0, 0.0);
        glVertex3d(0.0, 1.0, 0.0);

        // Z
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
        glVertex3d(-wh, 0, -hh);
        glVertex3d(-wh, 0,  hh);
        glVertex3d( wh, 0,  hh);
        glVertex3d( wh, 0, -hh);
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
    switch (cell.getShapeType())
    {
    case simulator::Cell::Shape::Sphere:
        drawCellSphere(cell);
        break;

    default:
        // Unsupported type
        break;
    }
}

/* ************************************************************************ */

void Context::drawCellSphere(const simulator::Cell& cell) noexcept
{
    constexpr float DEG2RAD = 3.14159f / 180.f;

    const auto pos = cell.getPosition();
    const auto radius = simulator::Cell::calcSphereRadius(cell.getVolume()).value();

    // Setup transformation matrix
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslatef(pos.x.value(), pos.y.value(), pos.z.value());

    auto gfp = cell.getGfp();
    auto rfp = cell.getRfp();
    auto yfp = cell.getYfp();

    float red = rfp / 1000.f + yfp / 1000.f;
    float green = gfp / 1000.f + yfp / 1000.f;
    float blue = 0;

    glColor4f(red, green, blue, 0.5f);
/*
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0, 0);

    for (int i = 0; i <= 360; i++)
    {
        float degInRad = i * DEG2RAD;
        glVertex2f(cos(degInRad) * radius, sin(degInRad) * radius);
    }
*/
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

            //glNormal3f(x * zr0, y * zr0, z0);
            glVertex3f(radius * x * zr0, radius * y * zr0, radius * z0);
            //glNormal3f(x * zr1, y * zr1, z1);
            glVertex3f(radius * x * zr1, radius * y * zr1, radius * z1);
        }

        glEnd();
    }
/*
    glColor3f(0.2f, 0.2f, 0.2f);

    glBegin(GL_LINES);

    for (int i = 0; i <= 360; i++)
    {
        float degInRad = i * DEG2RAD;
        glVertex2f(cos(degInRad) * radius, sin(degInRad) * radius);
    }

    glEnd();
*/
    glPopMatrix();
}

/* ************************************************************************ */

}

/* ************************************************************************ */

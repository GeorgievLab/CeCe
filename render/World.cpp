
/* ************************************************************************ */

// Declaration
#include "render/World.h"

// OpenGL
#include <GL/gl.h>

/* ************************************************************************ */

namespace render {

/* ************************************************************************ */

void World::render(Context& context) noexcept
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
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
}

/* ************************************************************************ */

}

/* ************************************************************************ */


/* ************************************************************************ */

// Declaration
#include "render/Yeast.h"

// C++
#include <cmath>

// OpenGL
#include <GL/gl.h>

/* ************************************************************************ */

namespace render {

/* ************************************************************************ */

void Yeast::render(Context& context, MicroMeters x, MicroMeters y, MicroMeters radius) noexcept
{
    constexpr float DEG2RAD = 3.14159f / 180.f;

    // Setup transformation matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(x.value(), y.value(), 0);

    glColor3f(0, 0, 0);

    glBegin(GL_LINE_LOOP);

    for (int i = 0; i < 360; i++)
    {
        float degInRad = i * DEG2RAD;
        glVertex2f(cos(degInRad) * radius.value(), sin(degInRad) * radius.value());
    }

    glEnd();
}

/* ************************************************************************ */

}

/* ************************************************************************ */

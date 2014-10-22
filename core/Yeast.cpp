
/* ************************************************************************ */

// Declaration
#include "Yeast.h"

// wxWidgets
#include <wx/glcanvas.h>

/* ************************************************************************ */

Yeast::Yeast(World* world, MicroMeters x, MicroMeters y, MicroMeters radius)
    : Yeast(world, x, y, CalcVolume(radius))
{
    // Nothing to do
}

/* ************************************************************************ */

Yeast::Yeast(World* world, MicroMeters x, MicroMeters y, MicroMeters3 volume)
    : Cell(world, x, y)
    , m_volume(volume)
{

}

/* ************************************************************************ */

void Yeast::Render() const noexcept
{
    constexpr float DEG2RAD = 3.14159f / 180.f;

    auto body = GetPhysicsBody();
    auto pos = body->GetPosition();
    float radius =  GetRadius().value();

    // Setup transformation matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(pos.x, pos.y, 0);

    glColor3f(0, 0, 0);

    //dc.DrawCircle(pos.x, pos.y, GetRadius().value());
    glBegin(GL_LINE_LOOP);

    for (int i = 0; i < 360; i++)
    {
        float degInRad = i * DEG2RAD;
        glVertex2f(cos(degInRad) * radius, sin(degInRad) * radius);
    }

    glEnd();
}

/* ************************************************************************ */


/* ************************************************************************ */

// Declaration
#include "Barrier.h"

// C++
#include <algorithm>

// Core
#include "World.h"

// wxWidgets
#include <wx/glcanvas.h>

/* ************************************************************************ */

Barrier::Barrier(World* world, MicroMeters x1, MicroMeters y1, MicroMeters x2, MicroMeters y2)
    : m_world(world)
    , m_size{MicroMeters(std::abs(x2.value() - x1.value())), MicroMeters(std::abs(y2.value() - y1.value()))}
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.position.Set(x1.value(), y1.value());

    m_physicsBody = world->GetPhysicsWorld().CreateBody(&bodyDef);

    b2PolygonShape shape;
    shape.SetAsBox(m_size.x.value(), m_size.y.value());

    b2FixtureDef def;
    def.shape = &shape;
    def.density = 1;
    m_physicsBody->CreateFixture(&def);
}

/* ************************************************************************ */

Barrier::Barrier(Barrier&& rhs) noexcept
    : m_world(rhs.m_world)
    , m_physicsBody(rhs.m_physicsBody)
    , m_size(rhs.m_size)
{
    rhs.m_world = nullptr;
    rhs.m_physicsBody = nullptr;
}

/* ************************************************************************ */

Barrier::~Barrier()
{
    if (m_world)
        m_world->GetPhysicsWorld().DestroyBody(m_physicsBody);
}

/* ************************************************************************ */

//void Barrier::Render(wxDC& dc) const noexcept
void Barrier::Render() const noexcept
{
    auto pos = m_physicsBody->GetPosition();
    float width = m_size.x.value();
    float height = m_size.y.value();

    // Setup transformation matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(pos.x, pos.y, 0);

    glColor3f(0.5f, 0.5f, 0.5f);

    glBegin(GL_QUADS);
        glVertex3f(0, 0, 0);
        glVertex3f(width, 0, 0);
        glVertex3f(width, height, 0);
        glVertex3f(0, height, 0);
    glEnd();
}

/* ************************************************************************ */

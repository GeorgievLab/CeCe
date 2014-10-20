
/* ************************************************************************ */

// Declaration
#include "Cell.h"

// Core
#include "World.h"

/* ************************************************************************ */

Cell::Id Cell::s_id = 0;

/* ************************************************************************ */

Cell::Cell(World* world, MicroMeters x, MicroMeters y)
    : m_id(s_id++)
    , m_world(world)
{
    b2BodyDef def;
    def.type = b2_dynamicBody;
    def.position.Set(x.value(), y.value());

    m_physicsBody = world->GetPhysicsWorld().CreateBody(&def);
}

/* ************************************************************************ */

Cell::~Cell()
{
    m_world->GetPhysicsWorld().DestroyBody(m_physicsBody);
}

/* ************************************************************************ */

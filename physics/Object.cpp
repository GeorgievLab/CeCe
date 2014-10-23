
/* ************************************************************************ */

// Declaration
#include "physics/Object.h"

// C++
#include <cassert>

// Bullet
#include <btBulletDynamicsCommon.h>

// Physics
#include "physics/World.h"

/* ************************************************************************ */

namespace physics {

/* ************************************************************************ */

Object::Object(World* world)
    : m_world(world)
{
    assert(m_world);
}

/* ************************************************************************ */

Object::~Object()
{
    if (m_rigidBody)
        unregisterRigidBody();
}

/* ************************************************************************ */

void Object::registerRigidBody() noexcept
{
    assert(m_rigidBody);
    assert(m_world);
    assert(m_world->getDynamicsWorld());
    m_world->getDynamicsWorld()->addRigidBody(m_rigidBody.get());
}

/* ************************************************************************ */

void Object::unregisterRigidBody() noexcept
{
    assert(m_rigidBody);
    assert(m_world);
    assert(m_world->getDynamicsWorld());
    m_world->getDynamicsWorld()->removeRigidBody(m_rigidBody.get());
}

/* ************************************************************************ */

}

/* ************************************************************************ */

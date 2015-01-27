
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

    // Create initial motion state
    m_motionState.reset(new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0))));
}

/* ************************************************************************ */

Object::~Object()
{
    if (m_rigidBody)
        unregisterRigidBody();
}

/* ************************************************************************ */

Position Object::getPosition() const noexcept
{
    assert(m_rigidBody);
    auto pos = m_rigidBody->getCenterOfMassPosition();

    return {MicroMeters(pos.x()), MicroMeters(pos.y()), MicroMeters(pos.z())};
}

/* ************************************************************************ */

void Object::setPosition(Position pos) noexcept
{
    btTransform trans;
    assert(m_rigidBody);
    m_rigidBody->getMotionState()->getWorldTransform(trans);

    // Update position
    trans.setOrigin(btVector3(pos.x.value(), pos.y.value(), pos.z.value()));

    m_rigidBody->setCenterOfMassTransform(trans);
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

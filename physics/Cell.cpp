
/* ************************************************************************ */

// Declaration
#include "physics/Cell.h"

// Bullet
#include <btBulletDynamicsCommon.h>

// Core
#include "physics/World.h"

/* ************************************************************************ */

namespace physics {

/* ************************************************************************ */

Cell::Cell(World* world, MicroMeters x, MicroMeters y)
    : Object(world)
{
    // Create initial motion state
    m_motionState.reset(new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(x.value(), 1, y.value()))));
}

/* ************************************************************************ */

Vect<MicroMeters> Cell::getPosition() const noexcept
{
    btTransform trans;
    m_rigidBody->getMotionState()->getWorldTransform(trans);

    auto pos = trans.getOrigin();

    return {MicroMeters(pos.x()), MicroMeters(pos.z())};
}

/* ************************************************************************ */

void Cell::reset() noexcept
{
    // TODO: implement
    //btTransform(btQuaternion(0, 0, 0, 1), btVector3(x.value(), 1, y.value()))
}

/* ************************************************************************ */

void Cell::createSphereBody(MicroMeters radius) noexcept
{
    float mass = 1;
    btVector3 fallInertia(0, 0, 0);

    m_shape.reset(new btSphereShape(radius.value()));
    m_shape->calculateLocalInertia(mass, fallInertia);

    m_rigidBody.reset(new btRigidBody(btRigidBody::btRigidBodyConstructionInfo(mass, m_motionState.get(), m_shape.get(), fallInertia)));

    // Register body
    registerRigidBody();
}

/* ************************************************************************ */

}

/* ************************************************************************ */

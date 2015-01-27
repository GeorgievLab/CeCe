
/* ************************************************************************ */

// Declaration
#include "physics/Barrier.hpp"

// Bullet
#include <btBulletDynamicsCommon.h>

// Core
#include "physics/World.h"

/* ************************************************************************ */

namespace physics {

/* ************************************************************************ */

Barrier::Barrier(World* world)
    : Object(world)
{
    updateBody();
}

/* ************************************************************************ */

void Barrier::updateBody() noexcept
{
    if (m_rigidBody)
        unregisterRigidBody();

    m_shape.reset(new btBoxShape(btVector3(m_width.value(), m_height.value(), m_depth.value())));
    m_motionState.reset(new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0))));
    m_rigidBody.reset(new btRigidBody(btRigidBody::btRigidBodyConstructionInfo(0, m_motionState.get(), m_shape.get(), btVector3(0, 0, 0))));

    // Register body
    registerRigidBody();
}

/* ************************************************************************ */

}

/* ************************************************************************ */

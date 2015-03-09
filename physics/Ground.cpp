
/* ************************************************************************ */

// Declaration
#include "physics/Ground.hpp"

// C++
#include <cassert>

// Bullet
#include <btBulletDynamicsCommon.h>

/* ************************************************************************ */

namespace physics {

/* ************************************************************************ */

Ground::Ground(World* world)
    : Object(world)
{
    m_shape.reset(new btStaticPlaneShape(btVector3(0, 1, 0), 0));
    m_motionState.reset(new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0))));
    m_rigidBody.reset(new btRigidBody(btRigidBody::btRigidBodyConstructionInfo(0, m_motionState.get(), m_shape.get(), btVector3(0, 0, 0))));

    // Register body
    registerRigidBody();
}

/* ************************************************************************ */

}

/* ************************************************************************ */

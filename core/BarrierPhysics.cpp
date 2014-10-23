
/* ************************************************************************ */

// Declaration
#include "BarrierPhysics.h"

// C++
#include <algorithm>

// Core
#include "World.h"

/* ************************************************************************ */

BarrierPhysics::BarrierPhysics(World* world, MicroMeters x1, MicroMeters y1, MicroMeters x2, MicroMeters y2)
    : m_world(world)
{
    float width = std::abs(x2.value() - x1.value());
    float height = std::abs(y2.value() - y1.value());

    // Create shape
    m_shape.reset(new btBoxShape(btVector3(width, 2, height)));

    // Create motion state
    m_motionState.reset(new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0))));

    // Create body
    m_rigidBody.reset(new btRigidBody(btRigidBody::btRigidBodyConstructionInfo(0, m_motionState.get(), m_shape.get(), btVector3(0, 0, 0))));

    // Add body
    m_world->GetDynamicsWorld()->addRigidBody(m_rigidBody.get());
}

/* ************************************************************************ */

BarrierPhysics::~BarrierPhysics()
{
    // Remove body
    m_world->GetDynamicsWorld()->removeRigidBody(m_rigidBody.get());
}

/* ************************************************************************ */

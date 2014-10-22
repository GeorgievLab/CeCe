
/* ************************************************************************ */

// Declaration
#include "WorldPhysics.h"

// C++
#include <algorithm>

// Library

/* ************************************************************************ */

WorldPhysics::WorldPhysics() noexcept
    : m_broadphase(new btDbvtBroadphase())
    , m_collisionConfiguration(new btDefaultCollisionConfiguration())
    , m_dispatcher(new btCollisionDispatcher(m_collisionConfiguration.get()))
    , m_solver(new btSequentialImpulseConstraintSolver)
    , m_dynamicsWorld(new btDiscreteDynamicsWorld(m_dispatcher.get(), m_broadphase.get(), m_solver.get(), m_collisionConfiguration.get()))
    , m_groundShape(new btStaticPlaneShape(btVector3(0, 1, 0), 0))
    , m_groundMotionState(new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0))))
    , m_groundRigidBody(new btRigidBody(btRigidBody::btRigidBodyConstructionInfo(0, m_groundMotionState.get(), m_groundShape.get(), btVector3(0, 0, 0))))
{
    m_dynamicsWorld->setGravity(btVector3(0, -10, 0));

    // Get body
    m_dynamicsWorld->addRigidBody(m_groundRigidBody.get());
}

/* ************************************************************************ */

WorldPhysics::~WorldPhysics() noexcept
{
    m_dynamicsWorld->removeRigidBody(m_groundRigidBody.get());
}

/* ************************************************************************ */

void WorldPhysics::Update() noexcept
{
    m_dynamicsWorld->stepSimulation(1 / 60.f, 5);
}

/* ************************************************************************ */

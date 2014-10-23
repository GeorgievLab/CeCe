
/* ************************************************************************ */

// Declaration
#include "physics/World.h"

// C++
#include <algorithm>

// Bullet
#include <btBulletDynamicsCommon.h>

// Library
#include "physics/Ground.h"

/* ************************************************************************ */

namespace physics {

/* ************************************************************************ */

World::World() noexcept
    : m_broadphase(new btDbvtBroadphase())
    , m_collisionConfiguration(new btDefaultCollisionConfiguration())
    , m_dispatcher(new btCollisionDispatcher(m_collisionConfiguration.get()))
    , m_solver(new btSequentialImpulseConstraintSolver)
    , m_dynamicsWorld(new btDiscreteDynamicsWorld(m_dispatcher.get(), m_broadphase.get(), m_solver.get(), m_collisionConfiguration.get()))
{
    m_dynamicsWorld->setGravity(btVector3(0, -10, 0));

    m_ground.reset(new Ground(this));
}

/* ************************************************************************ */

World::~World() noexcept
{
    // Nothing to do
}

/* ************************************************************************ */

void World::Update() noexcept
{
    m_dynamicsWorld->stepSimulation(1 / 60.f, 5);
}

/* ************************************************************************ */

}

/* ************************************************************************ */

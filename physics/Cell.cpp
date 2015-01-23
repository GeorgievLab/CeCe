
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

Cell::Cell(World* world, Shape shapeType)
    : Object(world)
    , m_shapeType(shapeType)
{
    setVolume(10000_um3);
}

/* ************************************************************************ */

void Cell::setVolume(MicroMeters3 volume) noexcept
{
    m_volume = volume;

    // Update shape
    switch (m_shapeType)
    {
    case Shape::Sphere:
        updateSphereBody();
        break;
    }
}

/* ************************************************************************ */

void Cell::updateSphereBody() noexcept
{
    float mass = 1;
    btVector3 fallInertia(0, 0, 0);

    btSphereShape* shape = dynamic_cast<btSphereShape*>(m_shape.get());

    if (shape)
    {
        // Update radius
        shape->setUnscaledRadius(calcSphereRadius(m_volume).value());
        shape->calculateLocalInertia(mass, fallInertia);
    }
    else
    {
        m_shape.reset(new btSphereShape(calcSphereRadius(m_volume).value()));
        m_shape->calculateLocalInertia(mass, fallInertia);

        m_rigidBody.reset(new btRigidBody(btRigidBody::btRigidBodyConstructionInfo(mass, m_motionState.get(), m_shape.get(), fallInertia)));

        // Register body
        registerRigidBody();
    }
}

/* ************************************************************************ */

}

/* ************************************************************************ */

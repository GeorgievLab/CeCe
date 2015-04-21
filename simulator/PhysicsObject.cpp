
/* ************************************************************************ */

// Declaration
#include "simulator/PhysicsObject.hpp"

// Simulator
#include "simulator/Simulation.hpp"

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

PhysicsObject::PhysicsObject(Simulation& simulation) noexcept
    : DynamicObject(simulation)
{
    setFlag(OBJECT_PHYSICS);

#ifdef ENABLE_PHYSICS
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;

    // Create body
    m_body = simulation.getWorld().CreateBody(&bodyDef);
#endif
}

/* ************************************************************************ */

#ifdef ENABLE_PHYSICS
PhysicsObject::PhysicsObject(Simulation& simulation, b2Body* body) noexcept
    : DynamicObject(simulation)
    , m_body(body)
{
    setFlag(OBJECT_PHYSICS);
}
#endif

/* ************************************************************************ */

PhysicsObject::~PhysicsObject()
{
#ifdef ENABLE_PHYSICS
    getSimulation().getWorld().DestroyBody(m_body);
#endif
}

/* ************************************************************************ */

const Position& PhysicsObject::getPosition() const noexcept
{
#ifdef ENABLE_PHYSICS
    auto pos = m_body->GetPosition();
    const_cast<PhysicsObject*>(this)->DynamicObject::setPosition({pos.x, pos.y});
#endif

    return DynamicObject::getPosition();
}

/* ************************************************************************ */

const Velocity& PhysicsObject::getVelocity() const noexcept
{
#ifdef ENABLE_PHYSICS
    auto vel = m_body->GetLinearVelocity();
    const_cast<PhysicsObject*>(this)->DynamicObject::setVelocity({vel.x, vel.y});
#endif

    return DynamicObject::getVelocity();
}

/* ************************************************************************ */

void PhysicsObject::setPosition(Position pos) noexcept
{
#ifdef ENABLE_PHYSICS
    m_body->SetTransform({pos.getX(), pos.getY()}, 0);
#endif

    DynamicObject::setPosition(pos);
}

/* ************************************************************************ */

void PhysicsObject::setVelocity(Velocity vel) noexcept
{
#ifdef ENABLE_PHYSICS
    m_body->SetLinearVelocity({vel.getX(), vel.getY()});
#endif

    DynamicObject::setVelocity(vel);
}

/* ************************************************************************ */

void PhysicsObject::addAcceleration(const Acceleration& acc) noexcept
{
#ifdef ENABLE_PHYSICS
    m_body->ApplyForceToCenter({acc.getX(), acc.getY()}, true);
#endif

    DynamicObject::addAcceleration(acc);
}

/* ************************************************************************ */

void PhysicsObject::update(units::Duration dt)
{
    //DynamicObject::update(dt);
}

/* ************************************************************************ */

}

/* ************************************************************************ */



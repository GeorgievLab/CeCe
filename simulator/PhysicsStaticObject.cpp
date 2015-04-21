
/* ************************************************************************ */

// Declaration
#include "simulator/PhysicsStaticObject.hpp"

// Simulator
#include "simulator/Simulation.hpp"

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

PhysicsStaticObject::PhysicsStaticObject(Simulation& simulation) noexcept
    : StaticObject(simulation)
{
    setFlag(OBJECT_PHYSICS);

#ifdef ENABLE_PHYSICS
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;

    // Create body
    m_body = simulation.getWorld().CreateBody(&bodyDef);
#endif
}

/* ************************************************************************ */

#ifdef ENABLE_PHYSICS
PhysicsStaticObject::PhysicsStaticObject(Simulation& simulation, b2Body* body) noexcept
    : StaticObject(simulation)
    , m_body(body)
{
    setFlag(OBJECT_PHYSICS);
}
#endif

/* ************************************************************************ */

PhysicsStaticObject::~PhysicsStaticObject()
{
#ifdef ENABLE_PHYSICS
    getSimulation().getWorld().DestroyBody(m_body);
#endif
}

/* ************************************************************************ */

const Position& PhysicsStaticObject::getPosition() const noexcept
{
#ifdef ENABLE_PHYSICS
    auto pos = m_body->GetPosition();
    const_cast<PhysicsStaticObject*>(this)->StaticObject::setPosition({pos.x, pos.y});
#endif

    return StaticObject::getPosition();
}

/* ************************************************************************ */

void PhysicsStaticObject::setPosition(Position pos) noexcept
{
#ifdef ENABLE_PHYSICS
    m_body->SetTransform({pos.getX(), pos.getY()}, 0);
#endif

    StaticObject::setPosition(pos);
}

/* ************************************************************************ */

void PhysicsStaticObject::update(units::Duration dt)
{
    //StaticObject::update(dt);
}

/* ************************************************************************ */

}

/* ************************************************************************ */



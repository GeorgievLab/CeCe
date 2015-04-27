
/* ************************************************************************ */

// Declaration
#include "simulator/Object.hpp"

// C++
#include <cassert>

// Simulator
#include "simulator/Simulation.hpp"

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

#if ENABLE_PHYSICS
/**
 * @brief Convert object type into Box2 body type.
 *
 * @param type
 *
 * @return
 */
b2BodyType convert(Object::Type type) noexcept
{
    switch (type)
    {
    default:                    return b2_staticBody;
    case Object::Type::Static:  return b2_staticBody;
    case Object::Type::Dynamic: return b2_dynamicBody;
    }
}
#endif

/* ************************************************************************ */

#if ENABLE_PHYSICS
/**
 * @brief Convert Box2 body type into object type.
 *
 * @param type
 *
 * @return
 */
Object::Type convert(b2BodyType type) noexcept
{
    switch (type)
    {
    default:                return Object::Type::Static;
    case b2_staticBody:     return Object::Type::Static;
    case b2_dynamicBody:    return Object::Type::Dynamic;
    }
}
#endif

/* ************************************************************************ */

}

/* ************************************************************************ */

Object::IdType s_id = 0;

/* ************************************************************************ */

Object::Object(Simulation& simulation, Type type) noexcept
    : m_simulation(simulation)
    , m_id(++s_id)
#if !ENABLE_PHYSICS
    , m_type(type)
#endif
{
#if ENABLE_PHYSICS
    b2BodyDef bodyDef;
    bodyDef.type = convert(type);

    // Create body
    m_body = getSimulation().getWorld().CreateBody(&bodyDef);
#endif
}

/* ************************************************************************ */

Object::~Object()
{
#if ENABLE_PHYSICS
    assert(m_body);
    getSimulation().getWorld().DestroyBody(m_body);
#endif
}

/* ************************************************************************ */

Object::Type Object::getType() const noexcept
{
#if ENABLE_PHYSICS
    assert(m_body);
    return convert(m_body->GetType());
#else
    return m_type;
#endif
}

/* ************************************************************************ */

PositionVector Object::getPosition() const noexcept
{
#if ENABLE_PHYSICS
    assert(m_body);
    auto pos = m_body->GetPosition();
    return {pos.x, pos.y};
#else
    return m_position;
#endif
}

/* ************************************************************************ */

VelocityVector Object::getVelocity() const noexcept
{
#if ENABLE_PHYSICS
    assert(m_body);
    auto vel = m_body->GetLinearVelocity();
    return {vel.x, vel.y};
#else
    return m_velocity;
#endif
}

/* ************************************************************************ */

void Object::setType(Type type) noexcept
{
#if ENABLE_PHYSICS
    assert(m_body);
    m_body->SetType(convert(type));
#else
    m_type = type;
#endif
}

/* ************************************************************************ */

void Object::setPosition(PositionVector pos) noexcept
{
#if ENABLE_PHYSICS
    assert(m_body);
    m_body->SetTransform({pos.getX(), pos.getY()}, 0);
#else
    m_position = std::move(pos);
#endif
}

/* ************************************************************************ */

void Object::setVelocity(VelocityVector vel) noexcept
{
#if ENABLE_PHYSICS
    assert(m_body);
    m_body->SetLinearVelocity({vel.getX(), vel.getY()});
#else
    m_velocity = std::move(vel);
#endif
}

/* ************************************************************************ */

void Object::applyForce(const ForceVector& force) noexcept
{
#if ENABLE_PHYSICS
    assert(m_body);
    m_body->ApplyForceToCenter({force.getX(), force.getY()}, true);
#else
    // NOTE: This is a little bit weird
    m_velocity = force;
#endif
}

/* ************************************************************************ */

void Object::applyForce(const ForceVector& force, const PositionVector& pos) noexcept
{
#if ENABLE_PHYSICS
    assert(m_body);
    m_body->ApplyForce({force.getX(), force.getY()}, {pos.getX(), pos.getY()}, true);
#else
    m_velocity = force;
#endif
}

/* ************************************************************************ */

void Object::update(units::Duration dt)
{
#if !ENABLE_PHYSICS
    // Calculate new object position
    setPosition(getPosition() + getVelocity() * dt);
#endif
}

/* ************************************************************************ */

}

/* ************************************************************************ */

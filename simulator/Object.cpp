/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* ************************************************************************ */

// Declaration
#include "simulator/Object.hpp"

// C++
#include <cassert>
#include <string>
#include <sstream>

// Simulator
#include "core/Units.hpp"
#include "core/Log.hpp"
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
    case Object::Type::Pinned:  return b2_dynamicBody;
    }
}
#endif

/* ************************************************************************ */

/**
 * @brief Split string into multiple strings separated by separator.
 *
 * @param value
 * @param separator
 *
 * @return
 */
DynamicArray<String> split(String value, char separator) noexcept
{
    DynamicArray<String> elems;
    std::istringstream ss(std::move(value));
    String item;

    while (std::getline(ss, item, separator))
    {
        elems.push_back(item);
    }

    return elems;
}

/* ************************************************************************ */

}

/* ************************************************************************ */

Object::IdType s_id = 0;

/* ************************************************************************ */

Object::Object(Simulation& simulation, Type type) noexcept
    : m_simulation(simulation)
    , m_id(++s_id)
    , m_type(type)
{
#if ENABLE_PHYSICS
    auto& world = getSimulation().getWorld();

    b2BodyDef bodyDef;
    bodyDef.type = convert(type);
    bodyDef.userData = this;

    // Create body
    m_body = world.CreateBody(&bodyDef);

    // Pin the body
    if (type == Type::Pinned)
    {
        b2BodyDef pivotDef;
        pivotDef.position = m_body->GetWorldCenter();
        m_pinBody = world.CreateBody(&pivotDef);

        b2RevoluteJointDef jointDef;
        jointDef.Initialize(m_pinBody, m_body, m_pinBody->GetPosition());
        jointDef.motorSpeed =       1;
        jointDef.maxMotorTorque =   10000000;
        jointDef.enableMotor =      true;
        jointDef.enableLimit =      false;

        m_pinJoint = world.CreateJoint(&jointDef);
    }
#endif
}

/* ************************************************************************ */

Object::~Object()
{
#if ENABLE_PHYSICS
    auto& world = getSimulation().getWorld();

    assert(m_body);
    world.DestroyBody(m_body);
#endif
}

/* ************************************************************************ */

PositionVector Object::getPosition() const noexcept
{
#if ENABLE_PHYSICS
    assert(m_body);
    auto pos = m_body->GetPosition();
    return {units::Length(pos.x), units::Length(pos.y)};
#else
    return m_position;
#endif
}

/* ************************************************************************ */

units::Angle Object::getRotation() const noexcept
{
#if ENABLE_PHYSICS
    assert(m_body);
    return m_body->GetAngle();
#else
    return m_rotation;
#endif
}

/* ************************************************************************ */

VelocityVector Object::getVelocity() const noexcept
{
#if ENABLE_PHYSICS
    assert(m_body);
    auto vel = m_body->GetLinearVelocity();
    return {units::Velocity(vel.x), units::Velocity(vel.y)};
#else
    return m_velocity;
#endif
}

/* ************************************************************************ */

void Object::setType(Type type) noexcept
{
    m_type = type;
#if ENABLE_PHYSICS
    assert(m_body);
    m_body->SetType(convert(type));
#endif
}

/* ************************************************************************ */

void Object::setPosition(PositionVector pos) noexcept
{
#if ENABLE_PHYSICS
    assert(m_body);
    m_body->SetTransform({pos.getX().value(), pos.getY().value()}, m_body->GetAngle());
#else
    m_position = std::move(pos);
#endif
}

/* ************************************************************************ */

void Object::setRotation(units::Angle angle) noexcept
{
#if ENABLE_PHYSICS
    assert(m_body);
    m_body->SetTransform(m_body->GetPosition(), angle);
#else
    m_rotation = angle;
#endif
}

/* ************************************************************************ */

void Object::setVelocity(VelocityVector vel) noexcept
{
#if ENABLE_PHYSICS
    assert(m_body);
    m_body->SetLinearVelocity({vel.getX().value(), vel.getY().value()});
#else
    m_velocity = std::move(vel);
#endif
}

/* ************************************************************************ */

void Object::applyForce(const ForceVector& force) noexcept
{
#if ENABLE_PHYSICS
    assert(m_body);
    m_body->ApplyForceToCenter({force.getX().value(), force.getY().value()}, true);
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
    m_body->ApplyForce({force.getX().value(), force.getY().value()}, {pos.getX().value(), pos.getY().value()}, true);
    m_force += force;
#else
    m_velocity = force;
#endif
}

/* ************************************************************************ */

void Object::destroy()
{
    m_simulation.deleteObject(this);
}

/* ************************************************************************ */

void Object::update(units::Duration dt)
{
#if !ENABLE_PHYSICS
    // Calculate new object position
    setPosition(getPosition() + getVelocity() * dt);
#endif

    // Call object programs
    for (auto& program : getPrograms())
        program(*this, getSimulation(), dt);
}

/* ************************************************************************ */

void Object::configure(const Configuration& config, Simulation& simulation)
{
    // Set object position
    setPosition(config.get("position", getPosition()));

    // Set object density
    setDensity(config.get("density", getDensity()));

    // Set object programs
    if (config.has("programs"))
    {
        for (const auto& name : split(config.get("programs"), ' '))
        {
            auto program = simulation.getProgram(name);

            if (program)
                addProgram(std::move(program));
            else
                Log::warning("Unable to create program: ", name);
        }
    }
}

/* ************************************************************************ */

}

/* ************************************************************************ */

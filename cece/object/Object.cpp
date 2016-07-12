/* ************************************************************************ */
/* Georgiev Lab (c) 2015-2016                                               */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/*                                                                          */
/* This file is part of CeCe.                                               */
/*                                                                          */
/* CeCe is free software: you can redistribute it and/or modify             */
/* it under the terms of the GNU General Public License as published by     */
/* the Free Software Foundation, either version 3 of the License, or        */
/* (at your option) any later version.                                      */
/*                                                                          */
/* CeCe is distributed in the hope that it will be useful,                  */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of           */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            */
/* GNU General Public License for more details.                             */
/*                                                                          */
/* You should have received a copy of the GNU General Public License        */
/* along with CeCe.  If not, see <http://www.gnu.org/licenses/>.            */
/*                                                                          */
/* ************************************************************************ */

// Declaration
#include "cece/object/Object.hpp"

// C++
#include <string>
#include <sstream>

// CeCe
#include "cece/core/Assert.hpp"
#include "cece/core/UnitIo.hpp"
#include "cece/core/Log.hpp"
#include "cece/core/Real.hpp"
#include "cece/core/FileStream.hpp"
#include "cece/config/Configuration.hpp"
#include "cece/plugin/Context.hpp"
#include "cece/simulator/Simulation.hpp"

// Box2D
#ifdef CECE_ENABLE_BOX2D_PHYSICS
#  include <Box2D/Box2D.h>
#  include "cece/simulator/ConverterBox2D.hpp"
#endif

/* ************************************************************************ */

namespace cece {
namespace object {

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

#ifdef CECE_ENABLE_BOX2D_PHYSICS
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

Object::Object(simulator::Simulation& simulation, String typeName, Type type) noexcept
    : m_simulation(simulation)
    , m_typeName(std::move(typeName))
    , m_id(++s_id)
    , m_type(type)
{
#ifdef CECE_ENABLE_BOX2D_PHYSICS
    auto& world = getSimulation().getWorld();

    b2BodyDef bodyDef;
    bodyDef.type = convert(type);
    bodyDef.userData = this;

    // Create body
    m_body = world.CreateBody(&bodyDef);

    // Pin the body
    if (m_type == Type::Pinned)
    {
        b2BodyDef pivotDef;
        pivotDef.position = m_body->GetWorldCenter();
        m_pinBody = world.CreateBody(&pivotDef);

        b2RevoluteJointDef jointDef;
        jointDef.collideConnected = false;
        jointDef.Initialize(m_pinBody, m_body, m_pinBody->GetPosition());

        m_pinJoint = world.CreateJoint(&jointDef);
    }
#endif
}

/* ************************************************************************ */

Object::~Object()
{
#ifdef CECE_ENABLE_BOX2D_PHYSICS
    auto& world = getSimulation().getWorld();

    // Pin the body
    if (m_type == Type::Pinned)
    {
        world.DestroyJoint(m_pinJoint);
        world.DestroyBody(m_pinBody);
    }

    Assert(m_body);
    world.DestroyBody(m_body);
#endif
}

/* ************************************************************************ */

units::PositionVector Object::getPosition() const noexcept
{
#ifdef CECE_ENABLE_BOX2D_PHYSICS
    Assert(m_body);
    return simulator::ConverterBox2D::getInstance().convertPosition(m_body->GetPosition());
#else
    return m_position;
#endif
}

/* ************************************************************************ */

units::PositionVector Object::getMassCenterPosition() const noexcept
{
#ifdef CECE_ENABLE_BOX2D_PHYSICS
    Assert(m_body);
    return simulator::ConverterBox2D::getInstance().convertPosition(m_body->GetWorldCenter());
#else
    return m_position;
#endif
}

/* ************************************************************************ */

units::PositionVector Object::getMassCenterOffset() const noexcept
{
#ifdef CECE_ENABLE_BOX2D_PHYSICS
    Assert(m_body);
    return simulator::ConverterBox2D::getInstance().convertPosition(m_body->GetLocalCenter());
#else
    return m_position;
#endif
}

/* ************************************************************************ */

units::PositionVector Object::getWorldPosition(units::PositionVector local) const noexcept
{
#ifdef CECE_ENABLE_BOX2D_PHYSICS
    Assert(m_body);
    return simulator::ConverterBox2D::getInstance().convertPosition(
        m_body->GetWorldPoint(simulator::ConverterBox2D::getInstance().convertPosition(local))
    );
#else
    return local;
#endif
}

/* ************************************************************************ */

units::Angle Object::getRotation() const noexcept
{
#ifdef CECE_ENABLE_BOX2D_PHYSICS
    Assert(m_body);
    return simulator::ConverterBox2D::getInstance().convertAngle(m_body->GetAngle());
#else
    return m_rotation;
#endif
}

/* ************************************************************************ */

units::VelocityVector Object::getVelocity() const noexcept
{
#ifdef CECE_ENABLE_BOX2D_PHYSICS
    Assert(m_body);
    return simulator::ConverterBox2D::getInstance().convertLinearVelocity(m_body->GetLinearVelocity());
#else
    return m_velocity;
#endif
}

/* ************************************************************************ */

units::AngularVelocity Object::getAngularVelocity() const noexcept
{
#ifdef CECE_ENABLE_BOX2D_PHYSICS
    Assert(m_body);
    return simulator::ConverterBox2D::getInstance().convertAngularVelocity(m_body->GetAngularVelocity());
#else
    return Zero;
#endif
}

/* ************************************************************************ */

units::Mass Object::getMass() const noexcept
{
#ifdef CECE_ENABLE_BOX2D_PHYSICS
    Assert(m_body);
    return simulator::ConverterBox2D::getInstance().convertMass(m_body->GetMass());
#else
    return {1};
#endif
}

/* ************************************************************************ */

units::Length Object::getMaxTranslation() const noexcept
{
#ifdef CECE_ENABLE_BOX2D_PHYSICS
    return simulator::ConverterBox2D::getInstance().getMaxObjectTranslation();
#else
    return units::Length(NAN);
#endif
}

/* ************************************************************************ */

void Object::setType(Type type) noexcept
{
    m_type = type;
#ifdef CECE_ENABLE_BOX2D_PHYSICS
    Assert(m_body);
    m_body->SetType(convert(type));
#endif
}

/* ************************************************************************ */

void Object::setPosition(units::PositionVector pos) noexcept
{
#ifdef CECE_ENABLE_BOX2D_PHYSICS
    Assert(m_body);
    m_body->SetTransform(simulator::ConverterBox2D::getInstance().convertPosition(pos), m_body->GetAngle());

    if (m_pinBody)
        m_pinBody->SetTransform(simulator::ConverterBox2D::getInstance().convertPosition(pos), 0);
#else
    m_position = std::move(pos);
#endif
}

/* ************************************************************************ */

void Object::setRotation(units::Angle angle) noexcept
{
#ifdef CECE_ENABLE_BOX2D_PHYSICS
    Assert(m_body);
    m_body->SetTransform(m_body->GetPosition(), simulator::ConverterBox2D::getInstance().convertAngle(angle));
#else
    m_rotation = angle;
#endif
}

/* ************************************************************************ */

void Object::setVelocity(units::VelocityVector vel) noexcept
{
#ifdef CECE_ENABLE_BOX2D_PHYSICS
    Assert(m_body);
    m_body->SetLinearVelocity(simulator::ConverterBox2D::getInstance().convertLinearVelocity(vel));
#else
    m_velocity = std::move(vel);
#endif
}

/* ************************************************************************ */

void Object::setAngularVelocity(units::AngularVelocity vel) noexcept
{
#ifdef CECE_ENABLE_BOX2D_PHYSICS
    Assert(m_body);
    m_body->SetAngularVelocity(simulator::ConverterBox2D::getInstance().convertAngularVelocity(vel));
#endif
}

/* ************************************************************************ */

void Object::applyForce(const units::ForceVector& force) noexcept
{
#ifdef CECE_ENABLE_BOX2D_PHYSICS
    Assert(m_body);
    m_body->ApplyForceToCenter(simulator::ConverterBox2D::getInstance().convertForce(force), true);
#endif
}

/* ************************************************************************ */

void Object::applyForce(const units::ForceVector& force, const units::PositionVector& offset) noexcept
{
#ifdef CECE_ENABLE_BOX2D_PHYSICS
    Assert(m_body);
    m_body->ApplyForce(
        simulator::ConverterBox2D::getInstance().convertForce(force),
        m_body->GetWorldPoint(simulator::ConverterBox2D::getInstance().convertPosition(offset)),
        true
    );

    m_force += force;
#endif
}

/* ************************************************************************ */

void Object::applyLinearImpulse(const units::ImpulseVector& impulse, const units::PositionVector& offset) noexcept
{
#ifdef CECE_ENABLE_BOX2D_PHYSICS
    Assert(m_body);
    m_body->ApplyLinearImpulse(
        simulator::ConverterBox2D::getInstance().convertLinearImpulse(impulse),
        m_body->GetWorldPoint(simulator::ConverterBox2D::getInstance().convertPosition(offset)),
        true
    );
#else
    m_velocity += 1 / getMass() * impulse;
#endif
}

/* ************************************************************************ */

void Object::applyAngularImpulse(const units::Impulse& impulse) noexcept
{
#ifdef CECE_ENABLE_BOX2D_PHYSICS
    Assert(m_body);
    m_body->ApplyAngularImpulse(simulator::ConverterBox2D::getInstance().convertAngularImpulse(impulse), true);
#endif
}

/* ************************************************************************ */

void Object::useProgram(StringView name) noexcept
{
    auto program = getSimulation().getProgram(name);

    if (program)
        addProgram(std::move(program));
    else
        Log::warning("Unable to create program '", name, "'");
}

/* ************************************************************************ */

void Object::destroy()
{
    getSimulation().deleteObject(this);
}

/* ************************************************************************ */

void Object::update(units::Duration dt)
{
#ifndef CECE_ENABLE_BOX2D_PHYSICS
    // Calculate new object position
    setPosition(getPosition() + getVelocity() * dt);
#endif

    // Call object programs
    m_programs.call(getSimulation(), *this, dt);

    // Store streamlines data
    if (m_dataOut)
    {
        const auto pos = getPosition();
        const auto posMass = getMassCenterPosition();
        const auto vel = getVelocity();
        const auto force = getForce();

        *m_dataOut <<
            // iteration
            m_simulation.getIteration() << ";" <<
            // totalTime
            m_simulation.getTotalTime().value() << ";" <<
            // id
            getId() << ";" <<
            // x
            pos.getX().value() << ";" <<
            // y
            pos.getY().value() << ";" <<
            // massX
            posMass.getX().value() << ";" <<
            // massY
            posMass.getY().value() << ";" <<
            // velX
            vel.getX().value() << ";" <<
            // velY
            vel.getY().value() << ";" <<
            // forceX
            force.getX().value() << ";" <<
            // forceY
            force.getY().value() << ";" <<
            // angle
            getRotation() << ";" <<
            // omega
            getAngularVelocity().value() <<
            "\n"
        ;

        m_dataOut->flush();
    }
}

/* ************************************************************************ */

void Object::configure(const config::Configuration& config, simulator::Simulation& simulation)
{
#ifdef CECE_ENABLE_RENDER
    setVisible(config.get("visible", isVisible()));
    setColor(config.get("color", getColor()));
#endif

    // Set object position
    setPosition(config.get("position", getPosition()));

    // Set object velocity
    setVelocity(config.get("velocity", getVelocity()));

    // Set object density
    setDensity(config.get("density", getDensity()));

    // Set object programs
    if (config.has("programs"))
    {
        for (const auto& name : split(config.get("programs"), ' '))
            useProgram(name);
    }

    if (config.has("data-out"))
    {
        m_dataOut = makeUnique<OutFileStream>(config.get("data-out"));
        *m_dataOut << "iteration;totalTime;id;x;y;massX;massY;velX;velY;forceX;forceY;angle;omega\n";
    }
}

/* ************************************************************************ */

void Object::initShapes()
{
#ifdef CECE_ENABLE_BOX2D_PHYSICS

    // Delete old fixtures
    for (b2Fixture* fixture = getBody()->GetFixtureList();
         fixture != nullptr;
         fixture = getBody()->GetFixtureList())
    {
        getBody()->DestroyFixture(fixture);
    }

    Assert(getBody()->GetFixtureList() == nullptr);

    // Delete shapes
    m_bodyShapes.clear();

    for (const auto& shape : getShapes())
    {
        // Get shape type
        const auto type = variantApplyVisitor(Shape::GetType(), shape);

        UniquePtr<b2Shape> bodyShape;

        switch (type)
        {
        case ShapeType::Undefined:
            Log::warning("[object] Undefined shape");
            break;

        case ShapeType::Circle:
        {
            // Create body shape
            auto ptr = makeUnique<b2CircleShape>();
            ptr->m_radius = simulator::ConverterBox2D::getInstance().convertLength(shape.getCircle().radius);
            ptr->m_p = simulator::ConverterBox2D::getInstance().convertPosition(shape.getCircle().center);
            bodyShape = std::move(ptr);
            break;
        }

        case ShapeType::Rectangle:
        {
            // Create body shape
            auto ptr = makeUnique<b2PolygonShape>();
            const auto sh = 0.5 * shape.getRectangle().size;
            b2Vec2 box = simulator::ConverterBox2D::getInstance().convertPosition(sh);
            ptr->SetAsBox(box.x, box.y);
            bodyShape = std::move(ptr);
            break;
        }

        case ShapeType::Edges:
        {
            DynamicArray<b2Vec2> vertices;

            for (const auto& v : shape.getEdges().edges)
                vertices.push_back(simulator::ConverterBox2D::getInstance().convertPosition(v));

            auto ptr = makeUnique<b2ChainShape>();

            // Create edges loop
            if (vertices.size() < 3)
                ptr->CreateChain(vertices.data(), static_cast<int32>(vertices.size()));
            else
                ptr->CreateLoop(vertices.data(), static_cast<int32>(vertices.size()));

            bodyShape = std::move(ptr);
            break;
        }
        }

        // Store body shape
        if (bodyShape)
        {
            getBody()->CreateFixture(bodyShape.get(), simulator::ConverterBox2D::getInstance().convertDensity(getDensity()));
            m_bodyShapes.push_back(std::move(bodyShape));
        }
    }
#endif
}

/* ************************************************************************ */

#ifdef CECE_ENABLE_RENDER
void Object::draw(const simulator::Visualization&, render::Context& context)
{
    draw(context);
}
#endif

/* ************************************************************************ */

#ifdef CECE_ENABLE_RENDER
void Object::draw(render::Context& context)
{
    // Nothing to do
}
#endif

/* ************************************************************************ */

#ifdef CECE_ENABLE_RENDER
void Object::drawStoreState(const simulator::Visualization&)
{
    drawStoreState();
}
#endif

/* ************************************************************************ */

#ifdef CECE_ENABLE_RENDER
void Object::drawStoreState()
{
    // Nothing to do
}
#endif

/* ************************************************************************ */

#ifdef CECE_ENABLE_RENDER
void Object::drawSwapState()
{
    // Nothing to do
}
#endif

/* ************************************************************************ */

}
}

/* ************************************************************************ */

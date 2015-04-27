
/* ************************************************************************ */

// Declaration
#include "Yeast.hpp"

// C++
#include <cmath>
#include <random>

// Simulator
#include "core/constants.hpp"
#include "simulator/Simulation.hpp"

/* ************************************************************************ */

namespace module {
namespace cell {

/* ************************************************************************ */

static std::random_device g_rd;

/* ************************************************************************ */

Yeast::Yeast(simulator::Simulation& simulation, simulator::Object::Type type) noexcept
    : CellBase(simulation, type)
{
    setVolume(units::um3(37));

    // Max 2 shapes
    getShapes().reserve(2);
    getShapes().push_back(Shape::makeCircle(calcSphereRadius(getVolume())));
}

/* ************************************************************************ */

Yeast::~Yeast()
{
    // Nothing to do
}

/* ************************************************************************ */

void Yeast::update(units::Duration dt)
{
    const float RATIO = 5.f;

    std::mt19937 eng(g_rd());
    std::bernoulli_distribution dist(0.1f * dt);
    std::uniform_real_distribution<float> add_coeff(0.1f, 4.f);

    // Volume increase
    auto volumeAdd = dt * RATIO * add_coeff(eng);

    if (hasBud())
    {
        m_bud->volume += volumeAdd;

        if (m_bud->volume >= units::um3(35))
        {
            budRelease();
        }
    }
    else if (getVolume() >= units::um3(42) && dist(eng))
    {
        budCreate();
    }
    else
    {
        setVolume(getVolume() + volumeAdd);
    }

#if ENABLE_PHYSICS
    // Update cell shape
    updateShape();
#endif

    CellBase::update(dt);
}

/* ************************************************************************ */

void Yeast::budCreate()
{
    assert(!hasBud());

    std::random_device rd;
    std::default_random_engine eng(g_rd());
    std::uniform_real_distribution<float> dist(0.f, 1.f);

    m_bud.reset(new Bud{});
    m_bud->rotation = 2 * constants::PI * dist(eng);

    m_shapeForceUpdate = true;
}

/* ************************************************************************ */

void Yeast::budRelease()
{
    assert(hasBud());

    // Calculate bud position
    const auto angle = getBody()->GetAngle() + m_bud->rotation;
    const auto distance = m_bud->shape.m_p.Length();

    // Get current position
    const auto pos = getPosition() + Vector<float>(
        distance * std::cos(angle),
        distance * std::sin(angle)
    );

    // Release bud into the world
    auto bud = getSimulation().createObject<Yeast>();
    bud->setVolume(m_bud->volume);
    bud->setPosition(pos);
    bud->setVelocity(getVelocity());

    // TODO: When yeast is rotating and bud is released it should be throw
    // away by some force.

    // Release bud
    m_bud.release();

    m_shapeForceUpdate = true;
}

/* ************************************************************************ */

#ifdef ENABLE_RENDER
void Yeast::drawInit(render::Context& context)
{
    m_renderObject.init(context);
}
#endif

/* ************************************************************************ */

#ifdef ENABLE_RENDER
void Yeast::draw(render::Context& context)
{
    auto pos = getPosition();
    auto radius = calcSphereRadius(getVolume());
    const auto angle = getBody()->GetAngle() - (m_bud ? m_bud->rotation : 0.0f);
    const auto budRadius = m_bud ? calcSphereRadius(m_bud->volume) : 0.0f;

    // Transform
    context.matrixPush();
    context.matrixTranslate(pos);
    context.matrixRotate(angle);
    context.matrixScale(2 * radius);
    m_renderObject.draw(context, 0.5, 0.5 * (budRadius / radius));
    context.matrixPop();
}
#endif

/* ************************************************************************ */

#if ENABLE_PHYSICS
void Yeast::updateShape()
{
    static constexpr float MIN_CHANGE = 0.2f;

    // Alias for yeast shapes
    auto& shapes = getShapes();

    // Calculate new radius
    const units::Length newRadius = calcSphereRadius(getVolume());
    const units::Length oldRadius = m_shape.m_radius;
    const units::Length newBudRadius = m_bud ? calcSphereRadius(m_bud->volume) : 0;
    const units::Length oldBudRadius = m_bud ? m_bud->shape.m_radius : 0;

    const bool needs_update = m_shapeForceUpdate ||
        ((newRadius - oldRadius) > MIN_CHANGE) ||
        ((newBudRadius - oldBudRadius) > MIN_CHANGE)
    ;

    // Update main shape
    assert(shapes.size() >= 1);
    shapes[0].circle.radius = newRadius;

    // If bud shape is missing, create one.
    if (hasBud())
    {
        if (shapes.size() != 2)
        {
            shapes.push_back(Shape::makeCircle(newBudRadius, PositionVector{0, newRadius + newBudRadius}));
        }
        else
        {
            shapes[1].circle.radius = newBudRadius;
            shapes[1].circle.center.getY() = newRadius + newBudRadius;
        }
    }
    else
    {
        shapes.resize(1);
    }

    if (!needs_update)
        return;

    // Delete old fixtures
    for (b2Fixture* fixture = getBody()->GetFixtureList();
         fixture != nullptr;
         fixture = fixture->GetNext())
    {
        getBody()->DestroyFixture(fixture);
    }

    // Update main yeast shape
    {
        m_shape.m_radius = newRadius;
        getBody()->CreateFixture(&m_shape, 1.0f);
    }

    // Update bud shape
    if (m_bud)
    {
        m_bud->shape.m_radius = newBudRadius;

        // Distance between yeast and bud
        const float distance = m_shape.m_radius + m_bud->shape.m_radius;

        m_bud->shape.m_p = b2Vec2(
            distance * std::sin(m_bud->rotation),
            distance * std::cos(m_bud->rotation)
        );
        getBody()->CreateFixture(&m_bud->shape, 1.0f);
    }

    m_shapeForceUpdate = false;
}
#endif

/* ************************************************************************ */

}
}

/* ************************************************************************ */

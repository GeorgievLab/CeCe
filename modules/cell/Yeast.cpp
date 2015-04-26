
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
    //m_bud->rotation = 2 * constants::PI * dist(eng);

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

    // Transform
    context.matrixPush();
    context.matrixTranslate(pos);
    context.matrixRotate(getBody()->GetAngle());

    context.matrixPush();
    context.matrixScale(2 * radius);
    m_renderObject.draw(context, radius);
    context.matrixPop();

    if (m_bud)
    {
        const auto angle = m_bud->rotation;
        const auto budRadius = calcSphereRadius(m_bud->volume);
        const auto distance = radius + budRadius;

        context.matrixPush();
        context.matrixTranslate({distance, 0});
        context.matrixRotate(angle);
        context.matrixScale(2 * budRadius);

        m_renderObject.draw(context, budRadius);
        context.matrixPop();
    }

    context.matrixPop();
}
#endif

/* ************************************************************************ */

#if ENABLE_PHYSICS
void Yeast::updateShape()
{
    static constexpr float MIN_CHANGE = 0.2f;

    // Calculate new radius
    const units::Length newRadius = calcSphereRadius(getVolume());
    const units::Length oldRadius = m_shape.m_radius;
    const units::Length newBudRadius = m_bud ? calcSphereRadius(m_bud->volume) : 0;
    const units::Length oldBudRadius = m_bud ? m_bud->shape.m_radius : 0;

    const bool needs_update = m_shapeForceUpdate ||
        ((newRadius - oldRadius) > MIN_CHANGE) ||
        ((newBudRadius - oldBudRadius) > MIN_CHANGE)
    ;

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

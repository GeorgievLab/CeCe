
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

Yeast::Yeast(simulator::Simulation& simulation) noexcept
    : Cell(simulation)
{
    setFlag(OBJECT_YEAST);
    setVolume(units::um3(37));
}

/* ************************************************************************ */

#ifdef ENABLE_PHYSICS
Yeast::Yeast(simulator::Simulation& simulation, b2Body* body) noexcept
    : Cell(simulation, body)
{
    setFlag(OBJECT_YEAST);
    setVolume(units::um3(37));
}
#endif

/* ************************************************************************ */

Yeast::~Yeast()
{
#ifdef ENABLE_PHYSICS
    if (hasBud())
    {
        //getSimulation().getWorld().DestroyJoint(m_bud.joint);
        getSimulation().getWorld().DestroyBody(m_bud.body);
    }
#endif
}

/* ************************************************************************ */

void Yeast::update(units::Duration dt)
{
    const float RATIO = 1.f;

    std::mt19937 eng(g_rd());
    std::bernoulli_distribution dist(0.2f);
    std::uniform_real_distribution<float> add_coeff(0.1f, 4.f);

    if (hasBud())
    {
        m_bud.volume += dt * RATIO * (m_bud.volume + 1.f) * add_coeff(eng);

        if (m_bud.volume >= units::um3(35))
        {
            budRelease();
        }
#ifdef ENABLE_PHYSICS
        else
        {
            // Calculate cell radius
            m_bud.shape.m_radius = calcSphereRadius(m_bud.volume);

            // Update fixture
            b2Fixture* fixture = m_bud.body->GetFixtureList();
            assert(fixture);
            m_bud.body->DestroyFixture(fixture);

            b2FixtureDef fixtureDef;
            fixtureDef.shape = &m_bud.shape;
            fixtureDef.density = 1.0f;
            fixtureDef.friction = 0.3f;
            m_bud.body->CreateFixture(&fixtureDef);

            // Update joint
            b2DistanceJointDef jointDef;
            jointDef.Initialize(getBody(), m_bud.body, getBody()->GetWorldCenter(), m_bud.body->GetWorldCenter());
            jointDef.collideConnected = true;
            jointDef.length = (1.01f * getShape().m_radius) + m_bud.shape.m_radius;

            getSimulation().getWorld().DestroyJoint(m_bud.joint);
            m_bud.joint = static_cast<b2DistanceJoint*>(getSimulation().getWorld().CreateJoint(&jointDef));
        }
#endif
    }
    else if (getVolume() >= units::um3(42) && dist(eng))
    {
        budCreate();
    }
    else
    {
        setVolume(getVolume() + dt * RATIO);
    }

    Cell::update(dt);
}

/* ************************************************************************ */

void Yeast::budCreate()
{
    // Release previous bud
    assert(!hasBud());

    std::mt19937 eng(g_rd());
    std::uniform_real_distribution<float> dist(0.f, 1.f);

    const float theta = 2 * constants::PI * dist(eng);

    m_bud.volume = units::um3(0.1f);
    m_bud.exists = true;
    const auto radius = calcSphereRadius(getVolume());
    auto pos = getBody()->GetPosition();
    pos.x += (radius * std::sin(theta));
    pos.y += (radius * std::cos(theta));

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;

    // Create body
    m_bud.body = getSimulation().getWorld().CreateBody(&bodyDef);
    m_bud.body->SetTransform(pos, 0);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &m_bud.shape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    m_bud.body->CreateFixture(&fixtureDef);

    b2DistanceJointDef jointDef;
    jointDef.Initialize(getBody(), m_bud.body, getBody()->GetWorldCenter(), m_bud.body->GetWorldCenter());
    jointDef.length = radius;

    m_bud.joint = static_cast<b2DistanceJoint*>(getSimulation().getWorld().CreateJoint(&jointDef));
}

/* ************************************************************************ */

Yeast* Yeast::budRelease()
{
    if (!hasBud())
        return nullptr;

    // Release bud into the world
    Yeast* bud = getSimulation().createObject<Yeast>(m_bud.body);
    m_bud.body = nullptr;

    // Copy properties
    bud->setVolume(m_bud.volume);
    bud->setVelocity(getVelocity());

    // Split Xfp proteins
    const float ratio = bud->getVolume() / getVolume();
    const auto gfp = getGfp();
    const auto rfp = getRfp();
    const auto yfp = getYfp();

    setGfp((1 - ratio) * gfp);
    setRfp((1 - ratio) * rfp);
    setYfp((1 - ratio) * yfp);

    bud->setGfp(ratio * gfp);
    bud->setRfp(ratio * rfp);
    bud->setYfp(ratio * yfp);

    m_bud.exists = false;

#ifdef ENABLE_PHYSICS
    getSimulation().getWorld().DestroyJoint(m_bud.joint);
#endif

    return bud;
}

/* ************************************************************************ */

#ifdef ENABLE_RENDER
void Yeast::draw(render::Context& context)
{
    auto volume = getVolume();

    if (m_bud.exists)
        volume += m_bud.volume;

    auto gfp = getGfp();
    auto rfp = getRfp();
    auto yfp = getYfp();

    // TODO: Better calculation
    float red = (rfp / 1000.f + yfp / 1000.f) / (volume / 10000.f);
    float green = (gfp / 1000.f + yfp / 1000.f) / (volume / 10000.f);
    float blue = 0;

    // Yeast position
    auto pos = getPosition();
    const auto radius = getShape().m_radius;

    // Draw yeast
    getRenderObject().draw(pos, radius, {red, green, blue, 0.5f});

    // Draw bud
    if (m_bud.exists)
    {
        const auto radiusBud = m_bud.shape.m_radius;
        auto wpos = m_bud.body->GetPosition();

        // Draw yeast
        getRenderObject().draw({wpos.x, wpos.y}, radiusBud, {red, green, blue, 0.5f});
    }
}
#endif

/* ************************************************************************ */

}
}

/* ************************************************************************ */

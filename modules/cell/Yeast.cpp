
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

Yeast::Yeast(simulator::Simulation& simulation, Yeast* parent) noexcept
    : Cell(simulation, simulator::Object::Type::Dynamic)
    , m_parent(parent)
{
    if (m_parent)
    {
        // Cell is bud
        setVolume(units::um3(1));
    }
    else
    {
        setVolume(units::um3(37));
    }
}

/* ************************************************************************ */

Yeast::~Yeast()
{
    // Nothing to do
}

/* ************************************************************************ */

void Yeast::update(units::Duration dt)
{
    const float RATIO = 1.f;

    std::mt19937 eng(g_rd());
    std::bernoulli_distribution dist(0.2f);
    std::uniform_real_distribution<float> add_coeff(0.1f, 4.f);

    // Volume increase
    auto volumeAdd = dt * RATIO * add_coeff(eng);

    if (hasBud())
    {
        m_bud->setVolume(m_bud->getVolume() + volumeAdd);

        if (m_bud->getVolume() >= units::um3(35))
        {
            budRelease();
        }
#if ENABLE_PHYSICS
        else
        {
            // Calculate cell radius
            m_bud->getShape().m_radius = calcSphereRadius(m_bud->getVolume());

            // Update fixture
            b2Fixture* fixture = m_bud->getBody()->GetFixtureList();
            assert(fixture);
            m_bud->getBody()->DestroyFixture(fixture);

            b2FixtureDef fixtureDef;
            fixtureDef.shape = &m_bud->getShape();
            fixtureDef.density = 1.0f;
            fixtureDef.friction = 0.3f;
            m_bud->getBody()->CreateFixture(&fixtureDef);

            // Update joint
            b2DistanceJointDef jointDef;
            jointDef.Initialize(getBody(), m_bud->getBody(), getBody()->GetWorldCenter(), m_bud->getBody()->GetWorldCenter());
            jointDef.collideConnected = true;
            jointDef.length = (1.01f * getShape().m_radius) + m_bud->getShape().m_radius;

            getSimulation().getWorld().DestroyJoint(m_joint);
            m_joint = static_cast<b2DistanceJoint*>(getSimulation().getWorld().CreateJoint(&jointDef));
        }
#endif
    }
    else if (getVolume() >= units::um3(42) && dist(eng))
    {
        budCreate();
    }
    else
    {
        setVolume(getVolume() + volumeAdd);
    }

    Cell::update(dt);
}

/* ************************************************************************ */

void Yeast::budCreate()
{
    assert(!hasBud());

    std::mt19937 eng(g_rd());
    std::uniform_real_distribution<float> dist(0.f, 1.f);

    const float theta = 2 * constants::PI * dist(eng);

    const auto radius = calcSphereRadius(getVolume());
    auto pos = getPosition();
    pos.getX() += (radius * std::sin(theta));
    pos.getY() += (radius * std::cos(theta));

    // Create bud
    m_bud = getSimulation().createObject<Yeast>(this);
    m_bud->setPosition(pos);

#if ENABLE_PHYSICS
    b2DistanceJointDef jointDef;
    jointDef.Initialize(getBody(), m_bud->getBody(), getBody()->GetWorldCenter(), m_bud->getBody()->GetWorldCenter());
    jointDef.length = radius;
    m_joint = static_cast<b2DistanceJoint*>(getSimulation().getWorld().CreateJoint(&jointDef));
#endif
}

/* ************************************************************************ */

void Yeast::budRelease()
{
    assert(hasBud());

#ifdef ENABLE_PHYSICS
    // Destroy joint
    getSimulation().getWorld().DestroyJoint(m_joint);
#endif

    // Release bud into the world
    m_bud->m_parent = nullptr;
    m_bud = nullptr;
}

/* ************************************************************************ */

#ifdef ENABLE_RENDER
void Yeast::draw(render::Context& context)
{
    // TODO: Better calculation
    float red = 1;
    float green = 1;
    float blue = 1;

    // Yeast position
    auto pos = getPosition();
    const auto radius = getShape().m_radius;

    // Draw yeast
    getRenderObject().draw(pos, radius, {red, green, blue, 0.5f});
}
#endif

/* ************************************************************************ */

}
}

/* ************************************************************************ */

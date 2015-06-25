
/* ************************************************************************ */

// Declaration
#include "Yeast.hpp"

// C++
#include <cmath>
#include <random>

// Simulator
#include "core/constants.hpp"
#include "simulator/Simulation.hpp"
#include "simulator/Shape.hpp"

/* ************************************************************************ */

namespace module {
namespace cell {

/* ************************************************************************ */

static std::random_device g_rd;

/* ************************************************************************ */

Yeast::Yeast(simulator::Simulation& simulation, simulator::Object::Type type) NOEXCEPT
    : CellBase(simulation, type)
{
	setVolume(units::um3(37));
    // TODO: set density
    setDensity(units::kg(1200) / units::m3(1));

    // Max 2 shapes
    auto& shapes = getMutableShapes();
    shapes.reserve(2);
    shapes.push_back(simulator::Shape::makeCircle(calcSphereRadius(getVolume())));
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
    std::bernoulli_distribution dist(0.1f * dt.value());
    std::uniform_real_distribution<float> add_coeff(0.1f, 4.f);

    // Volume increase
    auto volumeAdd = units::Volume(dt.value() * RATIO * add_coeff(eng));

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

void Yeast::configure(const simulator::Configuration& config,
                      simulator::Simulation& simulation)
{
    CellBase::configure(config, simulation);
}

/* ************************************************************************ */

void Yeast::budCreate()
{
    assert(!hasBud());

    std::random_device rd;
    std::default_random_engine eng(g_rd());
    std::uniform_real_distribution<float> dist(0.f, 1.f);

    m_bud = Bud{};
    m_bud->rotation = 2 * constants::PI * dist(eng);

#if ENABLE_PHYSICS
    m_shapeForceUpdate = true;
#endif
}

/* ************************************************************************ */

void Yeast::budRelease()
{
    assert(hasBud());

    // Calculate bud position
    const auto angle = getRotation();
#if ENABLE_PHYSICS
    const auto offset = PositionVector(units::Length(m_bud->shape.m_p.x), units::Length(m_bud->shape.m_p.y));
#else
    const auto offset = m_bud->offset;
#endif

    // Get current position
    const auto pos = getPosition() + offset.rotated(angle);

    // Release bud into the world
    auto bud = getSimulation().createObject<Yeast>();
    bud->setVolume(m_bud->volume);
    bud->setPosition(pos);
    bud->setVelocity(getVelocity());
    bud->setPrograms(getPrograms());

    // TODO: When yeast is rotating and bud is released it should be throw
    // away by some force.

    // Release bud
    m_bud.reset();

#if ENABLE_PHYSICS
    m_shapeForceUpdate = true;
#endif
}

/* ************************************************************************ */

#if ENABLE_RENDER
void Yeast::draw(render::Context& context)
{
    if (!m_renderObject)
        m_renderObject.create(context);

    auto pos = getPosition();
    auto radius = calcSphereRadius(getVolume());
    const auto angle = getRotation() - (m_bud ? m_bud->rotation : units::Angle(0));
    const auto budRadius = m_bud ? calcSphereRadius(m_bud->volume) : units::Length(0);

    // Transform
    context.matrixPush();
    context.matrixTranslate(pos);
    context.matrixRotate(angle);
    context.matrixScale(2 * radius.value());
    m_renderObject->draw(context, 0.5f, 0.5f * (budRadius / radius));
    context.matrixPop();
}
#endif

/* ************************************************************************ */

#if ENABLE_PHYSICS
void Yeast::updateShape()
{
    static CONSTEXPR_CONST auto MIN_CHANGE = units::Length(0.2f);

    // Alias for yeast shapes
    auto& shapes = getMutableShapes();

    // Calculate new radius
    const auto newRadius = calcSphereRadius(getVolume());
    const auto oldRadius = units::Length(m_shape.m_radius);
    const auto newBudRadius = m_bud ? calcSphereRadius(m_bud->volume) : units::Length(0);
    const auto oldBudRadius = m_bud ? units::Length(m_bud->shape.m_radius) : units::Length(0);

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
            shapes.push_back(simulator::Shape::makeCircle(
                newBudRadius, PositionVector{ units::Length(0), newRadius + newBudRadius })
            );
        }
        else
        {
            shapes[1].circle.radius = newBudRadius;
            shapes[1].circle.center.y() = newRadius + newBudRadius;
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
         //fixture = fixture->GetNext())
         fixture = getBody()->GetFixtureList())
    {
        getBody()->DestroyFixture(fixture);
    }

    assert(getBody()->GetFixtureList() == nullptr);

    // Update main yeast shape
    {
        m_shape.m_radius = newRadius.value();
        getBody()->CreateFixture(&m_shape, getDensity().value());
    }

    // Update bud shape
    if (m_bud)
    {
        m_bud->shape.m_radius = newBudRadius.value();

        // Distance between yeast and bud
        const float distance = m_shape.m_radius + m_bud->shape.m_radius;

        m_bud->shape.m_p = b2Vec2(
            distance * std::sin(m_bud->rotation),
            distance * std::cos(m_bud->rotation)
        );
        getBody()->CreateFixture(&m_bud->shape, getDensity().value());
    }

    m_shapeForceUpdate = false;
}
#endif

/* ************************************************************************ */

}
}

/* ************************************************************************ */


/* ************************************************************************ */

// Declaration
#include "Cell.hpp"

/* ************************************************************************ */

namespace module {
namespace cell {

/* ************************************************************************ */

Cell::Cell(simulator::Simulation& simulation, simulator::Object::Type type) noexcept
    : simulator::Object(simulation, type)
{
#if ENABLE_PHYSICS
    m_shape.m_radius = calcSphereRadius(getVolume());

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &m_shape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    getBody()->CreateFixture(&fixtureDef);
#endif
}

/* ************************************************************************ */

void Cell::update(units::Duration dt)
{
    Object::update(dt);

#if ENABLE_PHYSICS
    // Calculate cell radius
    m_shape.m_radius = calcSphereRadius(getVolume());

    // Update fixture
    b2Fixture* fixture = getBody()->GetFixtureList();
    assert(fixture);
    getBody()->DestroyFixture(fixture);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &m_shape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    getBody()->CreateFixture(&fixtureDef);
#endif
}

/* ************************************************************************ */

#ifdef ENABLE_RENDER
void Cell::drawInit(render::Context& context)
{
    m_renderObject.init();
}
#endif

/* ************************************************************************ */

#ifdef ENABLE_RENDER
void Cell::draw(render::Context& context)
{
    auto gfp = getGfp();
    auto rfp = getRfp();
    auto yfp = getYfp();

    // TODO: Better calculation
    float red = 0.5f + rfp / 1000.f + yfp / 1000.f;
    float green = 0.5f + gfp / 1000.f + yfp / 1000.f;
    float blue = 0.5f + 0;

    const auto pos = getPosition();
    const auto radius = calcSphereRadius(getVolume());

    m_renderObject.draw(pos, radius, {red, green, blue, 0.8f});
}
#endif

/* ************************************************************************ */

}
}

/* ************************************************************************ */

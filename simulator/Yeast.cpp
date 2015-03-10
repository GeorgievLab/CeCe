
/* ************************************************************************ */

// Declaration
#include "simulator/Yeast.hpp"

// C++
#include <cmath>
#include <random>

// Core
#include "simulator/World.hpp"

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

Yeast::Yeast(World* world)
    : Cell(world, Cell::Shape::Sphere)
{
    // Nothing to do
}

/* ************************************************************************ */

Yeast::~Yeast()
{
    // Nothing to do
}

/* ************************************************************************ */

void Yeast::update()
{
    Cell::update();
}

/* ************************************************************************ */

void Yeast::budCreate()
{
    static constexpr float PI = 3.14;

    // Release previous bud
    if (m_bud)
        budRelease();

    std::random_device rd;
    std::default_random_engine eng(rd());
    std::uniform_real_distribution<float> dist(0.f, 1.f);

    const float theta = 2 * PI * dist(eng);
    const float phi =   2 * PI * dist(eng);

    m_bud.reset(new Bud(this, Angle(theta), Angle(phi)));
}

/* ************************************************************************ */

void Yeast::budRelease()
{
    if (!m_bud)
        return;

    // Release bud into the world
    Yeast* bud = getWorld()->newCell<Yeast>();

    // Copy properties
    bud->setBehaviour(getBehaviour());
    bud->setDensity(getDensity());
    bud->setVolume(m_bud->getVolume());

    const float theta = m_bud->getTheta().value();
    const float phi =   m_bud->getPhi().value();
    const auto radius = calcSphereRadius(getVolume()).value();

    // Calculate bud position
    auto pos = getPosition();
    pos.x += Length((radius + 1) * std::sin(theta) * std::cos(phi));
    pos.y += Length((radius + 1) * std::cos(theta) * std::sin(phi));
    pos.z += Length((radius + 1) * std::cos(theta));

    bud->setPosition(pos);

    // Split Xfp proteins
    const float ratio = m_bud->getVolume().value() / getVolume().value();
    const auto gfp = getGfp();
    const auto rfp = getRfp();
    const auto yfp = getYfp();

    setGfp((1 - ratio) * gfp);
    setRfp((1 - ratio) * rfp);
    setYfp((1 - ratio) * yfp);

    bud->setGfp(ratio * gfp);
    bud->setRfp(ratio * rfp);
    bud->setYfp(ratio * yfp);

    m_bud.release();

    // Update bud
    bud->update();
}

/* ************************************************************************ */

#ifdef ENABLE_RENDER

void Yeast::draw(render::Context& context)
{
    auto volume = getVolume();

    if (m_bud)
        volume += m_bud->getVolume();

    auto gfp = getGfp();
    auto rfp = getRfp();
    auto yfp = getYfp();

    // TODO: Better calculation
    float red = (rfp / 1000.f + yfp / 1000.f) / (volume.value() / 10000.f);
    float green = (gfp / 1000.f + yfp / 1000.f) / (volume.value() / 10000.f);
    float blue = 0;

    // Yeast position
    auto pos = getPosition();

    // Yeast radius
    const auto radius = calcSphereRadius(getVolume()).value();

    // Draw yeast
    context.drawSphere({pos.x.value(), pos.y.value(), pos.z.value()}, radius, {red, green, blue, 0.5f});

    // Draw bud
    if (m_bud)
    {
        const float theta = m_bud->getTheta().value();
        const float phi   = m_bud->getPhi().value();
        const auto radius = calcSphereRadius(getVolume()).value();
        const auto radiusBud = calcSphereRadius(m_bud->getVolume()).value();

        pos = getPosition();
        pos.x += Length((radius + radiusBud) * std::sin(theta) * std::cos(phi));
        pos.y += Length((radius + radiusBud) * std::cos(theta) * std::sin(phi));
        pos.z += Length((radius + radiusBud) * std::cos(theta));

        context.drawSphere({pos.x.value(), pos.y.value(), pos.z.value()}, radiusBud, {red, green, blue, 0.5f});
    }
}

#endif

/* ************************************************************************ */

}

/* ************************************************************************ */

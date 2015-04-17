
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

Yeast::~Yeast()
{
    // Nothing to do
}

/* ************************************************************************ */

void Yeast::update(units::Duration dt)
{
    const float RATIO = 0.01f;

    Cell::update(dt);

    if (hasBud())
    {
        m_bud.volume += dt * RATIO;

        if (m_bud.volume >= units::um3(0.05f))
        {
            budRelease();
        }
    }
    else if (getVolume() >= units::um3(0.05f))
    {
        budCreate();
    }
    else
    {
        setVolume(getVolume() + dt * RATIO);
    }
}

/* ************************************************************************ */

void Yeast::budCreate()
{
    // Release previous bud
    if (hasBud())
        budRelease();

    std::random_device rd;
    std::default_random_engine eng(rd());
    std::uniform_real_distribution<float> dist(0.f, 1.f);

    m_bud.volume = units::um3(0.001f);
    m_bud.theta = 2 * constants::PI * dist(eng);
    m_bud.exists = true;
}

/* ************************************************************************ */

Yeast* Yeast::budRelease()
{
    if (!hasBud())
        return nullptr;

    // Release bud into the world
    Yeast* bud = getSimulation().createObject<Yeast>();

    // Copy properties
    bud->setVolume(m_bud.volume);

    const auto theta = m_bud.theta;
    const auto radius = calcSphereRadius(getVolume());

    // Calculate bud position
    auto pos = getPosition();
    pos.getX() += ((radius + 1) * std::sin(theta));
    pos.getY() += ((radius + 1) * std::cos(theta));

    bud->setPosition(pos);

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

    // Yeast radius
    const auto radius = calcSphereRadius(getVolume());

    // Draw yeast
    getRenderObject().draw(pos, radius, {red, green, blue, 0.5f});

    // Draw bud
    if (m_bud.exists)
    {
        const float theta = m_bud.theta;
        const auto radius = calcSphereRadius(getVolume());
        const auto radiusBud = calcSphereRadius(m_bud.volume);

        pos = getPosition();
        pos.getX() += ((radius + radiusBud) * std::sin(theta));
        pos.getY() += ((radius + radiusBud) * std::cos(theta));

        // Draw yeast
        getRenderObject().draw(pos, radius, {red, green, blue, 0.5f});
    }
}

#endif

/* ************************************************************************ */

}
}

/* ************************************************************************ */

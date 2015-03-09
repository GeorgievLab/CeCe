
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

Volume Yeast::getVolume() const noexcept
{
    //if (m_bud)
    //{
    //    return Cell::getVolume() + m_bud->getVolume();
    //}
    //else
    {
        return Cell::getVolume();
    }
}

/* ************************************************************************ */

void Yeast::setVolume(Volume volume) noexcept
{
    //if (m_bud)
    //{
    //    auto diff = volume - Cell::getVolume();
    //    m_bud->setVolume(m_bud->getVolume() + diff);
    //}
    //else
    {
        Cell::setVolume(volume);
    }
}

/* ************************************************************************ */

void Yeast::update()
{
    Cell::update();
}

/* ************************************************************************ */

void Yeast::budCreate(Volume volume)
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
    const auto radius = calcSphereRadius(getVolume()).value();

    // Calculate bud position
    auto pos = getPosition();
    pos.x += Length((radius + 1) * std::sin(theta) * std::cos(phi));
    pos.y += Length((radius + 1) * std::cos(theta) * std::sin(phi));
    pos.z += Length((radius + 1) * std::cos(theta));

    m_bud.reset(new Yeast(getWorld()));
    m_bud->setVolume(volume);
    m_bud->setBehaviour(getBehaviour());
    m_bud->setPosition(pos);
}

/* ************************************************************************ */

void Yeast::budRelease()
{
    if (!m_bud)
        return;

    // Release bud into the world
    getWorld()->addCell(std::move(m_bud));
    assert(!m_bud.get());
}

/* ************************************************************************ */

#ifdef ENABLE_RENDER

void Yeast::draw(render::Context& context)
{
    Cell::draw(context);

    if (m_bud)
        m_bud->Cell::draw(context);
}

#endif

/* ************************************************************************ */

}

/* ************************************************************************ */

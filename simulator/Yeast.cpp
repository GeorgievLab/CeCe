
/* ************************************************************************ */

// Declaration
#include "simulator/Yeast.hpp"

// Core
#include "simulator/World.h"

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
    if (m_bud)
    {
        return Cell::getVolume() + m_bud->getVolume();
    }
    else
    {
        return Cell::getVolume();
    }
}

/* ************************************************************************ */

void Yeast::setVolume(Volume volume) noexcept
{
    if (m_bud)
    {
        auto diff = volume - Cell::getVolume();
        m_bud->setVolume(m_bud->getVolume() + diff);
    }
    else
    {
        Cell::setVolume(volume);
    }
}

/* ************************************************************************ */

void Yeast::update()
{
    Cell::update();

    if (!m_bud)
    {
        if (getVolume() > 5000_um3)
        {
            m_bud.reset(new Yeast(getWorld()));
        }
    }
    else
    {

    }
}

/* ************************************************************************ */

}

/* ************************************************************************ */

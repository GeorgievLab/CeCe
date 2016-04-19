/* ************************************************************************ */
/* Georgiev Lab (c) 2015-2016                                               */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/*                                                                          */
/* This file is part of CeCe.                                               */
/*                                                                          */
/* CeCe is free software: you can redistribute it and/or modify             */
/* it under the terms of the GNU General Public License as published by     */
/* the Free Software Foundation, either version 3 of the License, or        */
/* (at your option) any later version.                                      */
/*                                                                          */
/* CeCe is distributed in the hope that it will be useful,                  */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of           */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            */
/* GNU General Public License for more details.                             */
/*                                                                          */
/* You should have received a copy of the GNU General Public License        */
/* along with CeCe.  If not, see <http://www.gnu.org/licenses/>.            */
/*                                                                          */
/* ************************************************************************ */

// Declaration
#include "cece/plugins/cell/Yeast.hpp"

// C++
#include <random>

// CeCe
#include "cece/core/Assert.hpp"
#include "cece/core/constants.hpp"
#include "cece/core/Shape.hpp"
#include "cece/core/UnitIo.hpp"
#include "cece/simulator/Simulation.hpp"

#ifdef CECE_ENABLE_RENDER
#  include "cece/render/Color.hpp"
#endif

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace cell {

/* ************************************************************************ */

static std::random_device g_rd;

/* ************************************************************************ */

Yeast::Yeast(simulator::Simulation& simulation, String typeName, object::Object::Type type) noexcept
    : CellBase(simulation, std::move(typeName), type)
{
    setVolume(units::um3(13));
    setDensity(units::kg(1200) / units::m3(1));

    // Max 2 shapes
    auto& shapes = getMutableShapes();
    shapes.reserve(2);
    shapes.push_back(Shape::makeCircle(calcRadius(getVolume())));

    //getBody()->SetAngularDamping(10);
}

/* ************************************************************************ */

Yeast::~Yeast()
{
    // Nothing to do
}

/* ************************************************************************ */

void Yeast::update(units::Duration dt)
{
#ifdef CECE_THREAD_SAFE
    // Lock access
    MutexGuard guard(m_mutex);
#endif

    const auto volume0 = getVolume();
    CellBase::update(dt);
    const auto volume1 = getVolume();

    // Volume change
    const auto volumeDiff = volume1 - volume0;

    if (hasBud())
    {
        m_bud.volume += volumeDiff;
        setVolume(getVolume() - volumeDiff);

        if (m_bud.volume >= getVolumeBudRelease())
        {
            budRelease();
        }
    }
    else if (getVolume() >= getVolumeBudCreate())
    {
        budCreate();
    }

    // Update cell shape
    updateShape();
}

/* ************************************************************************ */

void Yeast::configure(const config::Configuration& config, simulator::Simulation& simulation)
{
    CellBase::configure(config, simulation);

    setVolumeBud(config.get("volume-bud", getVolumeBud()));
    setAngleBud(config.get("angle-bud", getAngleBud()));
    setVolumeBudCreate(config.get("volume-bud-create", getVolumeBudCreate()));
    setVolumeBudRelease(config.get("volume-bud-release", getVolumeBudRelease()));

    // Update cell shape
    updateShape();
}

/* ************************************************************************ */

void Yeast::budCreate()
{
    Assert(!hasBud());

    std::random_device rd;
    std::default_random_engine eng(g_rd());
    std::uniform_real_distribution<float> dist(0.f, 1.f);

    m_hasBud = true;
    m_bud.angle = units::Angle(2 * constants::PI * dist(eng));

    m_shapeForceUpdate = true;
}

/* ************************************************************************ */

void Yeast::budRelease()
{
    Assert(hasBud());

    // Calculate bud position
    const auto angle = getRotation();
    const auto offset = PositionVector(Zero, calcRadius(getVolume()) + calcRadius(getVolumeBud())).rotated(-getAngleBud());

    const auto omega = getAngularVelocity();
    const auto center = getMassCenterPosition();

    // Change yeast velocity
    if (omega != Zero)
    {
        setVelocity(getVelocity() + cross(omega, getPosition() - center));
        setAngularVelocity(omega);
    }

    // Get current position
    const auto posBud = getPosition() + offset.rotated(angle);
    const auto velocityBud = getVelocity() + cross(omega, getWorldPosition(offset) - center);

    // Release bud into the world
    auto bud = getSimulation().createObject<Yeast>();
    bud->setVolume(m_bud.volume);
    bud->setPosition(posBud);
    bud->setVelocity(velocityBud);
    bud->setAngularVelocity(omega);
    bud->setPrograms(getPrograms().clone());
    bud->setDensity(getDensity());
    bud->setGrowthRate(getGrowthRate());
    bud->updateShape();

    // Split molecules between yeast and bud

    // Total volume
    const auto totalVolume = getVolume() + m_bud.volume;

    // Copy old state
    const auto molecules = getMolecules();

    // Foreach molecules
    for (const auto& p : molecules)
    {
        // Molecules per volume unit
        const auto concentration = p.second / totalVolume;

        // Set molecule count to Yeast and bud
        setMoleculeCount(p.first, concentration * getVolume());
        bud->setMoleculeCount(p.first, concentration * bud->getVolume());
    }

    // Release bud
    m_hasBud = false;
    m_bud.volume = Zero;
    m_shapeForceUpdate = true;
}

/* ************************************************************************ */

#ifdef CECE_ENABLE_RENDER
void Yeast::draw(render::Context& context)
{
    if (!m_renderObject)
        m_renderObject.create(context);

    PositionVector pos;
    units::Length radius;
    units::Angle angle;
    units::Length budRadius;
    render::Color color;

    {
#ifdef CECE_THREAD_SAFE
        // Lock access
        MutexGuard guard(m_mutex);
#endif

        pos = getPosition();
        radius = calcRadius(getVolume());

        if (hasBud())
        {
            angle = getRotation() - m_bud.angle;
            budRadius = calcRadius(m_bud.volume);
            color = calcFluorescentColor(getVolume() + m_bud.volume);
        }
        else
        {
            angle = getRotation();
            budRadius = Zero;
            color = calcFluorescentColor(getVolume());
        }
    }

    // Transform
    context.matrixPush();
    context.matrixTranslate(pos);
    context.matrixRotate(angle);
    context.matrixScale(2 * radius.value());
    context.colorPush();
    context.enableAlpha();
    m_renderObject->draw(context, 0.5, 0.5 * (budRadius / radius), color, getIdentificationColor());
    context.disableAlpha();
    context.colorPop();
    context.matrixPop();
}
#endif

/* ************************************************************************ */

void Yeast::updateShape()
{
    static constexpr auto MIN_CHANGE = units::Length(0.10);

    // Alias for yeast shapes
    auto& shapes = getMutableShapes();

    bool needs_update = m_shapeForceUpdate;

    if (hasBud())
    {
        const auto newRadius = calcRadius(getVolume());
        const auto newBudRadius = calcRadius(getVolumeBud());
        const auto oldBudRadius = m_bud.lastRadius;
        needs_update = needs_update || ((newBudRadius - oldBudRadius) > MIN_CHANGE);

        const auto radiusMin = MIN_CHANGE;
        const auto radiusRelease = calcRadius(getVolumeBudRelease());

        const auto c1 = (radiusRelease + radiusMin) / (radiusRelease - radiusMin);
        const auto c2 = newRadius - 2 * radiusMin * (1 + radiusMin / (radiusRelease - radiusMin));
        const auto c = c1 * newBudRadius + c2;

        const auto center = 0.9 * PositionVector(Zero, c).rotated(-m_bud.angle);

        if (shapes.size() != 2)
        {
            shapes.push_back(Shape::makeCircle(newBudRadius, center));
        }
        else
        {
            shapes[1].getCircle().radius = newBudRadius;
            shapes[1].getCircle().center = center;
        }
    }
    else
    {
        const auto newRadius = calcRadius(getVolume());
        const auto oldRadius = m_lastRadius;
        needs_update = needs_update || ((newRadius - oldRadius) > MIN_CHANGE);

        // Update main shape
        shapes.resize(1);
        Assert(shapes.size() == 1);
        shapes[0].getCircle().radius = newRadius;
    }

    if (!needs_update)
        return;

    // Initialize shapes
    initShapes();
    m_shapeForceUpdate = false;
}

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */

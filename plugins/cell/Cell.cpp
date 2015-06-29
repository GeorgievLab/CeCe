
/* ************************************************************************ */

// Declaration
#include "Cell.hpp"

// C++
#include <cassert>

#ifdef ENABLE_PHYSICS
#include "Box2D/Box2D.h"
#endif

/* ************************************************************************ */

namespace plugin {
namespace cell {

/* ************************************************************************ */

Cell::Cell(simulator::Simulation& simulation, simulator::Object::Type type) NOEXCEPT
    : CellBase(simulation, type)
{
#if ENABLE_PHYSICS
    b2CircleShape shape;
    shape.m_radius = getRadius().value();

    getBody()->CreateFixture(&shape, getDensity().value());
#endif

    // Create initial shape
    auto& shapes = getMutableShapes();
    shapes.push_back(simulator::Shape::makeCircle(getRadius()));
}

/* ************************************************************************ */

void Cell::update(units::Duration dt)
{
    CellBase::update(dt);

#if ENABLE_PHYSICS
    // Update cell shape
    b2CircleShape shape;
    shape.m_radius = getRadius().value();

    // Update fixture
    b2Fixture* fixture = getBody()->GetFixtureList();
    assert(fixture);
    getBody()->DestroyFixture(fixture);
    getBody()->CreateFixture(&shape, getDensity().value());
#endif

    // Update shape
    auto& shapes = getMutableShapes();
    assert(shapes.size() == 1);
    shapes[0].circle.radius = getRadius();
}

/* ************************************************************************ */

void Cell::configure(const simulator::Configuration& config,
                     simulator::Simulation& simulation)
{
    CellBase::configure(config, simulation);
}

/* ************************************************************************ */

#if ENABLE_RENDER
void Cell::draw(render::Context& context)
{
    if (!m_renderObject)
        m_renderObject.create(context);

    const auto pos = getPosition();
    const auto radius = calcSphereRadius(getVolume());

    // Transform
    context.matrixPush();
    context.matrixTranslate(pos);
    context.matrixScale(2 * radius.value());
    context.matrixRotate(getRotation());

    m_renderObject->draw(context, radius.value());

    context.matrixPop();
}
#endif

/* ************************************************************************ */

}
}

/* ************************************************************************ */

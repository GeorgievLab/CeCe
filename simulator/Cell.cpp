
/* ************************************************************************ */

// Declaration
#include "simulator/Cell.hpp"

// Core
#include "simulator/World.hpp"

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

Cell::Id Cell::s_id = 0;

/* ************************************************************************ */

Cell::Cell(World* world, Shape shape)
    : physics::Cell(world, shape)
    , m_id(++s_id)
    , m_world(world)
{
    // Nothing to do
}

/* ************************************************************************ */

Cell::~Cell()
{
    // Nothing to do
}

/* ************************************************************************ */

void Cell::update()
{
    // Update object state
    const auto& fn = getBehaviour();

    if (fn)
        fn(*this);

    // Update physics object
    physics::Cell::update();
}

/* ************************************************************************ */

#ifdef ENABLE_RENDER

void Cell::draw(render::Context& context)
{
    auto gfp = getGfp();
    auto rfp = getRfp();
    auto yfp = getYfp();

    // TODO: Better calculation
    float red = rfp / 1000.f + yfp / 1000.f;
    float green = gfp / 1000.f + yfp / 1000.f;
    float blue = 0;

    auto pos = getPosition();
    auto x = pos.x;
    auto y = pos.y;
    auto z = pos.z;

    auto radius = calcSphereRadius(getVolume());

    context.drawSphere({x.value(), y.value(), z.value()}, radius.value(), {red, green, blue, 0.5f});
}

#endif

/* ************************************************************************ */

}

/* ************************************************************************ */

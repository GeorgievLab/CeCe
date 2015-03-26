
/* ************************************************************************ */

// Declaration
#include "simulator/Cell.hpp"

// Core
#include "simulator/World.hpp"

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

void Cell::update(units::Duration dt)
{
    DynamicObject::update(dt);
}

/* ************************************************************************ */

#ifdef ENABLE_RENDER

void Cell::render(render::Context& context)
{
    auto gfp = getGfp();
    auto rfp = getRfp();
    auto yfp = getYfp();

    // TODO: Better calculation
    float red = rfp / 1000.f + yfp / 1000.f;
    float green = gfp / 1000.f + yfp / 1000.f;
    float blue = 0;

    const auto pos = getPosition();
    const auto radius = calcSphereRadius(getVolume());

    context.drawCircle({pos.x, pos.y}, radius, {red, green, blue, 0.5f});
}

#endif

/* ************************************************************************ */

}

/* ************************************************************************ */

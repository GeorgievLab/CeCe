
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
    // Just update position
    DynamicObject::update(dt);
}

/* ************************************************************************ */

#ifdef ENABLE_RENDER
void Cell::renderInit(render::Context& context)
{
    m_renderObject.init();
}
#endif

/* ************************************************************************ */

#ifdef ENABLE_RENDER
void Cell::render(render::Context& context)
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

    m_renderObject.render(pos, radius, {red, green, blue, 0.8f});
}
#endif

/* ************************************************************************ */

}

/* ************************************************************************ */

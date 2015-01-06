
/* ************************************************************************ */

// Declaration
#include "Yeast.h"

// Simulator
#include "simulator/World.h"

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

Yeast::Yeast(World* world, MicroMeters x, MicroMeters y, BehaviourFn fn, MicroMeters radius)
    : Cell(world, x, y, std::move(fn))
    , m_radius(radius)
{
    // Use sphere body
    createSphereBody(radius);
}

/* ************************************************************************ */

Yeast::Yeast(World* world, MicroMeters x, MicroMeters y, BehaviourFn fn, MicroMeters3 volume)
    : Yeast(world, x, y, std::move(fn), calcRadius(volume))
{
    // Nothing to do
}

/* ************************************************************************ */

}

/* ************************************************************************ */

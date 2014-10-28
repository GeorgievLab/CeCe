
/* ************************************************************************ */

// Declaration
#include "Yeast.h"

// Simulator
#include "simulator/World.h"

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

Yeast::Yeast(World* world, MicroMeters x, MicroMeters y, MicroMeters radius)
    : Cell(world, x, y)
    , m_radius(radius)
{
    // Use sphere body
    createSphereBody(radius);
}

/* ************************************************************************ */

Yeast::Yeast(World* world, MicroMeters x, MicroMeters y, MicroMeters3 volume)
    : Yeast(world, x, y, calcRadius(volume))
{
    // Nothing to do
}

/* ************************************************************************ */

}

/* ************************************************************************ */

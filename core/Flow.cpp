
/* ************************************************************************ */

// Declaration
#include "Flow.h"

/* ************************************************************************ */

Flow::Flow(World* world, MicroMeters x, MicroMeters y, MicroMeters vx, MicroMeters vy)
    : m_world(world)
    , m_origin{x, y}
    , m_orientation{vx, vy}
{
    // Nothing to do
}

/* ************************************************************************ */

void Flow::Render(wxDC& dc) const noexcept
{
    dc.DrawLine(m_origin.x.value(), m_origin.y.value(), m_orientation.x.value(), m_orientation.y.value());
}

/* ************************************************************************ */

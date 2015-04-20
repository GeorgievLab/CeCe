
/* ************************************************************************ */

// Declaration
#include "simulator/DynamicObject.hpp"

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

void DynamicObject::update(units::Duration dt)
{
    // Update object velocity
    setVelocity(getVelocity() + getAcceleration() * dt);

    // Calculate new object position
    setPosition(getPosition() + getVelocity() * dt);

    // Reset acceleration
    m_acceleration = {};
}

/* ************************************************************************ */

}

/* ************************************************************************ */



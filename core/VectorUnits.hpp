
#pragma once

/* ************************************************************************ */

// Core
#include "core/Units.hpp"
#include "core/Vector.hpp"

/* ************************************************************************ */

inline namespace core {

/* ************************************************************************ */

/**
 * @brief Position vector structure.
 */
using PositionVector = Vector<units::Length>;

/* ************************************************************************ */

/**
 * @brief Velocity vector structure.
 */
using VelocityVector = Vector<units::Velocity>;

/* ************************************************************************ */

/**
 * @brief Acceleration vector structure.
 */
using AccelerationVector = Vector<units::Acceleration>;

/* ************************************************************************ */

/**
 * @brief Force vector structure.
 */
using ForceVector = Vector<units::Force>;

/* ************************************************************************ */

}

/* ************************************************************************ */

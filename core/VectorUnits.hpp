/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* ************************************************************************ */

#pragma once

/* ************************************************************************ */

// Core
#include "core/Units.hpp"
#include "core/Vector.hpp"

/* ************************************************************************ */

#ifndef _MSC_VER
inline namespace core {
#endif

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

/**
 * @brief Scale vector.
 */
using ScaleVector = Vector<float>;

/* ************************************************************************ */

/**
 * @brief Size vector.
 */
using SizeVector = Vector<units::Length>;

/* ************************************************************************ */

#ifndef _MSC_VER
}
#endif

/* ************************************************************************ */

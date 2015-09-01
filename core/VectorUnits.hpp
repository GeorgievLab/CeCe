/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */

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

#if !defined(__GNUC__) // NOTE: GCC have problem with =default constructor in linking stage.
extern template class Vector<units::Length>;
extern template class Vector<units::Velocity>;
extern template class Vector<units::Acceleration>;
extern template class Vector<units::Force>;
#endif

/* ************************************************************************ */

}

/* ************************************************************************ */

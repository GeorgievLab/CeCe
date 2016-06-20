/* ************************************************************************ */
/* Georgiev Lab (c) 2015-2016                                               */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/*                                                                          */
/* This file is part of CeCe.                                               */
/*                                                                          */
/* CeCe is free software: you can redistribute it and/or modify             */
/* it under the terms of the GNU General Public License as published by     */
/* the Free Software Foundation, either version 3 of the License, or        */
/* (at your option) any later version.                                      */
/*                                                                          */
/* CeCe is distributed in the hope that it will be useful,                  */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of           */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            */
/* GNU General Public License for more details.                             */
/*                                                                          */
/* You should have received a copy of the GNU General Public License        */
/* along with CeCe.  If not, see <http://www.gnu.org/licenses/>.            */
/*                                                                          */
/* ************************************************************************ */

#pragma once

/* ************************************************************************ */

// CeCe
#include "cece/core/Real.hpp"
#include "cece/core/Units.hpp"
#include "cece/core/Vector.hpp"

/* ************************************************************************ */

namespace cece {
inline namespace core {

/* ************************************************************************ */

namespace units {

/* ************************************************************************ */

/**
 * @brief Position vector structure.
 */
using PositionVector = Vector<Length>;

/* ************************************************************************ */

/**
 * @brief Velocity vector structure.
 */
using VelocityVector = Vector<Velocity>;

/* ************************************************************************ */

/**
 * @brief Acceleration vector structure.
 */
using AccelerationVector = Vector<Acceleration>;

/* ************************************************************************ */

/**
 * @brief Force vector structure.
 */
using ForceVector = Vector<Force>;

/* ************************************************************************ */

/**
 * @brief Impulse vector structure.
 */
using ImpulseVector = Vector<Impulse>;

/* ************************************************************************ */

/**
 * @brief Scale vector.
 */
using ScaleVector = Vector<RealType>;

/* ************************************************************************ */

/**
 * @brief Size vector.
 */
using SizeVector = Vector<Length>;

/* ************************************************************************ */

}

/* ************************************************************************ */

extern template class BasicVector<units::Length, config::DIMENSION>;
extern template class BasicVector<units::Velocity, config::DIMENSION>;
extern template class BasicVector<units::Acceleration, config::DIMENSION>;
extern template class BasicVector<units::Force, config::DIMENSION>;
extern template class BasicVector<units::Impulse, config::DIMENSION>;
extern template class BasicVector<RealType, config::DIMENSION>;

/* ************************************************************************ */

}
}

/* ************************************************************************ */

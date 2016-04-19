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
#include "cece/core/Unit.hpp"

/* ************************************************************************ */

namespace cece {
inline namespace core {

/* ************************************************************************ */

namespace units {

/* ************************************************************************ */

/**
 * @brief Type without units.
 */
using None = Unit<List<>, List<>>;

/* ************************************************************************ */

/**
 * @brief Class for representing distance (meters).
 */
using Length = Unit<List<BaseLength>, List<>>;

/* ************************************************************************ */

/**
 * @brief Class for representing mass (kilograms).
 */
using Mass = Unit<List<BaseMass>, List<>>;

/* ************************************************************************ */

/**
 * @brief Class for representing time (seconds).
 */
using Time = Unit<List<BaseTime>, List<>>;
using Duration = Time;

/* ************************************************************************ */

/**
 * @brief Class for representing area.
 */
using Area = Unit<List<BaseLength, BaseLength>, List<>>;

/* ************************************************************************ */

/**
 * @brief Class for representing volume.
 */
using Volume = Unit<List<BaseLength, BaseLength, BaseLength>, List<>>;

/* ************************************************************************ */

/**
 * @brief Class for representing velocity (micrometers per second).
 */
using Velocity = Unit<List<BaseLength>, List<BaseTime>>;

/* ************************************************************************ */

/**
 * @brief Class for representing acceleration (micrometers per second^2).
 */
using Acceleration = Unit<List<BaseLength>, List<BaseTime, BaseTime>>;

/* ************************************************************************ */

/**
 * @brief Class for representing force (Newton).
 */
using Force = Unit<List<BaseLength, BaseMass>, List<BaseTime, BaseTime>>;

/* ************************************************************************ */

/**
 * @brief Class for representing impulse.
 */
using Impulse = Unit<List<BaseLength, BaseMass>, List<BaseTime>>;

/* ************************************************************************ */

/**
 * @brief Class for representing density.
 */
using Density = Unit<List<BaseMass>, List<BaseLength, BaseLength, BaseLength>>;

/* ************************************************************************ */

/**
 * @brief Class for representing dynamic viscosity.
 */
using DynamicViscosity = Unit<List<BaseMass>, List<BaseLength, BaseTime>>;

/* ************************************************************************ */

/**
 * @brief Class for representing kinematic viscosity.
 */
using KinematicViscosity = Unit<List<BaseLength, BaseLength>, List<BaseTime>>;

/* ************************************************************************ */

/**
 * @brief Amount of substance.
 */
using AmountOfSubstance = Unit<List<BaseAmountOfSubstance>, List<>>;

/* ************************************************************************ */

/**
 * @brief Molar concentration.
 */
using MolarConcentration = Unit<List<BaseAmountOfSubstance>, List<BaseLength, BaseLength, BaseLength>>;

/* ************************************************************************ */

/**
 * @brief Class for representing volume.
 */
using Angle = Unit<List<>, List<>>;

/* ************************************************************************ */

/**
 * @brief Class for representing angular velocity (radian per second).
 */
using AngularVelocity = Unit<List<>, List<BaseTime>>;

/* ************************************************************************ */

/**
 * @brief Class for representing probability.
 */
using Probability = Unit<List<>, List<>>;

/* ************************************************************************ */

/**
 * @brief Convert degrees to radians.
 *
 * @param value
 *
 * @return
 */
inline constexpr Value deg2rad(Value value) noexcept
{
    return value * 0.01745329252f;
}

/* ************************************************************************ */

/**
 * @brief Convert radians to degrees.
 *
 * @param value
 *
 * @return
 */
inline constexpr Value rad2deg(Value value) noexcept
{
    return value * 57.2957795f;
}

/* ************************************************************************ */

}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

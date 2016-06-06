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
#include "cece/core/Units.hpp"

/* ************************************************************************ */

namespace cece {
inline namespace core {

/* ************************************************************************ */

namespace units {

/* ************************************************************************ */

/**
 * @brief Meter value.
 *
 * @param value Value.
 *
 * @return Length value.
 */
inline constexpr Length m(Value value) noexcept
{
    // 1m = 1'000mm
    return Length(value * exponentToCoefficient(LENGTH_EXPONENT));
}

/* ************************************************************************ */

/**
 * @brief Decimeter value.
 *
 * @param value Value.
 *
 * @return Length value.
 */
inline constexpr Length dm(Value value) noexcept
{
    return m(value * 1e-1);
}

/* ************************************************************************ */

/**
 * @brief Millimeter value.
 *
 * @param value Value.
 *
 * @return Length value.
 */
inline constexpr Length mm(Value value) noexcept
{
    // 1mm = 1/1'000m
    return m(value * 1e-3);
}

/* ************************************************************************ */

/**
 * @brief Micrometer value.
 *
 * @param value Value.
 *
 * @return Length value.
 */
inline constexpr Length um(Value value) noexcept
{
    // 1um = 1/1'000'000m
    return m(value * 1e-6);
}

/* ************************************************************************ */

/**
 * @brief Grams value.
 *
 * @param value Value.
 *
 * @return Mass value.
 */
inline constexpr Mass g(Value value) noexcept
{
    return Mass(value * exponentToCoefficient(MASS_EXPONENT));
}

/* ************************************************************************ */

/**
 * @brief Kilograms value.
 *
 * @param value Value.
 *
 * @return Mass value.
 */
inline constexpr Mass kg(Value value) noexcept
{
    // 1g = 1/1'000 kg
    return g(value * 1e3);
}

/* ************************************************************************ */

/**
 * @brief Milligrams value.
 *
 * @param value Value.
 *
 * @return Mass value.
 */
inline constexpr Mass mg(Value value) noexcept
{
    // 1mg = 1/1'000 g
    return g(value * 1e-3);
}

/* ************************************************************************ */

/**
 * @brief Micrograms value.
 *
 * @param value Value.
 *
 * @return Mass value.
 */
inline constexpr Mass ug(Value value) noexcept
{
    // 1ug = 1/1'000 mg
    return mg(value * 1e-3);
}

/* ************************************************************************ */

/**
 * @brief Nanograms value.
 *
 * @param value Value.
 *
 * @return Mass value.
 */
inline constexpr Mass ng(Value value) noexcept
{
    // 1ng = 1/1'000 ug
    return ug(value * 1e-3);
}

/* ************************************************************************ */

/**
 * @brief Micrograms value.
 *
 * @param value Value.
 *
 * @return Mass value.
 */
inline constexpr Mass pg(Value value) noexcept
{
    // 1pg = 1/1'000 ng
    return ng(value * 1e-3);
}

/* ************************************************************************ */

/**
 * @brief Seconds value.
 *
 * @param value Value.
 *
 * @return Time value.
 */
inline constexpr Time s(Value value) noexcept
{
    // 1s
    return Time(value * exponentToCoefficient(TIME_EXPONENT));
}

/* ************************************************************************ */

/**
 * @brief Milliseconds value.
 *
 * @param value Value.
 *
 * @return Time value.
 */
inline constexpr Time ms(Value value) noexcept
{
    // 1s = 1'000ms
    return s(value * 1e-3);
}

/* ************************************************************************ */

/**
 * @brief Microseconds value.
 *
 * @param value Value.
 *
 * @return Time value.
 */
inline constexpr Time us(Value value) noexcept
{
    // 1ms = 1'000us
    return ms(value * 1e-3);
}

/* ************************************************************************ */

/**
 * @brief Minutes value.
 *
 * @param value Value.
 *
 * @return Time value.
 */
inline constexpr Time min(Value value) noexcept
{
    // 60s = 1min
    return s(60 * value);
}

/* ************************************************************************ */

/**
 * @brief Hours value.
 *
 * @param value Value.
 *
 * @return Time value.
 */
inline constexpr Time h(Value value) noexcept
{
    // 60min = 1h
    return min(60 * value);
}

/* ************************************************************************ */

/**
 * @brief Meters^2 value.
 *
 * @param value Value.
 *
 * @return Area value.
 */
inline constexpr Area m2(Value value) noexcept
{
    return m(value) * m(1);
}

/* ************************************************************************ */

/**
 * @brief Decimeters^2 value.
 *
 * @param value Value.
 *
 * @return Area value.
 */
inline constexpr Area dm2(Value value) noexcept
{
    return dm(value) * dm(1);
}

/* ************************************************************************ */

/**
 * @brief Millimeters^2 value.
 *
 * @param value Value.
 *
 * @return Area value.
 */
inline constexpr Area mm2(Value value) noexcept
{
    return mm(value) * mm(1);
}

/* ************************************************************************ */

/**
 * @brief Micrometers^2 value.
 *
 * @param value Value.
 *
 * @return Area value.
 */
inline constexpr Area um2(Value value) noexcept
{
    return um(value) * um(1);
}

/* ************************************************************************ */

/**
 * @brief Meters^3 value.
 *
 * @param value Value.
 *
 * @return Volume value.
 */
inline constexpr Volume m3(Value value) noexcept
{
    return m2(value) * m(1);
}

/* ************************************************************************ */

/**
 * @brief Decimeters^3 value.
 *
 * @param value Value.
 *
 * @return Volume value.
 */
inline constexpr Volume dm3(Value value) noexcept
{
    return dm2(value) * dm(1);
}

/* ************************************************************************ */

/**
 * @brief Millimeters^3 value.
 *
 * @param value Value.
 *
 * @return Volume value.
 */
inline constexpr Volume mm3(Value value) noexcept
{
    return mm2(value) * mm(1);
}

/* ************************************************************************ */

/**
 * @brief Micrometers^3 value.
 *
 * @param value Value.
 *
 * @return Volume value.
 */
inline constexpr Volume um3(Value value) noexcept
{
    return um2(value) * um(1);
}

/* ************************************************************************ */

/**
 * @brief Meters per second value.
 *
 * @param value Value.
 *
 * @return Velocity value.
 */
inline constexpr Velocity m_s(Value value) noexcept
{
    return m(value) / s(1);
}

/* ************************************************************************ */

/**
 * @brief Millimeters per second value.
 *
 * @param value Value.
 *
 * @return Velocity value.
 */
inline constexpr Velocity mm_s(Value value) noexcept
{
    return mm(value) / s(1);
}

/* ************************************************************************ */

/**
 * @brief Micrometers per second value.
 *
 * @param value Value.
 *
 * @return Velocity value.
 */
inline constexpr Velocity um_s(Value value) noexcept
{
    return um(value) / s(1);
}

/* ************************************************************************ */

/**
 * @brief Meters per second^2 value.
 *
 * @param value Value.
 *
 * @return Acceleration value.
 */
inline constexpr Acceleration m_s2(Value value) noexcept
{
    return m_s(value) / s(1);
}

/* ************************************************************************ */

/**
 * @brief Millimeters per second^2 value.
 *
 * @param value Value.
 *
 * @return Acceleration value.
 */
inline constexpr Acceleration mm_s2(Value value) noexcept
{
    return mm_s(value) / s(1);
}

/* ************************************************************************ */

/**
 * @brief Micrometers per second^2 value.
 *
 * @param value Value.
 *
 * @return Acceleration value.
 */
inline constexpr Acceleration um_s2(Value value) noexcept
{
    return um_s(value) / s(1);
}

/* ************************************************************************ */

/**
 * @brief Kilogram meter per second^2 value.
 *
 * @param value Value.
 *
 * @return Force value.
 */
inline constexpr Force kgm_s2(Value value) noexcept
{
    return kg(value) * m_s2(1);
}

/* ************************************************************************ */

/**
 * @brief Gram meter per second^2 value.
 *
 * @param value Value.
 *
 * @return Force value.
 */
inline constexpr Force gm_s2(Value value) noexcept
{
    return g(value) * m_s2(1);
}

/* ************************************************************************ */

/**
 * @brief Milligram meter per second^2 value.
 *
 * @param value Value.
 *
 * @return Force value.
 */
inline constexpr Force mgm_s2(Value value) noexcept
{
    return kg(value) * m_s2(1);
}

/* ************************************************************************ */

/**
 * @brief Newton value.
 *
 * @param value Value.
 *
 * @return Force value.
 */
inline constexpr Force N(Value value) noexcept
{
    return kgm_s2(value);
}

/* ************************************************************************ */

/**
 * @brief Millinewton value.
 *
 * @param value Value.
 *
 * @return Force value.
 */
inline constexpr Force mN(Value value) noexcept
{
    return gm_s2(value);
}

/* ************************************************************************ */

/**
 * @brief Micronewton value.
 *
 * @param value Value.
 *
 * @return Force value.
 */
inline constexpr Force uN(Value value) noexcept
{
    return mgm_s2(value);
}

/* ************************************************************************ */

/**
 * @brief Kilogram per meter and second value.
 *
 * @param value Value.
 *
 * @return Viscosity value.
 */
inline constexpr DynamicViscosity kg_m_s(Value value) noexcept
{
    return kg(value) / (m(1) * s(1));
}

/* ************************************************************************ */

/**
 * @brief Kilogram per meter and second value.
 *
 * @param value Value.
 *
 * @return Viscosity value.
 */
inline constexpr DynamicViscosity g_m_s(Value value) noexcept
{
    return g(value) / (m(1) * s(1));
}

/* ************************************************************************ */

/**
 * @brief Newton second per meter square value.
 *
 * @param value Value.
 *
 * @return Viscosity value.
 */
inline constexpr DynamicViscosity Ns_m2(Value value) noexcept
{
    return N(value) * s(1) / m2(1);
}

/* ************************************************************************ */

/**
 * @brief Newton second per meter square value.
 *
 * @param value Value.
 *
 * @return Viscosity value.
 */
inline constexpr DynamicViscosity Pas(Value value) noexcept
{
    return N(value) * s(1) / m2(1);
}

/* ************************************************************************ */

/**
 * @brief Newton second per meter square value.
 *
 * @param value Value.
 *
 * @return Viscosity value.
 */
inline constexpr DynamicViscosity mPas(Value value) noexcept
{
    return mN(value) * s(1) / m2(1);
}

/* ************************************************************************ */

/**
 * @brief Meter square per second.
 *
 * @param value Value.
 *
 * @return Viscosity value.
 */
inline constexpr KinematicViscosity m2_s(Value value) noexcept
{
    return m2(value) / s(1);
}

/* ************************************************************************ */

/**
 * @brief Millimeter square per second.
 *
 * @param value Value.
 *
 * @return Viscosity value.
 */
inline constexpr KinematicViscosity mm2_s(Value value) noexcept
{
    return mm2(value) / s(1);
}

/* ************************************************************************ */

/**
 * @brief Micrometer square per second.
 *
 * @param value Value.
 *
 * @return Viscosity value.
 */
inline constexpr KinematicViscosity um2_s(Value value) noexcept
{
    return um2(value) / s(1);
}

/* ************************************************************************ */

/**
 * @brief Mole value.
 *
 * @param value
 *
 * @return Amount of substance.
 */
inline constexpr AmountOfSubstance mol(Value value) noexcept
{
    return AmountOfSubstance(value * exponentToCoefficient(AMOUNT_OF_SUBSTANCE_EXPONENT));
}

/* ************************************************************************ */

/**
 * @brief Millimole value.
 *
 * @param value
 *
 * @return Amount of substance.
 */
inline constexpr AmountOfSubstance mmol(Value value) noexcept
{
    return mol(value * 1e-3);
}

/* ************************************************************************ */

/**
 * @brief Micromole value.
 *
 * @param value
 *
 * @return Amount of substance.
 */
inline constexpr AmountOfSubstance umol(Value value) noexcept
{
    return mmol(value * 1e-3);
}

/* ************************************************************************ */

/**
 * @brief Nanomole value.
 *
 * @param value
 *
 * @return Amount of substance.
 */
inline constexpr AmountOfSubstance nmol(Value value) noexcept
{
    return umol(value * 1e-3);
}

/* ************************************************************************ */

/**
 * @brief Returns concentration in mole per meter cubic.
 *
 * @param value
 *
 * @return Molar concentration.
 */
inline constexpr MolarConcentration mol_m3(Value value) noexcept
{
    return mol(value) / m3(1);
}

/* ************************************************************************ */

/**
 * @brief Returns concentration in millimole per meter cubic.
 *
 * @param value
 *
 * @return Molar concentration.
 */
inline constexpr MolarConcentration mmol_m3(Value value) noexcept
{
    return mmol(value) / m3(1);
}

/* ************************************************************************ */

/**
 * @brief Returns concentration in micromole per meter cubic.
 *
 * @param value
 *
 * @return Molar concentration.
 */
inline constexpr MolarConcentration umol_m3(Value value) noexcept
{
    return umol(value) / m3(1);
}

/* ************************************************************************ */

/**
 * @brief Returns concentration in nanomole per meter cubic.
 *
 * @param value
 *
 * @return Molar concentration.
 */
inline constexpr MolarConcentration nmol_m3(Value value) noexcept
{
    return nmol(value) / m3(1);
}

/* ************************************************************************ */

/**
 * @brief Returns concentration in Molar.
 *
 * @param value
 *
 * @return Molar concentration.
 */
inline constexpr MolarConcentration M(Value value) noexcept
{
    return mol(value) / dm3(1);
}

/* ************************************************************************ */

/**
 * @brief Returns concentration in miniMolar.
 *
 * @param value
 *
 * @return Molar concentration.
 */
inline constexpr MolarConcentration mM(Value value) noexcept
{
    return mmol(value) / dm3(1);
}

/* ************************************************************************ */

/**
 * @brief Returns concentration in microMolar.
 *
 * @param value
 *
 * @return Molar concentration.
 */
inline constexpr MolarConcentration uM(Value value) noexcept
{
    return umol(value) / dm3(1);
}

/* ************************************************************************ */

/**
 * @brief Returns concentration in nanoMolar.
 *
 * @param value
 *
 * @return Molar concentration.
 */
inline constexpr MolarConcentration nM(Value value) noexcept
{
    return nmol(value) / dm3(1);
}

/* ************************************************************************ */

/**
 * @brief Returns concentration in mole per micrometer cubic.
 *
 * @param value
 *
 * @return Molar concentration.
 */
inline constexpr MolarConcentration mol_um3(Value value) noexcept
{
    return mol(value) / um3(1);
}

/* ************************************************************************ */

/**
 * @brief Returns concentration in millimole per micrometer cubic.
 *
 * @param value
 *
 * @return Molar concentration.
 */
inline constexpr MolarConcentration mmol_um3(Value value) noexcept
{
    return mmol(value) / um3(1);
}

/* ************************************************************************ */

/**
 * @brief Returns concentration in micromole per micrometer cubic.
 *
 * @param value
 *
 * @return Molar concentration.
 */
inline constexpr MolarConcentration umol_um3(Value value) noexcept
{
    return umol(value) / um3(1);
}

/* ************************************************************************ */

/**
 * @brief Returns concentration in nanomole per micrometer cubic.
 *
 * @param value
 *
 * @return Molar concentration.
 */
inline constexpr MolarConcentration nmol_um3(Value value) noexcept
{
    return nmol(value) / um3(1);
}

/* ************************************************************************ */

/**
 * @brief Radian constructor.
 *
 * @param value Value in radians.
 *
 * @return Angle.
 */
inline constexpr Angle rad(Value value) noexcept
{
    return Angle(value);
}

/* ************************************************************************ */

/**
 * @brief Degree constructor.
 *
 * @param value Value in degrees.
 *
 * @return Angle.
 */
inline constexpr Angle deg(Value value) noexcept
{
    return rad(deg2rad(value));
}

/* ************************************************************************ */

/**
 * @brief Probability constructor.
 *
 * @param value Value in precent.
 *
 * @return
 */
inline constexpr Probability precent(Value value) noexcept
{
    return Probability(value / 100.0);
}

/* ************************************************************************ */

}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

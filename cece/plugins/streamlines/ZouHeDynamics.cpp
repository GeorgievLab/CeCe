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

// Declaration
#include "cece/plugins/streamlines/ZouHeDynamics.hpp"

// CeCe
#include "cece/core/Map.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace streamlines {

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

const Map<ZouHeDynamics::Position, StaticArray<Descriptor::DirectionType, 3>> CENTER_RHO{{
    {ZouHeDynamics::Position::Right,    Descriptor::MIDDLE_COLUMN},
    {ZouHeDynamics::Position::Left,     Descriptor::MIDDLE_COLUMN},
    {ZouHeDynamics::Position::Top,      Descriptor::MIDDLE_LINE},
    {ZouHeDynamics::Position::Bottom,   Descriptor::MIDDLE_LINE}
}};

/* ************************************************************************ */

const Map<ZouHeDynamics::Position, StaticArray<Descriptor::DirectionType, 3>> KNOWN_RHO{{
    {ZouHeDynamics::Position::Right,    Descriptor::RIGHT_COLUMN},
    {ZouHeDynamics::Position::Left,     Descriptor::LEFT_COLUMN},
    {ZouHeDynamics::Position::Top,      Descriptor::TOP_LINE},
    {ZouHeDynamics::Position::Bottom,   Descriptor::BOTTOM_LINE}
}};

/* ************************************************************************ */

const Map<ZouHeDynamics::Position, StaticArray<Descriptor::DirectionType, 3>> UNKNOWN_RHO{{
    {ZouHeDynamics::Position::Right,    Descriptor::LEFT_COLUMN},
    {ZouHeDynamics::Position::Left,     Descriptor::RIGHT_COLUMN},
    {ZouHeDynamics::Position::Top,      Descriptor::BOTTOM_LINE},
    {ZouHeDynamics::Position::Bottom,   Descriptor::TOP_LINE}
}};

/* ************************************************************************ */

const Map<ZouHeDynamics::Position, ZouHeDynamics::VelocityType> VELOCITIES{{
    {ZouHeDynamics::Position::Right,    {-1,  0}},
    {ZouHeDynamics::Position::Left,     { 1,  0}},
    {ZouHeDynamics::Position::Top,      { 0, -1}},
    {ZouHeDynamics::Position::Bottom,   { 0,  1}}
}};

/* ************************************************************************ */

const Map<ZouHeDynamics::Position, Descriptor::DirectionType> BC_CENTER{{
    {ZouHeDynamics::Position::Right,    Descriptor::INDEX_MAP[1][0]},
    {ZouHeDynamics::Position::Left,     Descriptor::INDEX_MAP[1][2]},
    {ZouHeDynamics::Position::Top,      Descriptor::INDEX_MAP[2][1]},
    {ZouHeDynamics::Position::Bottom,   Descriptor::INDEX_MAP[0][1]}
}};

/* ************************************************************************ */

const Map<ZouHeDynamics::Position, StaticArray<Descriptor::DirectionType, 2>> BC_SIDE1{{
    {ZouHeDynamics::Position::Right,    {{Descriptor::INDEX_MAP[0][0], Descriptor::INDEX_MAP[2][1]}}},
    {ZouHeDynamics::Position::Left,     {{Descriptor::INDEX_MAP[0][2], Descriptor::INDEX_MAP[2][1]}}},
    {ZouHeDynamics::Position::Top,      {{Descriptor::INDEX_MAP[2][2], Descriptor::INDEX_MAP[1][0]}}},
    {ZouHeDynamics::Position::Bottom,   {{Descriptor::INDEX_MAP[0][2], Descriptor::INDEX_MAP[1][0]}}}
}};

/* ************************************************************************ */

const Map<ZouHeDynamics::Position, StaticArray<Descriptor::DirectionType, 2>> BC_SIDE2{{
    {ZouHeDynamics::Position::Right,    {{Descriptor::INDEX_MAP[2][0], Descriptor::INDEX_MAP[0][1]}}},
    {ZouHeDynamics::Position::Left,     {{Descriptor::INDEX_MAP[2][2], Descriptor::INDEX_MAP[0][1]}}},
    {ZouHeDynamics::Position::Top,      {{Descriptor::INDEX_MAP[2][0], Descriptor::INDEX_MAP[1][2]}}},
    {ZouHeDynamics::Position::Bottom,   {{Descriptor::INDEX_MAP[0][0], Descriptor::INDEX_MAP[1][2]}}}
}};

/* ************************************************************************ */

/**
 * @brief Compute of total sum of given value indices.
 *
 * @tparam N Number of indices.
 *
 * @param list List of indices to sum.
 *
 * @return
 */
template<std::size_t N>
ZouHeDynamics::DensityType
sumValues(ZouHeDynamics::DataType& data, StaticArray<Descriptor::DirectionType, N> list) noexcept
{
    ZouHeDynamics::DensityType density{};

    for (auto iPop : list)
        density += data[iPop];

    return density;
}

/* ************************************************************************ */

}

/* ************************************************************************ */

void
ZouHeDynamics::defineDensity(DataType& data, DensityType density) const noexcept
{
    const auto velocity = calcVelocity(data, density);
    init(data, velocity, density);
}

/* ************************************************************************ */

void
ZouHeDynamics::defineVelocity(DataType& data, VelocityType velocity) const noexcept
{
    const auto density = calcDensity(data, velocity);
    init(data, velocity, density);
}

/* ************************************************************************ */

ZouHeDynamics::DensityType
ZouHeDynamics::calcDensity(DataType& data, const VelocityType& velocity) const noexcept
{
    const auto center = sumValues(data, CENTER_RHO.at(m_position));
    const auto known  = sumValues(data, KNOWN_RHO.at(m_position));
    const auto velP = velocity.dot(VELOCITIES.at(m_position));

    return 1.0 / (1.0 - velP) * (center + 2 * known);
}

/* ************************************************************************ */

ZouHeDynamics::VelocityType
ZouHeDynamics::calcVelocity(DataType& data, DensityType rho) const noexcept
{
    const auto center = sumValues(data, CENTER_RHO.at(m_position));
    const auto known  = sumValues(data, KNOWN_RHO.at(m_position));

    // Speed
    const RealType speed = (1.0 - 1.0 / rho * (center + 2 * known));

    // Velocity vector
    return speed * VELOCITIES.at(m_position);
}

/* ************************************************************************ */

void ZouHeDynamics::init(DataType& data, VelocityType velocity, DensityType density) const noexcept
{
    const auto center = BC_CENTER.at(m_position);
    const auto side1 = BC_SIDE1.at(m_position);
    const auto side2 = BC_SIDE2.at(m_position);

    auto eqDiff = [this, &density, &velocity] (Descriptor::DirectionType iPop) {
        return
            computeEquilibrium(iPop, density, velocity) -
            computeEquilibrium(Descriptor::opposite(iPop), density, velocity)
        ;
    };

    auto dataDiff = [&data] (Descriptor::DirectionType iPop) {
        return data[iPop] - data[Descriptor::opposite(iPop)];
    };

    const auto side1_0 = side1[0];
    const auto side1_1 = side1[1];
    const auto side2_0 = side2[0];
    const auto side2_1 = side2[1];

    // Center
    data[center] = data[Descriptor::opposite(center)]
        + eqDiff(center)
    ;
    Assert(data[center] > 0);

    // Side 1
    data[side1_0] = data[Descriptor::opposite(side1_0)]
        + eqDiff(side1_0)
        + 0.5 * dataDiff(side1_1)
    ;
    Assert(data[side1_0] > 0);

    // Side 2
    data[side2_0] = data[Descriptor::opposite(side2_0)]
        + eqDiff(side2_0)
        + 0.5 * dataDiff(side2_1)
    ;
    Assert(data[side2_0] > 0);
}

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */
